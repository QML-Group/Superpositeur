#include "superpositeur/LibqasmInterface.hpp"

#include "v1x/cqasm-semantic-gen.hpp"
#include "superpositeur/Circuit.hpp"
#include "superpositeur/MixedState.hpp"
#include "superpositeur/Operations.hpp"

namespace superpositeur {

namespace cq = ::cqasm::v1x::semantic;
namespace values = ::cqasm::v1x::values;

namespace {
Operations::Signature getSignature(cq::Instruction const &instruction) {
    Operations::Signature res;
    for (auto const &op : instruction.operands) {
        if (op->as_const_real()) {
            res.push_back(OperandType::Double);
        } else if (op->as_const_int()) {
            res.push_back(OperandType::Int);
        } else if (op->as_qubit_refs()) {
            res.push_back(OperandType::Qubit);
        } else if (op->as_bit_refs()) {
            throw std::runtime_error("Classical bits not allowed as operands");
        } else {
            throw std::runtime_error("Unhandled operand type");
        }
    }
    return res;
}

std::vector<std::vector<QubitIndex>>
getDynamicOperands(cq::Instruction const &instruction) {
    std::vector<std::vector<QubitIndex>> res;
    for (auto const &op : instruction.operands) {
        if (auto qubitRefs = op->as_qubit_refs()) {
            assert(res.size() == 0 || res.size() == qubitRefs->index.size());
            res.resize(std::max(res.size(), qubitRefs->index.size()));
            for (std::uint64_t i = 0; i < qubitRefs->index.size(); ++i) {
                res[i].push_back(QubitIndex{
                    static_cast<std::uint64_t>(qubitRefs->index[i]->value)});
            }
        } else if (auto bitRefs = op->as_bit_refs()) {
            // assert(res.size() == 0 || res.size() == bitRefs->index.size());
            // res.resize(std::max(res.size(), bitRefs->index.size()));
            // for (std::uint64_t i = 0; i < bitRefs->index.size(); ++i) {
            //     res[i].push_back(MeasurementRegisterIndex{
            //         static_cast<std::uint64_t>(bitRefs->index[i]->value)});
            // }
            (void) bitRefs;
            throw std::runtime_error("Unimplemented");
        }
    }
    return res;
}

Operations::StaticOperands
getStaticOperands(cq::Instruction const &instruction) {
    Operations::StaticOperands res;
    for (auto const &op : instruction.operands) {
        if (auto real = op->as_const_real()) {
            res.push_back(real->value);
        } else if (auto integer = op->as_const_int()) {
            res.push_back(integer->value);
        } else if (op->as_qubit_refs()) {
            // No-op
        } else if (op->as_bit_refs()) {
            // No-op
        } else {
            throw std::runtime_error("Unhandled operand type");
        }
    }
    return res;
}

std::string to_string(cq::NodeType nodeType) {
    switch (nodeType) {
    case cq::NodeType::AnnotationData:
        return "AnnotationData";
    case cq::NodeType::Block:
        return "Block";
    case cq::NodeType::BreakStatement:
        return "BreakStatement";
    case cq::NodeType::Bundle:
        return "Bundle";
    case cq::NodeType::BundleExt:
        return "BundleExt";
    case cq::NodeType::ContinueStatement:
        return "ContinueStatement";
    case cq::NodeType::ErrorModel:
        return "ErrorModel";
    case cq::NodeType::ForLoop:
        return "ForLoop";
    case cq::NodeType::ForeachLoop:
        return "ForeachLoop";
    case cq::NodeType::GotoInstruction:
        return "GotoInstruction";
    case cq::NodeType::IfElse:
        return "IfElse";
    case cq::NodeType::IfElseBranch:
        return "IfElseBranch";
    case cq::NodeType::Instruction:
        return "Instruction";
    case cq::NodeType::Mapping:
        return "Mapping";
    case cq::NodeType::Program:
        return "Program";
    case cq::NodeType::RepeatUntilLoop:
        return "RepeatUntilLoop";
    case cq::NodeType::SetInstruction:
        return "SetInstruction";
    case cq::NodeType::Subcircuit:
        return "Subcircuit";
    case cq::NodeType::Variable:
        return "Variable";
    case cq::NodeType::Version:
        return "Version";
    case cq::NodeType::WhileLoop:
        return "WhileLoop";
    }

    return "Unknown";
}

class GateConvertor : public cq::RecursiveVisitor {
public:
    GateConvertor(std::vector<CircuitInstruction> &b, Operations const &ops)
        : block(b), operations(ops) {}

    void visit_instruction(cq::Instruction &instr) override {
        addQuantumOperation(instr);
    }

    void visit_bundle_ext(cq::BundleExt &node) override {
        node.items.visit(*this);
    }

    void visit_node(cq::Node &node) override {
        throw std::runtime_error("Statements of the following type are not "
                                 "supported by the simulator: " +
                                 to_string(node.type()));
    }

private:
    void addQuantumOperation(const cq::Instruction &instruction) {
        auto b = instruction.condition->as_const_bool();
        if (b && !(b->value)) {
            return;
        }

        auto signature = getSignature(instruction);

        Operations::StaticOperands nonQubitOperands =
            getStaticOperands(instruction);

        auto krausOperators = operations.get(instruction.name, signature, nonQubitOperands);

        Operations::checkValidKrausOperatorSet(
            instruction.name, signature.size() - nonQubitOperands.size(),
            krausOperators);

        std::vector<QubitIndex> controlQubits; // FIXME: inlined vector

        if (auto bitref = instruction.condition->as_bit_refs()) {
            
            throw std::runtime_error("Unimplemented");

            for (auto const &b : bitref->index) {
                controlQubits.push_back(QubitIndex{static_cast<std::size_t>(b->value)});
            }
        }

        for (auto ops : getDynamicOperands(instruction)) { // FIXME: create mapping qubit -> bit.
            block.emplace_back(krausOperators, ops, controlQubits);
        }
    }

    std::vector<CircuitInstruction> &block;
    Operations const &operations;
};

Circuit cQasmProgramToCircuit(cq::Program const &program, Operations const &operations) {
    Circuit circuit(program.num_qubits);

    for (auto const& subcircuit: program.subcircuits) {
        std::vector<CircuitInstruction> block;

        GateConvertor gateConvertor(block, operations);
        for (const auto &statement : subcircuit->body->statements) {
            statement->visit(gateConvertor);
        }

        circuit.addLoop(std::move(block), subcircuit->iterations);
    }

    return circuit;
}
} // namespace

CQasmSimulationResult executeCqasmCode(cq::Program const &program,
                          Operations const &operations) {
    
    auto state = cQasmProgramToCircuit(program, operations).execute();
    
    return CQasmSimulationResult{std::move(state), {}};
}

} // namespace  superpositeur