#include "superpositeur/Matrix.hpp"
#include "superpositeur/Common.hpp"
#include "superpositeur/CircuitInstruction.hpp"

namespace superpositeur {

template <std::uint64_t MaxNumberOfQubits>
using InputSpan = std::span<KeyValue<MaxNumberOfQubits> const>;

template <std::uint64_t MaxNumberOfQubits>
class Iterators {
public:
    using Input = InputSpan<MaxNumberOfQubits>;
    using Operands = CircuitInstruction::Mask;

    Iterators(Matrix const& m, Input const& s, Operands ops) : matrix(m), span(s), operands(ops) {
        assert(matrix.isSquare());
        assert(std::popcount(matrix.getNumberOfRows()) == 1);
        assert(matrix.getNumberOfRows() >= 2);

        bool hasIdentity = false;
        for (std::uint64_t i = 0; i < matrix.getNumberOfRows(); ++i) {
            for (std::uint64_t j = 0; j < matrix.getNumberOfCols(); ++j) {
                if (i == j) {
                    hasIdentity |= utils::isNotNull(matrix.get(i, j));
                    continue; // Only one identity iterator.
                }

                if (utils::isNotNull(matrix.get(i, j))) {
                    auto it = span.begin();
                    while (it != span.end() && it->first.pext(operands) != j) {
                        ++it;
                    }

                    if (it != span.end()) {
                        terms.push_back(SumTerm{it->first.pdep(i, operands), it});
                    }
                }
            }
        }
        
        if (hasIdentity && !span.empty()) {
            terms.push_back({span.begin()->first, span.begin()});
        }

        std::ranges::make_heap(terms, [this](auto const& left, auto const& right) { return this->ltSumTerms(left, right); });
    }

    std::optional<KeyValue<MaxNumberOfQubits>> next() {
        if (terms.empty()) {
            return {};
        }
        
        auto const comp = [this](auto const& left, auto const& right) { return this->ltSumTerms(left, right); };

        assert(std::ranges::is_heap(terms, comp));

        std::ranges::pop_heap(terms, comp);
        
        assert(terms.back().iterator != span.end());

        auto input = terms.back().iterator->first.pext(operands);
        auto output = terms.back().resultKet.pext(operands);

        KeyValue<MaxNumberOfQubits> result = {terms.back().resultKet, terms.back().iterator->second * matrix.get(output, input)};

        if (input == output) {
            ++terms.back().iterator;
        } else {
            do {
                ++terms.back().iterator; // FIXME: use lower_bound and nextWithBits.
            } while (terms.back().iterator != span.end() && terms.back().iterator->first.pext(operands) != input);
        }

        if (terms.back().iterator == span.end()) {
            terms.pop_back();
        } else {
            terms.back().resultKet = (input == output) ? terms.back().iterator->first : terms.back().iterator->first.pdep(output, operands);
            std::ranges::push_heap(terms, comp);
        }

        return result;
    }

private:
    struct SumTerm {
        BasisVector<MaxNumberOfQubits> resultKet;
        typename Input::iterator iterator;
    };

    bool ltSumTerms(auto const& left, auto const& right) {
        assert(left.iterator != span.end() && right.iterator != span.end());

        return left.resultKet > right.resultKet; // GT because we want a min-heap.
    }

    Matrix const& matrix;
    Input const& span;
    Operands operands;
    std::vector<SumTerm> terms; // FIXME: inlined vector.
};

struct AddedElementsAndHash {
    std::uint64_t numberOfAddedElements = 0;
    std::uint64_t hashOfTheKeys = 0; // This is used as a hash to compare the keys of two association lists.
};

template <std::uint64_t MaxNumberOfQubits>
inline AddedElementsAndHash multiplyMatrix(Matrix matrix, InputSpan<MaxNumberOfQubits> input, CircuitInstruction::Mask operands, std::back_insert_iterator<SparseVector<MaxNumberOfQubits>> inserter) {
    assert(matrix.isSquare());
    assert(std::popcount(matrix.getNumberOfRows()) == 1);
    assert(matrix.getNumberOfRows() >= 2);
    assert(operands.popcount() == static_cast<std::uint64_t>(std::countr_zero(matrix.getNumberOfRows())));

    auto iterators = Iterators<MaxNumberOfQubits>(matrix, input, operands);

    std::uint64_t numberOfAddedElements = 0;
    std::uint64_t hashOfTheKeys = 0;
    
    std::optional<KeyValue<MaxNumberOfQubits>> accumulator;
    while (auto kv = iterators.next()) {
        if (!accumulator) {
            accumulator = kv;
        } else if (kv->first != accumulator->first) {
            assert(kv->first > accumulator->first);

            if (utils::isNotNull(accumulator->second)) {
                inserter = *accumulator;
                hashOfTheKeys += accumulator->first.hash(); // FIXME: hashing is only required when the output is a mixed state.
                ++numberOfAddedElements;
            }
            accumulator = kv;
        } else {
            accumulator->second += kv->second;
        }
    }

    if (accumulator && utils::isNotNull(accumulator->second)) {
        inserter = *accumulator;
        hashOfTheKeys += accumulator->first.hash(); // FIXME: hashing is only required when the output is a mixed state.
        ++numberOfAddedElements;
    }

    return {numberOfAddedElements, hashOfTheKeys};
}

}