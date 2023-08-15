#include "superpositeur/API.hpp"

#include <iostream>
#include <optional>
#include <vector>
#include <climits>
#include <cassert>
#include <charconv>
#include <unordered_map>
#include <stack>
#include <functional>
#include <algorithm>
#include <expected>

#include "superpositeur/MixedState.hpp"
#include "superpositeur/StrongTypes.hpp"
#include "superpositeur/DefaultOperations.hpp"

#ifdef SUPERPOSITEUR_WASM
#include <emscripten/bind.h>
#endif

namespace superpositeur {
namespace cli {

class SuperpositeurCLISession {
public:
    SuperpositeurCLISession() = default;

    std::string operator()(std::string const& input) {
        auto words = getWords(input);

        if (words.size() <= 0) {
            return "";
        }

        auto const& command = words[0];

        if (command == "p" || command == "print") {
            std::vector<QubitIndex> operands;
            
            for (std::uint64_t i = 1; i < words.size(); ++i) {
                auto qubitIndex = getQubit(words[i]);
                
                if (!qubitIndex) {
                    return "Error: should be 'print #0 #1 #2'";
                }

                operands.push_back({*qubitIndex});
            }

            return print(std::move(operands));
        } else if (command == "reset") {
            return reset();
        } else if (command == "help") {
            return help();
        } else if (command == "l" || command == "list") {
            return list();
        } else {
            auto const& gateName = command;

            auto it = operationsMap.find(std::string(gateName));
            if (it == operationsMap.end()) {
                return std::string("Unknown operation '") + std::string(gateName) + "'";
            }
            
            auto operation = it->second(words);

            if (!operation) {
                return operation.error();
            }
            state(*operation);
            return "";
        }
    }

private:
    using Words = std::vector<std::string_view>;
    using MatrixGen = std::function<std::expected<CircuitInstruction, std::string>(std::vector<std::string_view> const&)>;
    using OperationsMap = std::unordered_map<std::string, MatrixGen>;

    static OperationsMap createOperationsMap() {
        namespace ops = default_operations;

        OperationsMap result;
        
        auto singleQubit = [&result](std::string name, Matrix const& m) {
            MatrixGen doThisGate = [name, &m](Words const& args) -> std::expected<CircuitInstruction, std::string> {
                if (args.size() != 2) {
                    return std::unexpected(std::string("Syntax: '") + name + " #0'");
                }

                auto qubit = getQubit(args[1]);
                if (!qubit) {
                    return std::unexpected(std::string("Syntax: '") + name + " #0'");
                }

                return CircuitInstruction({m}, {*qubit});
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
            MatrixGen doThisGate = [name, fn](Words const& args) -> std::expected<CircuitInstruction, std::string> {
                if (args.size() != 3) {
                    return std::unexpected(std::string("Syntax: '") + name + " #0 0.12345'");
                }

                auto qubit = getQubit(args[1]);
                if (qubit) {
                    // auto floatOperand = get<double>(args[2]); // FIXME: from_chars with double not working with emscripten
                    auto floatOperand = getDouble(args[2]);
                    if (floatOperand) {
                        return CircuitInstruction(fn(*floatOperand), {*qubit});
                    }
                }
                
                return std::unexpected(std::string("Syntax: '") + name + " #0 0.12345'");
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

    static std::optional<double> getDouble(std::string_view s) {
        try {
            double value = std::stod(std::string(s));
            return value;
        } catch (std::exception const& e) {
            return std::nullopt;
        } 
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

    std::string print(std::vector<QubitIndex> &&operands) {
        if (operands.empty()) {
            return "No qubit";
        }

        std::vector<bool> mask;
        for (auto op: operands) {
            if (op.value > 128) {
                return "Wrong qubit index";
            }

            mask.resize(std::max(static_cast<std::uint64_t>(mask.size()), op.value + 1), false);
            mask[op.value] = true;
        }
        
        std::stringstream s;

        s << "Reduced density matrix for qubits ";
        for (auto op: operands) {
            s << op.value << " ";
        }

        s << state.getReducedDensityMatrix(mask);

        return s.str();
    }

    std::string list() const {
        std::stringstream s;
        s << "Available quantum operations (call gate without arguments for further help):" << std::endl;
        bool first = true;
        for (auto const& kv: operationsMap) { // FIXME: multiple columns?
            if (!first) {
                s << ", ";
            } else {
                first = false;
            }
            auto const& gateName = kv.first;
            s << gateName;
        }
        return s.str();
    }

    std::string help() const {
        std::stringstream s;
        s << "help                    show this help" << std::endl;
        s << "p[rint] #1 #2 #3        print state for qubits #1, #2 and #3" << std::endl;
        s << "reset                   reset the quantum state" << std::endl;
        s << "h #0                    apply Hadamard quantum gate on qubit #0" << std::endl;
        s << "l[ist]                  list available quantum gates";
        return s.str();
    };

    std::string reset() {
        state.reset();
        return "State is back to |00...0>";
    }

    MixedState state;
    OperationsMap const operationsMap = createOperationsMap();
    std::stack<CircuitInstruction> executedInstructions;
};

}
}

#ifdef SUPERPOSITEUR_WASM
EMSCRIPTEN_BINDINGS(superpositeur_cli_session) {
  emscripten::class_<superpositeur::cli::SuperpositeurCLISession>("SuperpositeurCLISession")
    .constructor<>()
    .function("input", &superpositeur::cli::SuperpositeurCLISession::operator())
    ;
}
#endif
