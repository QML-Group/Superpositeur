#include "superpositeur/API.hpp"

#include "cqasm.hpp"
#include "superpositeur/LibqasmInterface.hpp"

namespace superpositeur {

namespace {
using Program = cqasm::v1x::ast::One<cqasm::v1x::semantic::Program>;
using AnalysisResult = cqasm::v1x::analyzer::AnalysisResult;

auto createAnalyzer() {
    cqasm::v1x::analyzer::Analyzer analyzer{"1.2"};
    analyzer.register_default_functions_and_mappings();
    return analyzer;
}

std::string getErrors(AnalysisResult const &analysisResult) {
    assert(!analysisResult.errors.empty());

    std::stringstream errorsString;
    bool first = true;
    for (auto const &error : analysisResult.errors) {
        if (!first) {
            errorsString << "\n";
        } else {
            first = false;
        }
        errorsString << error;
    }

    return errorsString.str();
}

Program parseFile(std::string const &filePath) {
    auto res = createAnalyzer().analyze(filePath);

    if (!res.errors.empty()) {
        throw std::runtime_error(getErrors(res));
    }

    return res.root;
}

Program parseString(std::string const &s) {
    auto res = createAnalyzer().analyze_string(s);

    if (!res.errors.empty()) {
        throw std::runtime_error(getErrors(res));
    }

    return res.root;
}

Program unwrap(auto parseResult,
               std::stringstream &errors) { // throws when analysis fails
    return parseResult.unwrap(errors);
}

CQasmSimulationResult
execute(Program program, Operations const &operations) {
    if (program.empty()) {
        throw std::runtime_error("Program is empty");
    }

    if (!program->error_model.empty()) {
        throw std::runtime_error(
            "Probabilistic error models are not supported");
    }

    return executeCqasmCode(*program, operations);
}
} // namespace

CQasmSimulationResult
executeStringImpl(std::string const &s, Operations const &operations) {
    return execute(parseString(s), operations);
}

std::variant<CQasmSimulationResult, SimulationError>
executeString(std::string const &s, Operations operations) {
    try {
        return executeStringImpl(s, operations);
    } catch (std::exception const &e) {
        return SimulationError{e.what()};
    }
}

CQasmSimulationResult
executeFileImpl(std::string const &filePath, Operations operations) {
    return execute(parseFile(filePath), operations);
}

std::variant<CQasmSimulationResult, SimulationError>
executeFile(std::string const &filePath, Operations operations) {
    try {
        return executeFileImpl(filePath, operations);
    } catch (std::exception const &e) {
        return SimulationError{e.what()};
    }
}

} // namespace  superpositeur