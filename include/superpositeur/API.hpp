#pragma once

#include "superpositeur/MixedState.hpp"
#include "superpositeur/DefaultOperations.hpp"
#include "superpositeur/LibqasmInterface.hpp"

#include <optional>
#include <string>

namespace superpositeur {

struct SimulationError {
    std::string message = "Simulation failed";
};

CQasmSimulationResult
executeStringImpl(std::string const &s, Operations const &operations);

std::variant<CQasmSimulationResult, SimulationError>
executeString(std::string const &s,
              Operations operations = default_operations::defaultOperations);

CQasmSimulationResult
executeFileImpl(std::string const &filePath, Operations operations);

std::variant<CQasmSimulationResult, SimulationError>
executeFile(std::string const &filePath,
            Operations operations = default_operations::defaultOperations);

} // namespace  superpositeur