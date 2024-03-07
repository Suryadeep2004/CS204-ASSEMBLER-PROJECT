#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iomanip>

// Define opcodes for instructions
std::map<std::string, std::string> opcodeMap = {
    {"add", "0110011"},
    {"and", "0110011"},
    {"or", "0110011"},
    {"sll", "0110011"},
    {"slt", "0110011"},
    {"sra", "0110011"},
    {"srl", "0110011"},
    {"sub", "0110011"},
    {"xor", "0110011"},
    {"mul", "0110011"},
    {"div", "0110011"},
    {"rem", "0110011"},
    {"addi", "0010011"},
    {"andi", "0010011"},
    {"ori", "0010011"},
    {"lb", "0000011"},
    {"ld", "0000011"},
    {"lh", "0000011"},
    {"lw", "0000011"},
    {"jalr", "1100111"},
    {"sb", "0100011"},
    {"sw", "0100011"},
    {"sd", "0100011"},
    {"sh", "0100011"},
    {"beq", "1100011"},
    {"bne", "1100011"},
    {"bge", "1100011"},
    {"blt", "1100011"},
    {"auipc", "0010111"},
    {"lui", "0110111"},
    {"jal", "1101111"}
};

std::map<std::string, int> labels;

// Function to parse the assembly code and generate machine code
void assemble(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile) {
        std::cerr << "Error: Unable to open input file: " << inputFileName << std::endl;
        return;
    }

    if (!outputFile) {
        std::cerr << "Error: Unable to open output file: " << outputFileName << std::endl;
        return;
    }

    std::string line;
    int address = 0;

    while (std::getline(inputFile, line)) {
        // Split the line into tokens
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        // Check for labels
        if (tokens[0].back() == ':') {
            labels[tokens[0].substr(0, tokens[0].size() - 1)] = address;
            tokens.erase(tokens.begin());
        }

        // Check for assembler directives
        if (tokens[0] == ".text" || tokens[0] == ".data") {
            // Handle .text and .data directives
            // For simplicity, assuming .text and .data segments start at fixed addresses
            if (tokens[0] == ".text")
                address = 0;
            else if (tokens[0] == ".data")
                address = 0x10000000;
        } else if (tokens[0] == ".byte" || tokens[0] == ".half" || tokens[0] == ".word" || tokens[0] == ".dword" || tokens[0] == ".asciz") {
            // Handle data directives
            // For simplicity, skipping this part in this basic implementation
        } else {
            // Handle instructions
            std::string opcode = opcodeMap[tokens[0]];

            // For now, assuming all instructions are in the R-format
            // Assuming registers are specified as x0, x1, ..., x31
            int rd = std::stoi(tokens[1].substr(1)); // Remove 'x' and convert to int
            int rs1 = std::stoi(tokens[2].substr(1));
            int rs2 = std::stoi(tokens[3].substr(1));

            // Format the machine code
            int machineCode = 0;
            machineCode |= (rs2 << 20) | (rs1 << 15) | (opcode << 12) | (rd << 7) | 0x33;

            // Write the machine code to the output file
            outputFile << "0x" << std::hex << std::setfill('0') << std::setw(8) << address << " 0x" << std::setw(8) << machineCode << std::endl;

            // Increment the address
            address += 4;
        }
    }

    // Write termination code
    outputFile << "0x" << std::hex << std::setfill('0') << std::setw(8) << address << " 0x" << std::setw(8) << 0 << std::endl;

    inputFile.close();
    outputFile.close();
}

int main() {
    assemble("input.asm", "output.mc");
    return 0;
}
