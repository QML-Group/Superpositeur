#include "superpositeur/Common.hpp"
#include <algorithm>
#include <execution>
#include <chrono>
#include <iostream>

namespace superpositeur {

template <std::uint64_t MaxNumberOfQubits>
void removeSortIndexImpl(SparseVector<MaxNumberOfQubits> &data, BasisVector<MaxNumberOfQubits> currentSortIndices, std::uint64_t indexToRemove) {
    assert(currentSortIndices.test(indexToRemove));
    
    assert(std::is_sorted(data.begin(), data.end(), [currentSortIndices](auto const left, auto const right) {
        return (left.ket & currentSortIndices) < (right.ket & currentSortIndices);
    }));

    auto outputSortIndices = currentSortIndices;
    outputSortIndices.set(indexToRemove, false);

    auto mask = (indexToRemove + 1 >= MaxNumberOfQubits) ? BasisVector<MaxNumberOfQubits>() : (((~BasisVector<MaxNumberOfQubits>()) << (indexToRemove + 1)) & currentSortIndices);

    auto it = data.begin();
    while (it != data.end()) {
        auto mid = std::find_if(it, data.end(), [indexToRemove](auto x) { return x.ket.test(indexToRemove); });

        assert((mid->ket & mask) >= (it->ket & mask));

        if (mid == it) {
            ++it;
            continue;
        }

        if ((mid->ket & mask) != (it->ket & mask)) {
            ++it;
            continue;
        }

        auto end = std::find_if(mid, data.end(), [indexToRemove](auto x) { return !x.ket.test(indexToRemove); });

        assert((std::next(end, -1)->ket & mask) >= (mid->ket & mask));

        if ((std::next(end, -1)->ket & mask) == (mid->ket & mask)) {
            assert(std::all_of(it, end, [mask, it](auto x) { return (x.ket & mask) == (it->ket & mask); }));

            std::inplace_merge(it, mid, end, [outputSortIndices](auto left, auto right) { return (left.ket & outputSortIndices) < (right.ket & outputSortIndices); });
        }

        it = end;
    }

    assert(std::is_sorted(data.begin(), data.end(), [outputSortIndices](auto const left, auto const right) {
        return (left.ket & outputSortIndices) < (right.ket & outputSortIndices);
    }));
}


template <std::uint64_t MaxNumberOfQubits>
void removeSortIndices(SparseVector<MaxNumberOfQubits>& data, BasisVector<MaxNumberOfQubits> currentSortIndices, BasisVector<MaxNumberOfQubits> sortIndicesToRemove) {
    assert(std::is_sorted(data.begin(), data.end(), [currentSortIndices](auto const left, auto const right) {
        return (left.ket & currentSortIndices) < (right.ket & currentSortIndices);
    }));

    assert((sortIndicesToRemove & currentSortIndices) == sortIndicesToRemove);

    if (sortIndicesToRemove.empty()) {
        return;
    }

    auto startTime = std::chrono::steady_clock::now();

    auto remainingSortIndicesToRemove = sortIndicesToRemove;
    auto removedSortIndices = BasisVector<MaxNumberOfQubits>();
    assert((remainingSortIndicesToRemove | removedSortIndices) == sortIndicesToRemove);
    while (!remainingSortIndicesToRemove.empty()) {
        auto index = remainingSortIndicesToRemove.countrZero();
        assert(remainingSortIndicesToRemove.test(index));
        removeSortIndexImpl(data, currentSortIndices ^ removedSortIndices, index);

        remainingSortIndicesToRemove.set(index, false);
        removedSortIndices.set(index);
        assert((remainingSortIndicesToRemove | removedSortIndices) == sortIndicesToRemove);
    }
    
    auto endTime = std::chrono::steady_clock::now();

    INSTRU << "removeSortIndices," << data.size() << "," << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() << "," << currentSortIndices << "," << sortIndicesToRemove << std::endl;

    assert(std::is_sorted(data.begin(), data.end(), [targetSortIndices = currentSortIndices ^ sortIndicesToRemove](auto const left, auto const right) {
        return (left.ket & targetSortIndices) < (right.ket & targetSortIndices);
    }));
}

template <std::uint64_t MaxNumberOfQubits>
void addSortIndexImpl(SparseVector<MaxNumberOfQubits> &data, std::uint64_t index, BasisVector<MaxNumberOfQubits> mask) {
    auto it = data.begin();
    while (it != data.end()) {
        auto current = it->ket & mask;
        auto endPartition = std::find_if(it, data.end(), [current, mask](auto x) { return (x.ket & mask) != current; });
        std::stable_partition(it, endPartition, [index](auto x) { return !x.ket.test(index); });
        it = endPartition;
    }
}

template <std::uint64_t MaxNumberOfQubits>
void addSortIndices(SparseVector<MaxNumberOfQubits>& data, BasisVector<MaxNumberOfQubits> currentSortIndices, BasisVector<MaxNumberOfQubits> sortIndicesToAdd) {
    assert(std::is_sorted(data.begin(), data.end(), [currentSortIndices](auto const left, auto const right) {
        return (left.ket & currentSortIndices) < (right.ket & currentSortIndices);
    }));

    assert((currentSortIndices & sortIndicesToAdd).empty());

    if (sortIndicesToAdd.empty()) {
        return;
    }

    auto startTime = std::chrono::steady_clock::now();

    auto remainingSortIndicesToAdd = sortIndicesToAdd;
    while (!remainingSortIndicesToAdd.empty()) {
        auto index = MaxNumberOfQubits - 1 - remainingSortIndicesToAdd.countlZero();
        assert(remainingSortIndicesToAdd.test(index));
        remainingSortIndicesToAdd.set(index, false);

        auto mask = (index + 1 < MaxNumberOfQubits) ? ((~BasisVector<MaxNumberOfQubits>()) << (index + 1)) : BasisVector<MaxNumberOfQubits>();
        mask &= (currentSortIndices | sortIndicesToAdd);

        addSortIndexImpl(data, index, mask);
    }

    auto endTime = std::chrono::steady_clock::now();

    INSTRU << "addSortIndices," << data.size() << "," << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() << "," << currentSortIndices << "," << sortIndicesToAdd << std::endl;

    assert(std::is_sorted(data.begin(), data.end(), [currentSortIndices, sortIndicesToAdd](auto const left, auto const right) {
        return (left.ket & (currentSortIndices | sortIndicesToAdd)) < (right.ket & (currentSortIndices | sortIndicesToAdd));
    }));
}

template <std::uint64_t MaxNumberOfQubits>
void sortSparseVector(SparseVector<MaxNumberOfQubits>& data, BasisVector<MaxNumberOfQubits> currentSortIndices, BasisVector<MaxNumberOfQubits> desiredSortIndices) {
    assert(std::is_sorted(data.begin(), data.end(), [currentSortIndices](auto const left, auto const right) {
        return (left.ket & currentSortIndices) < (right.ket & currentSortIndices);
    }));

    // FIXME: if sortIndicesToRemove.countrZero() is too small, but non-zero, do regular sort instead?

    auto sortIndicesToRemove = currentSortIndices & (~desiredSortIndices);
    removeSortIndices(data, currentSortIndices, sortIndicesToRemove);

    auto commonSortIndices = currentSortIndices & desiredSortIndices;
    auto sortIndicesToAdd = (~currentSortIndices) & desiredSortIndices;
    addSortIndices(data, commonSortIndices, sortIndicesToAdd);

    assert(std::is_sorted(data.begin(), data.end(), [desiredSortIndices](auto const left, auto const right) {
        return (left.ket & desiredSortIndices) < (right.ket & desiredSortIndices);
    }));
}

}