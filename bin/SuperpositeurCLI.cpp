#include "superpositeur/API.hpp"

#include <iostream>

#include "superpositeur/LibqasmInterface.hpp"
#include "superpositeur/Version.hpp"

namespace {

struct CLIOptions {
    std::string filePath;
    std::optional<std::vector<bool>> reductionQubits = std::nullopt;
};

std::optional<CLIOptions> parseCLIOptions(int argc, char** argv) {
    if (argc < 2) {
        return std::nullopt;
    }

    auto filePath = std::string(argv[1]);

    if (argc == 2) {
        return {{ .filePath = std::string(argv[1]), .reductionQubits = std::nullopt }};
    }

    std::vector<bool> reductionQubits;

    for (auto i = 2; i < argc; ++i) {
        try {
            auto qubitIndex = std::stoll(std::string(argv[i]));
            if (qubitIndex < 0 || qubitIndex >= 1000) { // FIXME
                return std::nullopt;
            }

            reductionQubits.resize(std::max(static_cast<std::uint64_t>(qubitIndex) + 1, reductionQubits.size()), false);

            reductionQubits[qubitIndex] = true;
        } catch (std::exception const& e) {
            return std::nullopt;
        }
    }

    return {{ .filePath = std::string(argv[1]), .reductionQubits = reductionQubits }};
}

void printUsage(char** argv) {
    std::cerr << "Usage: \n   " << argv[0] << " <file>\n"
                  << "\nfile: the cQasm file to execute\n"
                  << std::endl;
}

}

int main(int argc, char **argv) {
    auto cliOptions = parseCLIOptions(argc, argv);

    if (!cliOptions) {
        printUsage(argv);
        return -1;
    }

    std::cout << "Will run cQasm file '" << cliOptions->filePath << "'..." << std::endl;

    auto output = superpositeur::executeFile(cliOptions->filePath);

    if (auto *simulationError = std::get_if<superpositeur::SimulationError>(&output)) {
        std::cerr << "Simulation failed:\n"
                  << simulationError->message << std::endl;
        return 1;
    }
    
    // std::cout << std::get<superpositeur::CQasmSimulationResult>(output).stateWithClassicalBits.getReducedDensityMatrix(cliOptions->reductionQubits) << std::endl;

    for (auto [bitString, proba]: std::get<superpositeur::CQasmSimulationResult>(output).getMeasurementRegisterStatistics()) {
        std::cout << bitString << " -> " << proba << std::endl;
    }

    return 0;
}