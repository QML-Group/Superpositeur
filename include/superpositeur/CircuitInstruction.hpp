#pragma once

#include <bit>
#include <cmath>
#include <span>

#include "superpositeur/Common.hpp"
#include "superpositeur/config/CompileTimeConfiguration.hpp"
#include "superpositeur/Matrix.hpp"
#include "superpositeur/Operations.hpp"

namespace superpositeur {

class CircuitInstruction {
public:
    using KrausOperators = Operations::KrausOperators;
    using QubitIndexVector = std::vector<QubitIndex>;

    static std::uint64_t constexpr MaxNumberOfQubits = 128;
    using Mask = utils::BitSet<MaxNumberOfQubits>;

    CircuitInstruction(KrausOperators const& inputKrausOperators, QubitIndexVector operands,
                       QubitIndexVector controlQubits = {}) {
        // FIXME: check intersection of control qubits and operands.

        assert(operands.size() <= MaxNumberOfQubits);

#ifndef NDEBUG
        Operations::checkValidKrausOperatorSet(
            "unknown", operands.size(), inputKrausOperators);
#endif

        for (auto op: operands) {
            assert(op.value < MaxNumberOfQubits);
            operandsMask.set(op.value);
        }

        // Find permutation to apply to the Kraus operators.
        std::vector<QubitIndex> perm(operands.size(), QubitIndex{0});
        std::iota(perm.begin(), perm.end(), QubitIndex{0});
        std::ranges::sort(perm, [&](auto left, auto right) {
            return operands[left.value] < operands[right.value];
        });

        for (auto const& krausOperator: inputKrausOperators) {
            krausOperators.push_back(applyOperands(krausOperator, perm));
        }

        for (auto q: controlQubits) {
            assert(q.value < MaxNumberOfQubits);
            controlQubitsMask.set(q.value);
        }
    }

    std::uint64_t getNumberOfKrausOperators() const {
        return krausOperators.size();
    }

    KrausOperators const& getKrausOperators() const {
        return krausOperators;
    }
    
    Mask getOperandsMask() const {
        return operandsMask;
    }

    Mask getControlQubitsMask() const {
        return controlQubitsMask;
    }

private:
    KrausOperators krausOperators;
    Mask operandsMask;
    Mask controlQubitsMask;
};

} // namespace  superpositeur