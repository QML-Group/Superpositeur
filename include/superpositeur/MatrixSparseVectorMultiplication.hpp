#include "superpositeur/Matrix.hpp"
#include "superpositeur/Common.hpp"
#include "superpositeur/CircuitInstruction.hpp"

namespace superpositeur {

enum class EraseInput { No, Yes };

template <EraseInput eraseInput, std::uint64_t MaxNumberOfQubits>
class Iterators {
public:
    using Operands = CircuitInstruction::Mask;

    Iterators(Matrix const& m, SparseVector<MaxNumberOfQubits>& q, SparseVector<MaxNumberOfQubits>::iterator e, Operands ops) : matrix(m), queue(q), end(e), operands(ops.cast<MaxNumberOfQubits>()) {
        assert(matrix.isSquare());
        assert(std::has_single_bit(matrix.getNumberOfRows()));
        assert(matrix.getNumberOfRows() >= 2);

        iterators.reserve(matrix.getNumberOfRows() * matrix.getNumberOfCols());

        for (std::uint64_t i = 0; i < matrix.getNumberOfRows(); ++i) {
            for (std::uint64_t j = 0; j < matrix.getNumberOfCols(); ++j) {
                if (utils::isNotNull(matrix.get(i, j))) {
                    auto it = queue.begin();
                    while (it != end && it->first.pext(operands) != j) {
                        ++it;
                    }

                    if (it != end) {
                        iterators.emplace_back(Iterator{
                            .input = BasisVector<MaxNumberOfQubits>().pdep(j, operands),
                            .output = BasisVector<MaxNumberOfQubits>().pdep(i, operands),
                            .coeff = matrix.get(i, j),
                            .resultKet = it->first.pdep(i, operands),
                            .iterator = it}
                        );
                    }
                }
            }
        }
    }

    KeyValue<MaxNumberOfQubits> next() {
        if (iterators.empty()) [[unlikely]] {
            return { BasisVector<MaxNumberOfQubits>(), { NAN, NAN } };
        }

        auto topIt = std::ranges::min_element(iterators, {}, &Iterator::resultKet);

        KeyValue<MaxNumberOfQubits> result = { topIt->resultKet, topIt->iterator->second * topIt->coeff };

        topIt->iterator = std::find_if(++topIt->iterator, end, [&](auto kv) { return (kv.first & operands) == topIt->input; });

        if (topIt->iterator != end) [[likely]] {
            topIt->resultKet = (topIt->iterator->first & (~operands)) | topIt->output;
        } else {
            iterators.erase(topIt);
        }
        
        if constexpr (eraseInput == EraseInput::Yes) {
            if (!iterators.empty()) [[likely]] {
                auto firstIt = std::ranges::min_element(iterators, {}, &Iterator::iterator);
                queue.erase(queue.begin(), firstIt->iterator);
            }
        }

        return result;
    }

private:
    struct Iterator {
        BasisVector<MaxNumberOfQubits> input;
        BasisVector<MaxNumberOfQubits> output;
        std::complex<double> coeff;
        BasisVector<MaxNumberOfQubits> resultKet;
        typename SparseVector<MaxNumberOfQubits>::iterator iterator;
    };
    
    Matrix const& matrix;
    SparseVector<MaxNumberOfQubits>& queue;
    typename SparseVector<MaxNumberOfQubits>::iterator const end;
    BasisVector<MaxNumberOfQubits> operands;
    std::vector<Iterator> iterators;
};

template <EraseInput eraseInput, std::uint64_t MaxNumberOfQubits>
inline std::uint64_t multiplyMatrix(Matrix const& matrix, SparseVector<MaxNumberOfQubits>& queue, typename SparseVector<MaxNumberOfQubits>::iterator end, CircuitInstruction::Mask const& operands, std::back_insert_iterator<SparseVector<MaxNumberOfQubits>> inserter) {
    assert(matrix.isSquare());
    assert(std::popcount(matrix.getNumberOfRows()) == 1);
    assert(matrix.getNumberOfRows() >= 2);
    assert(operands.popcount() == static_cast<std::uint64_t>(std::countr_zero(matrix.getNumberOfRows())));

    auto iterators = Iterators<eraseInput, MaxNumberOfQubits>(matrix, queue, end, operands);

    std::uint64_t hashOfTheKeys = 0;
    
    KeyValue<MaxNumberOfQubits> accumulator = { BasisVector<MaxNumberOfQubits>(), 0.};

    auto kv = iterators.next();
    while (!std::isnan(kv.second.real())) {
        if (kv.first != accumulator.first) {
            assert(kv.first > accumulator.first);

            if (utils::isNotNull(accumulator.second)) [[likely]] {
                inserter = accumulator;
                hashOfTheKeys += accumulator.first.hash();
            }
            accumulator = kv;
        } else {
            accumulator.second += kv.second;
        }

        kv = iterators.next();
    }

    if (utils::isNotNull(accumulator.second)) {
        inserter = accumulator;
        hashOfTheKeys += accumulator.first.hash();
    }

    if constexpr (eraseInput == EraseInput::Yes) {
        queue.erase(queue.begin(), end);
    }

    return hashOfTheKeys;
}

}