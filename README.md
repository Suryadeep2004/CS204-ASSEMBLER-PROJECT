TO EXECUTE THE PROJECT:

File Setup:
Ensure that all three files (assembler.cpp, assembler.hpp, and input.asm) are present in the same directory.

Compilation:
Compile the assembler.cpp file using the command:g++ assembler.cpp
This will generate an executable file named a.out.

Execution:
Run the executable using the command:./a.out
This will execute the program and produce the output file output.mc containing the required machine code of the instructions and the memory segment representation allocated in the .data segment.


PROJECT DESCRIPTION:

This project aims to implement a robust RISC-V assembler that translates RISC-V assembly code into machine code. The project consists of three main files:

assembler.cpp: This file contains the main code (driver) for the project. It reads a sample RISC-V code from the input.asm file, processes it, and generates the corresponding machine code for the instructions. The machine code, along with the memory segment representation allocated in the .data segment, is then written to an output file named output.mc.

assembler.hpp: This header file contains all the required functions and declarations used in the assembler.cpp file.

input.asm: This file contains a sample RISC-V code that serves as input to the assembler. The code may include instructions, labels, and data declarations.

KEY FEATURES OF THE ASSEMBLER:

Register Aliases: Our code supports multiple alternate names for registers such as t0, t1, sp, etc. This allows for flexibility in writing assembly code.

No Support for Pseudo Instructions: The assembler does not support pseudo instructions as they are not necessary as per the project requirements. Only standard RISC-V instructions are processed.

Data Segment Representation: Data segments are represented in decimal format, and strings are represented in ASCII format. This ensures compatibility with the RISC-V architecture.

Input Line Format: Every line in the input file should either contain an instruction, a comment, or a blank space. This ensures that the program counter (PC) is correctly evaluated during assembly.

Case Insensitivity: Our code supports case insensitivity for instructions. For example, if the user inputs "add" instruction, they can also use "Add" without causing errors.

Error Handling: The code incorporates robust error handling mechanisms. It detects errors such as incorrect instruction formats, typos in instruction names, and incorrect register names (e.g., x32, x33). Additionally, the assembler identifies incorrect usage of labels, such as attempting to jump to labels that are not defined in the code, and displays appropriate error messages.

Overall, this RISC-V assembler provides a reliable and efficient solution for converting RISC-V assembly code into machine code, with comprehensive error handling capabilities to ensure the accuracy and integrity of the assembly process.

README FOR CHAMPSIM:
Here the cache sets are divided into Hot and Cold inorder to reduce the conflict misses.
Dynamic remapping is used.
An attempt to increse the hit rates on a software level is made through this project.

