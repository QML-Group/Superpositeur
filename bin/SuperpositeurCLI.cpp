#include "superpositeur/cli/SuperpositeurCLISession.hpp"

int main() {
    superpositeur::cli::SuperpositeurCLISession session;

    std::string input;
    std::string instruction;
    std::cout << "Enter a command ('help' for help, ctrl-C or ctrl-D to exit)\n>>> ";
    while (std::getline(std::cin, input)) {
        std::stringstream lineStream(input);
        bool hasOutput = false;
        while(std::getline(lineStream, instruction, ';')){
            std::string output = session(instruction);
            hasOutput |= (!output.empty());
            std::cout << output;
        }
        if (hasOutput) {
            std::cout << std::endl;
        }
        std::cout << ">>> ";
    }
    std::cout << std::endl;

    return 0;
}