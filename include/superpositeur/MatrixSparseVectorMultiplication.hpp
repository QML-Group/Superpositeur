#include "superpositeur/Matrix.hpp"
#include "superpositeur/Common.hpp"
#include "superpositeur/CircuitInstruction.hpp"

#include <deque>

namespace superpositeur {

template <std::uint64_t MaxNumberOfQubits>
using InputSpan = std::span<KeyValue<MaxNumberOfQubits> const>;

template <std::uint64_t N>
using Q = moodycamel::BlockingConcurrentQueue<KeyValue<N>>;

using Q64 = moodycamel::BlockingConcurrentQueue<KeyValue<64>;
using Q128 = moodycamel::BlockingConcurrentQueue<KeyValue<128>;

template <std::uint64_t N, std::uint64_t M>
class Iterators {
public:
    using Operands = CircuitInstruction::Mask;

    Iterators(Matrix const& m, Q<N>& q, Operands ops) : matrix(m), inputQueue(q), operands(ops.cast<MaxNumberOfQubits>()) {
        assert(matrix.isSquare());
        assert(std::has_single_bit(matrix.getNumberOfRows()));
        assert(matrix.getNumberOfRows() >= 2);

        iterators.reserve(matrix.getNumberOfRows() * matrix.getNumberOfCols());

        for (std::uint64_t i = 0; i < matrix.getNumberOfRows(); ++i) {
            for (std::uint64_t j = 0; j < matrix.getNumberOfCols(); ++j) {
                if (utils::isNotNull(matrix.get(i, j))) {
                    auto it = inputStorage.begin();
                    while (it != inputStorage.end() && it->first.pext(operands) != j) {
                        ++it;
                    }

                    if (it == inputStorage.end() && !std::isnan(inputStorage.back().second)) {
                        do {
                            inputStorage.emplace_back();
                            q.wait_dequeue(inputStorage.back());
                            ++it; // Assigning it could be done after the loop.
                        while (!std::isnan(inputStorage.back().second) && inputStorage.back().pext(operands) != j);
                    }

                    if (it != inputStorage.end()) {
                        iterators.emplace_back(Iterator{
                            .input = BasisVector<MaxNumberOfQubits>().pdep(j, operands),
                            .output = BasisVector<MaxNumberOfQubits>().pdep(i, operands),
                            .rowIndex = i,
                            .colIndex = j,
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
            assert(inputStorage.empty()); // Really? Could also be null matrix?
            return { BasisVector<MaxNumberOfQubits>(), { NAN, NAN } };
        }

        auto topIt = std::ranges::min_element(iterators, {}, &Iterator::resultKet);

        KeyValue<MaxNumberOfQubits> result = { topIt->resultKet, topIt->iterator->second * matrix.get(topIt->rowIndex, topIt->colIndex) };

        do {
            ++topIt->iterator;
        } while (topIt->iterator != inputStorage.end() && (topIt->iterator->first & operands) != topIt->input);

        if (topIt->iterator == inputStorage.end()) {
            while (!std::isnan(inputStorage.back().second) && (inputStorage.back().first & operands) != topIt->input) {
                inputStorage.emplace_back();
                q.wait_dequeue(inputStorage.back());
                ++topIt->iterator; // Assigning it could be done after the loop.
            }
        }

        if (!std::isnan(topIt->iterator->second)) [[likely]] {
            topIt->resultKet = (topIt->iterator->first & (~operands)) | topIt->output;
        } else {
            iterators.erase(topIt);
        }

        auto leftmostIt = std::ranges::min_element(iterators, {}, &Iterator::iterator);
        while (inputStorage.begin() != leftMostIt) {
            inputStorage.pop_front();
        }

        return result;
    }

private:
    using InputStorage = std::deque<KeyValue<N>>;

    struct Iterator {
        BasisVector<MaxNumberOfQubits> input;
        BasisVector<MaxNumberOfQubits> output;
        std::uint64_t rowIndex;
        std::uint64_t colIndex;
        BasisVector<MaxNumberOfQubits> resultKet;
        typename InputStorage::iterator iterator;
    };
    
    Matrix const& matrix;
    Q<N>& inputQueue;
    BasisVector<MaxNumberOfQubits> operands;
    std::vector<Iterator> iterators;
    InputStorage inputStorage;
};

template <std::uint64_t N, M>
inline std::uint64_t multiplyMatrix(Matrix const& matrix, Q<N>& input, CircuitInstruction::Mask const& operands, Q<M>& output) {
    assert(matrix.isSquare());
    assert(std::popcount(matrix.getNumberOfRows()) == 1);
    assert(matrix.getNumberOfRows() >= 2);
    assert(operands.popcount() == static_cast<std::uint64_t>(std::countr_zero(matrix.getNumberOfRows())));

    auto iterators = Iterators<N, M>(matrix, input, operands);

    std::uint64_t hashOfTheKeys = 0;
    
    KeyValue<M> accumulator = { BasisVector<M>(), 0.};

    auto kv = iterators.next();
    while (!std::isnan(kv.second.real())) {
        if (kv.first != accumulator.first) {
            assert(kv.first > accumulator.first);

            if (utils::isNotNull(accumulator.second)) [[likely]] {
                output.enqueue(accumulator);
                hashOfTheKeys += accumulator.first.hash();
            }
            accumulator = kv;
        } else {
            accumulator.second += kv.second;
        }

        kv = iterators.next();
    }

    if (utils::isNotNull(accumulator.second)) {
        output.enqueue(accumulator);
        hashOfTheKeys += accumulator.first.hash();
    }

    return hashOfTheKeys;
}

}