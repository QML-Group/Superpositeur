#pragma once

#include "superpositeur/Operations.hpp"
#include "superpositeur/Circuit.hpp"
#include "superpositeur/utils/FloatComparison.hpp"
#include <memory>
#include <map>
#include <iomanip>

namespace cqasm::v1x::semantic {
class Program;
}

namespace superpositeur {

class CQasmSimulationResult {
public:
    CQasmSimulationResult(MixedState &&s, std::vector<bool> &&b) :
       stateWithClassicalBits(s), classicalBits(b) {
        assert(classicalBits.size() <= stateWithClassicalBits.getNumberOfQubits()); // FIXME: actually only most significant 1.
    }

    Matrix getDensityMatrix() {
        std::vector<bool> mask(numberOfSimulatedClassicalBits(), true);
        return stateWithClassicalBits.getReducedDensityMatrix(mask);
    }

    std::vector<std::pair<std::string, double>> getMeasurementRegisterStatistics() {
        if (!densityMatrixOfClassicalBits) {
            // FIXME: add diagonal-only retrieval of reduced density matrix.
            std::vector<bool> mask(numberOfSimulatedClassicalBits(), false);
            mask.resize(stateWithClassicalBits.getNumberOfQubits(), true);
            densityMatrixOfClassicalBits = std::make_unique<Matrix>(stateWithClassicalBits.getReducedDensityMatrix(mask));
        }

        std::vector<std::pair<std::string, double>> result;
        assert(densityMatrixOfClassicalBits);
        assert(densityMatrixOfClassicalBits->isSquare());
        for (std::uint64_t i = 0; i < densityMatrixOfClassicalBits->getNumberOfRows(); ++i) {
            if (utils::isNotNull(densityMatrixOfClassicalBits->get(i, i))) {
                std::string bitString('0', numberOfSimulatedClassicalBits());

                for (std::uint64_t i = 0; i < classicalBits.size(); ++i) {
                    if (classicalBits[i]) {
                        bitString[bitString.size() - i] = '1';
                    }
                }
                
                assert(utils::isNull(densityMatrixOfClassicalBits->get(i, i).imag()));
                result.emplace_back(bitString, densityMatrixOfClassicalBits->get(i, i).real());
            }
        }

        return result;
    }

private:
    std::uint64_t numberOfSimulatedClassicalBits() {
        return stateWithClassicalBits.getNumberOfQubits() - std::ranges::count_if(classicalBits, [](auto x) { return x; });
    }

    MixedState stateWithClassicalBits;
    std::vector<bool> classicalBits;
    std::unique_ptr<Matrix> densityMatrixOfClassicalBits;
};

CQasmSimulationResult executeCqasmCode(cqasm::v1x::semantic::Program const &program,
                                        Operations const &operations);

} // namespace  superpositeur