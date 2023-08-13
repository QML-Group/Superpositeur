#include "superpositeur/API.hpp"

#include <iostream>
#include <optional>
#include <vector>
#include <climits>
#include <cassert>
#include <charconv>
#include <unordered_map>
#include <stack>

#include "superpositeur/MixedState.hpp"
#include "superpositeur/StrongTypes.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {

class SuperpositeurCLISession {
public:
    SuperpositeurCLISession() = default;

    void operator()(std::string_view input) {
        auto words = getWords(input);

        if (words.size() <= 0) {
            return;
        }

        auto const& command = words[0];

        if (command == "p" || command == "print") {
            std::vector<QubitIndex> operands;
            
            for (std::uint64_t i = 1; i < words.size(); ++i) {
                auto qubitIndex = getQubit(words[i]);
                
                if (!qubitIndex) {
                    std::cout << "Error: should be 'print #0 #1 #2'" << std::endl;
                    return;
                }

                operands.push_back({*qubitIndex});
            }

            print(std::move(operands));
        } else if (command == "reset") {
            reset();
        } else if (command == "help") {
            help();
        } else if (command == "l" || command == "list") {
            list();
        } else {
            auto const& gateName = command;

            auto it = operationsMap.find(std::string(gateName));
            if (it == operationsMap.end()) {
                std::cout << "Unknown operation '" << gateName << "'" << std::endl;
                return;
            }
            
            auto instruction = it->second(words);

            if (instruction) {
                state(*instruction);
            }
        }
    }

private:
    using Words = std::vector<std::string_view>;
    using MatrixGen = std::function<std::optional<CircuitInstruction>(std::vector<std::string_view> const&)>;
    using OperationsMap = std::unordered_map<std::string, MatrixGen>;

    static OperationsMap createOperationsMap() {
        namespace ops = default_operations;

        OperationsMap result;
        
        auto singleQubit = [&result](std::string name, Matrix const& m) {
            MatrixGen doThisGate = [name, &m](Words const& args) {
                if (args.size() != 2) {
                    std::cout << "Syntax: '" << name << " #0'" << std::endl;
                    return std::optional<CircuitInstruction>();
                }

                auto qubit = getQubit(args[1]);
                if (!qubit) {
                    std::cout << "Syntax: '" << name << " #0'" << std::endl;
                    return std::optional<CircuitInstruction>();
                }

                return std::optional<CircuitInstruction>(CircuitInstruction({m}, {*qubit}));
            };

            result[std::move(name)] = doThisGate;
        };
        
        singleQubit("id", ops::IDENTITY);
        singleQubit("x", ops::X);
        singleQubit("x90", ops::X90);
        singleQubit("mx90", ops::MX90);
        singleQubit("y", ops::Y);
        singleQubit("y90", ops::Y90);
        singleQubit("my90", ops::MY90);
        singleQubit("z", ops::Z);
        singleQubit("z90", ops::Z90);
        singleQubit("mz90", ops::MZ90);
        singleQubit("s", ops::S);
        singleQubit("sdag", ops::SDAG);
        singleQubit("t", ops::T);
        singleQubit("tdag", ops::TDAG);
        singleQubit("h", ops::H);

        using KrausFromFloat = KrausOperators(*)(double);
        auto singleQubitKrausFromFloat = [&result](std::string name, KrausFromFloat fn) {
            MatrixGen doThisGate = [name, fn](Words const& args) {
                if (args.size() != 3) {
                    std::cout << "Syntax: '" << name << " #i 0.12345'" << std::endl;
                    return std::optional<CircuitInstruction>();
                }

                auto qubit = getQubit(args[1]);
                if (qubit) {
                    auto floatOperand = get<double>(args[2]);
                    if (floatOperand) {
                        return std::optional<CircuitInstruction>(CircuitInstruction(fn(*floatOperand), {*qubit}));
                    }
                }
                
                std::cout << "Syntax: '" << name << " #i 0.12345'" << std::endl;
                return std::optional<CircuitInstruction>();
            };

            result[name] = doThisGate;
        };
        
        singleQubitKrausFromFloat("depolarizing_channel", ops::DEPOLARIZING_CHANNEL);
        singleQubitKrausFromFloat("phase_damping", ops::PHASE_DAMPING);
        singleQubitKrausFromFloat("amplitude_damping", ops::AMPLITUDE_DAMPING);

        singleQubitKrausFromFloat("rx", [](double d) { return KrausOperators{ ops::RX(d) }; });
        singleQubitKrausFromFloat("ry", [](double d) { return KrausOperators{ ops::RY(d) }; });
        singleQubitKrausFromFloat("rz", [](double d) { return KrausOperators{ ops::RZ(d) }; });

        // TODO: add 2-qubit operations.

        return result;
    }

