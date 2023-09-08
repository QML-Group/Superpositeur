#include "superpositeur/Common.hpp"
#include "oneapi/tbb/parallel_invoke.h"
#include <algorithm>

namespace superpositeur {

template <std::uint64_t MaxNumberOfQubits>
void sortSparseVector(SparseVector<MaxNumberOfQubits>& data, BasisVector<MaxNumberOfQubits> currentSortIndices, BasisVector<MaxNumberOfQubits> desiredSortIndices) {
    assert(std::is_sorted(data.begin(), data.end(), [currentSortIndices](auto const left, auto const right) {
        return (left.ket & currentSortIndices) < (right.ket & currentSortIndices);
    }));
    
    auto commonSortIndices = currentSortIndices & desiredSortIndices;
    auto sortIndicesToRemove = currentSortIndices & (~desiredSortIndices);
    auto sortIndicesToAdd = (~currentSortIndices) & desiredSortIndices;

    // FIXME: if sortIndicesToRemove.countrZero() is too small, do regular sort instead?

    if (!sortIndicesToRemove.empty()) {
        using It = SparseVector<MaxNumberOfQubits>::iterator;
        std::function<void(It, It)> removeIndices = [&removeIndices, sortIndicesToRemove, commonSortIndices](It begin, It end) {
            auto dist = std::distance(begin, end);
            
#ifdef NDEBUG
            constexpr std::uint64_t SEQUENTIAL_LIMIT = 2;
#else
            constexpr std::uint64_t SEQUENTIAL_LIMIT = 500;
#endif
            if (dist < SEQUENTIAL_LIMIT) {
                std::sort(begin, end, [commonSortIndices](auto left, auto right) { return (left.ket & commonSortIndices) < (right.ket & commonSortIndices); });
                return;
            }

            auto mid = std::next(begin, dist / 2);
            auto current = (mid->ket & sortIndicesToRemove);
            auto other = std::find_if(mid, end, [current, sortIndicesToRemove](auto x) { return (x.ket & sortIndicesToRemove) > current; });
            if (other == end) {
                // Normally this would use a reverse iterator.
                other = mid;
                do {
                    --other;
                    if ((other->ket & sortIndicesToRemove) < current) {
                        break;
                    }
                } while (other != begin);

                if ((other->ket & sortIndicesToRemove) >= current) {
                    return;
                }

                ++other;
            }

            oneapi::tbb::parallel_invoke(
                [&removeIndices, begin, other]{ removeIndices(begin, other); },
                [&removeIndices, other, end]{ removeIndices(other, end); }
            );
            
            std::inplace_merge(begin, other, end, [commonSortIndices](auto left, auto right) { return (left.ket & commonSortIndices) < (right.ket & commonSortIndices); });
        };

        removeIndices(data.begin(), data.end());
    }


    // using It = SparseVector<MaxNumberOfQubits>::iterator;


    // std::vector<It> breakPoints;
    // breakPoints.push_back(data.begin());
    // auto it = data.begin();
    // while (it != data.end()) {
    //     auto adjacent = std::adjacent_find(it, data.end(), [commonSortIndices](auto left, auto right) { return (left.ket & commonSortIndices) > (right.ket & commonSortIndices); });
    //     if (adjacent == data.end()) {
    //         break;
    //     }
    //     ++adjacent;
    //     breakPoints.push_back(adjacent);
    //     it = adjacent;
    // }
    // breakPoints.push_back(data.end());
    
    // using ItIt = std::vector<It>::iterator;
    // std::function<void(ItIt, ItIt)> process = [&process, commonSortIndices](ItIt begin, ItIt end) {
    //     auto dist = std::distance(begin, end);

    //     if (dist < 2) {
    //         return;
    //     }

    //     auto mid = std::next(begin, dist / 2);

    //     process(begin, mid);
    //     process(mid, end);
        
    //     std::inplace_merge(*begin, *mid, *end, [commonSortIndices](auto left, auto right) { return (left.ket & commonSortIndices) < (right.ket & commonSortIndices); });
    // };

    // process(breakPoints.begin(), std::next(breakPoints.end(), -1));

    // std::vector<It> breakPoints;
    // breakPoints.push_back(data.begin());
    // auto it = data.begin();
    // while (it != data.end()) {
    //     auto adjacent = std::adjacent_find(it, data.end(), [commonSortIndices](auto left, auto right) { return (left.ket & commonSortIndices) > (right.ket & commonSortIndices); });
    //     if (adjacent == data.end()) {
    //         break;
    //     }
    //     ++adjacent;
    //     breakPoints.push_back(adjacent);
    //     it = adjacent;
    // }
    // breakPoints.push_back(data.end());
    
    // using ItIt = std::vector<It>::iterator;
    // std::function<void(ItIt, ItIt)> process = [&process, commonSortIndices](ItIt begin, ItIt end) {
    //     auto dist = std::distance(begin, end);

    //     if (dist < 2) {
    //         return;
    //     }

    //     auto mid = std::next(begin, dist / 2);

    //     process(begin, mid);
    //     process(mid, end);
        
    //     std::inplace_merge(*begin, *mid, *end, [commonSortIndices](auto left, auto right) { return (left.ket & commonSortIndices) < (right.ket & commonSortIndices); });
    // };

    // process(breakPoints.begin(), std::next(breakPoints.end(), -1));

    while (!sortIndicesToAdd.empty()) {
        auto index = MaxNumberOfQubits - 1 - sortIndicesToAdd.countlZero();
        assert(sortIndicesToAdd.test(index));
        sortIndicesToAdd.set(index, false);

        auto mask = (index + 1 < MaxNumberOfQubits) ? ((~BasisVector<MaxNumberOfQubits>()) << (index + 1)) : BasisVector<MaxNumberOfQubits>();
        mask &= desiredSortIndices;

        auto it = data.begin();
        while (it != data.end()) {
            auto current = it->ket & mask;
            auto end = std::find_if(it, data.end(), [current, mask](auto x) { return (x.ket & mask) != current; });
            std::stable_partition(it, end, [index](auto x) { return !x.ket.test(index); }); // Slight optimization possible: if indices are contiguous, this gives the following end?
            it = end;
        }
    }

    assert(std::is_sorted(data.begin(), data.end(), [desiredSortIndices](auto const left, auto const right) {
        return (left.ket & desiredSortIndices) < (right.ket & desiredSortIndices);
    }));
}

}