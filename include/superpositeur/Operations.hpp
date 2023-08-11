#pragma once

#include "superpositeur/Matrix.hpp"
#include <variant>
#include <fmt/format.h>

namespace superpositeur {
enum class OperandType { Double, Int, Qubit };

inline auto format_as(OperandType operandType) {
    switch (operandType) {
        case OperandType::Qubit: return "Qubit";
        case OperandType::Double: return "Double";
        case OperandType::Int: return "Int";
    }
    return "";
}

class Operations {
public:
    using Signature =
        std::vector<OperandType>;
    using KrausOperators =
        std::vector<Matrix>;
    using StaticOperand = std::variant<double, std::int64_t>;
    using StaticOperands = std::vector<StaticOperand>;
    using KrausOperatorsGenerationFunction =
        std::function<KrausOperators(StaticOperands const &)>;

    static std::string operandTypeToString(OperandType const &ot) {
        return fmt::format("{}", ot);
    }

    static std::string signatureToString(Signature const &signature) {
        return fmt::format("({})", fmt::join(signature, ", "));
    }

    static void
    checkValidKrausOperatorSet(std::string const &name,
                               std::int64_t numberOfDynamicOperands,
                               KrausOperators const &krausOperators) {
        if (numberOfDynamicOperands <= 0) {
            throw std::runtime_error(
                std::string(
                    "Invalid number of dynamic operands for operation ") +
                name);
        }

        if (krausOperators.empty()) {
            throw std::runtime_error(
                std::string("Kraus operators set for operation ") + name +
                " is empty");
        }

        // All Kraus operator matrices need to have the same size.
        for (auto const &m : krausOperators) {
            if (!m.isSquare()) {
                throw std::runtime_error(
                    std::string("Kraus operators for operation ") + name +
                    " are not all square matrices");
            }

            if (m.getNumberOfRows() != krausOperators.begin()->getNumberOfRows()) {
                throw std::runtime_error(
                    std::string("Kraus operators for operation ") + name +
                    " don't all have the same size");
            }
        }

        // Size of Kraus operators need to be a power of two.
        if (!std::has_single_bit(krausOperators.begin()->getNumberOfRows())) {
            throw std::runtime_error(
                std::string("Kraus operators size for operation ") + name +
                " is not a power of two");
        }

        if (static_cast<std::int64_t>(
                std::countr_zero(krausOperators.begin()->getNumberOfRows())) !=
            numberOfDynamicOperands) {
            throw std::runtime_error(
                std::string("Kraus operators for operation ") + name +
                " have size " +
                std::to_string(krausOperators.begin()->getNumberOfRows()) +
                ", which do not match the number of operands (" +
                std::to_string(numberOfDynamicOperands) + ")");
        }

        // Kraus operators need to satisfy the completeness relation.
        // That is: all the eigenvalues of the sum of E_k^t * E_k need to <= 1.
        Matrix accumulator(krausOperators.begin()->getNumberOfRows());
        for (auto const &m : krausOperators) {
            accumulator += m.dagger() * m;
        }

        double spectralRadius = computeSpectralRadius(accumulator);

        if (spectralRadius > 1. + config::ATOL) {
            throw std::runtime_error(
                std::string("Kraus operators for operation ") + name +
                " are not non-trace-increasing");
        }
    }

    KrausOperators get(std::string const &name, Signature signature,
                       StaticOperands const &staticOperands) const {
#ifndef NDEBUG
        assert(signature.size() >= staticOperands.size());
        auto signatureIt = signature.begin();
        auto operandIt = staticOperands.begin();

        while (signatureIt != signature.end()) {
            if (*signatureIt == OperandType::Qubit) {
                ++signatureIt;
                continue;
            }

            assert((operandIt != staticOperands.end()) &&
                   ((*signatureIt == OperandType::Double &&
                     std::holds_alternative<double>(*operandIt)) ||
                    (*signatureIt == OperandType::Int &&
                     std::holds_alternative<std::int64_t>(*operandIt))));

            ++operandIt;
            ++signatureIt;
        }
#endif

        auto it = krausOperatorsGeneration.find(name);

        if (it == krausOperatorsGeneration.end()) {
            throw std::runtime_error("No registered operation with name " +
                                     name);
        }

        if (it->second.first != signature) {
            throw std::runtime_error(
                "Requested operation " + name + " with signature " +
                Operations::signatureToString(signature) +
                ", but the registered operation with that name has signature " +
                Operations::signatureToString(it->second.first));
        }

        auto result = it->second.second(staticOperands);

        return result;
    }

    void add(std::string const &name, Signature signature, Matrix const &m) {
        add(name, std::move(signature), KrausOperators{m});
    }

    void add(std::string const &name, Signature signature,
             KrausOperators const &ks) {
        checkValidKrausOperatorSet(name, signature.size(), ks);

        assert(signature.size() == static_cast<std::uint64_t>(std::countr_zero(
                                       ks.begin()->getNumberOfRows())));
        assert(std::ranges::all_of(signature, [](auto x) {
            return x == OperandType::Qubit;
        }));

        add(name, signature,
            [ks](Operations::StaticOperands const &staticOperands)
                -> KrausOperators {
                assert(staticOperands.empty());
                (void) staticOperands;
                return ks;
            });
    }

    using SingleFloatOperand =
        KrausOperators (*)(double); // Here return type templated on # of
                                    // qubit/bit operands would be beneficial...
    void add(std::string const &name, Signature signature,
             SingleFloatOperand f) {
        assert(std::ranges::count_if(signature, [](auto x) { return x == OperandType::Double; }) == 1);

        add(name, signature,
            [f, signature](Operations::StaticOperands const &staticOperands)
                -> KrausOperators {
                assert(staticOperands.size() == 1 &&
                       std::holds_alternative<double>(staticOperands[0]));

                auto res = f(std::get<double>(staticOperands[0]));

                return res;
            });
    }

    using SingleIntOperand =
        KrausOperators (*)(std::int64_t); // Here return type templated on # of
                                    // qubit/bit operands would be beneficial...
    void add(std::string const &name, Signature signature,
             SingleIntOperand f) {
        assert(std::ranges::count_if(signature, [](auto x) { return x == OperandType::Int; }) == 1);

        add(name, signature,
            [f, signature](Operations::StaticOperands const &staticOperands)
                -> KrausOperators {
                assert(staticOperands.size() == 1 &&
                       std::holds_alternative<std::int64_t>(staticOperands[0]));

                auto res = f(std::get<std::int64_t>(staticOperands[0]));

                return res;
            });
    }

    void add(std::string const &name, Signature signature,
             KrausOperatorsGenerationFunction f) {
        UnderlyingT::value_type keyValue = {name, std::make_pair(signature, std::move(f))};
        auto inserted =
            krausOperatorsGeneration
                .insert(std::move(keyValue));

        if (!inserted.second) {
            throw std::runtime_error("Registering operation with name " + name +
                                     " twice");
        }
    }

private:
    using UnderlyingT = std::unordered_map<std::string,
                        std::pair<Signature, KrausOperatorsGenerationFunction>>;

    UnderlyingT krausOperatorsGeneration;
};

} // namespace  superpositeur