    Words getWords(std::string_view input) {
        Words result;

        std::uint64_t pos = 0;
        while (true) {
            while (pos < input.size() && input[pos] == ' ') { ++pos; }
            if (pos >= input.size()) {
                break;
            }

            auto newPos = input.find(' ', pos);
            assert(newPos > pos);

            result.push_back(input.substr(pos, newPos - pos));

            pos = newPos;
        }

        return result;
    }

    template <typename T>
    static std::optional<T> get(std::string_view s) {
        T arg;
        auto parseResult = std::from_chars(s.data(), s.data() + s.size(), arg);
        if (parseResult.ptr != s.data() + s.size() || parseResult.ec == std::errc::invalid_argument || parseResult.ec == std::errc::result_out_of_range) {
            return std::nullopt;
        }
        return arg;
    }

    static std::optional<QubitIndex> getQubit(std::string_view s) {
        if (s[0] != '#') {
            return std::nullopt;
        }

        auto qubitIndex = get<std::uint64_t>(s.substr(1, std::string_view::npos));

        if (!qubitIndex) {
            return std::nullopt;
        }

        return QubitIndex{*qubitIndex};
    }

    void print(std::vector<QubitIndex> &&operands) {
        if (operands.empty()) {
            std::cout << "No qubit" << std::endl;
            return;
        }

        std::vector<bool> mask;
        for (auto op: operands) {
            if (op.value > 128) {
                std::cout << "Wrong qubit index" << std::endl;
                return;
            }

            mask.resize(std::max(mask.size(), op.value + 1), false);
            mask[op.value] = true;
        }
        
        std::cout << std::endl << "Reduced density matrix for qubits ";
        for (auto op: operands) {
            std::cout << op.value << " ";
        }

        std::cout << state.getReducedDensityMatrix(mask) << std::endl;
    }

    void list() const {
        std::cout << "Available quantum operations (call gate without arguments for further help):" << std::endl;
        for (auto const& kv: operationsMap) { // FIXME: multiple columns?
            auto const& gateName = kv.first;
            std::cout << gateName << std::endl;
        }
    }

    void help() const {
        std::cout << "~ Help menu ~" << std::endl;
        std::cout << "help                    show this help" << std::endl;
        std::cout << "p[rint] #1 #2 #3        print state for qubits #1, #2 and #3" << std::endl;
        std::cout << "reset                   reset the quantum state" << std::endl;
        std::cout << "h #0                    apply Hadamard quantum gate on qubit #0" << std::endl;
        std::cout << "l[ist]                  list available quantum gates" << std::endl;
        std::cout << "ctrl-C or ctrl-D        exit this awesome shell" << std::endl;
    };

    void reset() {
        state.reset();
        std::cout << "State is back to |00...0>" <<  std::endl;
    }

    MixedState state;
    OperationsMap const operationsMap = createOperationsMap();
    std::stack<CircuitInstruction> executedInstructions;
};

}

int main() {
    superpositeur::SuperpositeurCLISession session;

    std::string input;
    std::string instruction;
    std::cout << "Enter a command ('help' for help, ctrl-C or ctrl-D to exit)\n>>> ";
    while (std::getline(std::cin, input)) {
        std::stringstream lineStream(input);
        while(std::getline(lineStream, instruction, ';')){
            session(instruction);
        }
        std::cout << ">>> ";
    }
    std::cout << std::endl;

    return 0;
}