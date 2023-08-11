#include "superpositeur/MixedState.hpp"

namespace superpositeur {

bool MixedState::isConsistent() const {
    auto dataSize = std::visit([](auto &&data) { return data.size(); }, dataVariant);

    auto sizesSum = std::reduce(sizes.begin(), sizes.end());

    if (dataSize == 0 || sizesSum != dataSize) {
        return false;
    }

    if (hashes.size() != sizes.size()) {
        return false;
    }

    if (std::ranges::any_of(sizes, [](auto x) { return x == 0; })) {
        return false;
    }

    return std::visit([](auto&& data) {
        if (data.empty()) {
            return false;
        }

        double accumulator = 0.;
        for (auto const &[key, factor] : data) {
            accumulator += std::norm(factor);
        }
        return utils::isNull(accumulator - 1.);
    }, dataVariant);
}

} // namespace  superpositeur