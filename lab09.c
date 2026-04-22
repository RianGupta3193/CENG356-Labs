/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
// Please finish the following functions for lab 8.
// Lab 8 will perform the following functions:
//   1. Fetch the code stored in memory
//   2. Decode the code and prepare for the execution of the code.
//   3. Setup the execution function for CPU.

// Lab 9 will perform the following functions:
//   4. Execute the code stored in the memory and print the results. 
#include "header.h"
#include "lab8header.h"

extern char *regNameTab[N_REG];
extern unsigned int PCRegister; // PC register, always pointing to the next instruction
extern int regFile[N_REG];      // Global register file

void CPU(char *mem){
    unsigned int machineCode = 0;
    unsigned char opcode = 0;

    PCRegister = CODESECTION;  // Start execution at the beginning of the code section

    do{
        printf("\nPC:%x\n", PCRegister);

        // Read instruction from memory
        machineCode = CPU_fetchCode(mem, PCRegister);

        // Stop execution when end of program is reached (0 instruction)
        if (machineCode == 0)
            break;

        // Move to next instruction address
        PCRegister += 4;

        // Decode instruction opcode
        opcode = CPU_Decode(machineCode);
        printf("Decoded Opcode is: %02X. \n", opcode);

        // Execute instruction (Lab 9 part)
        CPU_Execution(opcode, machineCode, mem);

    }while (1);

    // Print final register values after execution
    printRegisterFiles();

    // Print final memory state of data section
    printDataMemoryDump(mem);
}

// Lab 8 - Step 1. Finish the CPU_fectchCode function to
//         read the code section from memory and
//         get the 32-bit machine code from the memory.
unsigned int CPU_fetchCode(char *mem, int codeOffset){
    // Retrieve 32-bit instruction from memory at given offset
    return read_dword(mem, codeOffset);
}

// Lab 8 - Step 2. Finish the CPU_Decode function to
//         decode the instruction and return the
//         opcode/function of the instruction.
//         Hints: Need to consider how to find the opcode/function from different types of instructions:
//                i.e., I-, J- and R-type instructions.
//                The return value should be a 6-bit bianry code.
unsigned char CPU_Decode(unsigned int machineCode){
    // Extract opcode (top 6 bits of instruction)
    unsigned char opcode = (unsigned char)((machineCode >> 26) & 0x3F);
    return opcode;
}

// Lab 9: Finish the function CPU_Execution to run all the instructions.
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char *mem){
    unsigned char rt = 0, rs = 0, rd = 0, shamt = 0, funct = 0;
    short immediate = 0;
    unsigned int address = 0;
    unsigned int eff_addr = 0;

    switch (opcode)
    {
        // R-type instructions (add example)
        case 0b000000:
            rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            rd = (machineCode & 0x0000F800) >> 11;
            funct = machineCode & 0x0000003F;

            if (funct == 0x20) { // add instruction
                regFile[rd] = regFile[rs] + regFile[rt];
                if (DEBUG_CODE){
                    printf("Code Executed: %08X\n", machineCode);
                    printf("****** PC Register is %08X ******\n", PCRegister);
                }
            }
            break;

        // load address instruction
        case 0b101111:
            rt = (machineCode & 0x001F0000) >> 16;

            // Store address into register (DATASECTION base + offset)
            regFile[rt] = (machineCode & 0x0000FFFF) + DATASECTION;

            if (DEBUG_CODE){
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);
            }
            break;

        // load byte instruction
        case 0b100000:
            rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            immediate = (short)(machineCode & 0x0000FFFF);

            // Load byte and sign-extend into register
            regFile[rt] = (signed char)mem[regFile[rs] + immediate];
            break;

        // load word instruction
        case 0b100011:
            rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            immediate = (short)(machineCode & 0x0000FFFF);

            eff_addr = regFile[rs] + immediate;

            // Combine 4 bytes into a single 32-bit value
            regFile[rt] = ((unsigned char)mem[eff_addr] << 24) |
                          ((unsigned char)mem[eff_addr+1] << 16) |
                          ((unsigned char)mem[eff_addr+2] << 8) |
                          ((unsigned char)mem[eff_addr+3]);
            break;

        // store word instruction
        case 0b101011:
            rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            immediate = (short)(machineCode & 0x0000FFFF);

            eff_addr = regFile[rs] + immediate;

            // Store 32-bit register into memory (byte by byte)
            mem[eff_addr]   = (regFile[rt] >> 24) & 0xFF;
            mem[eff_addr+1] = (regFile[rt] >> 16) & 0xFF;
            mem[eff_addr+2] = (regFile[rt] >> 8) & 0xFF;
            mem[eff_addr+3] = regFile[rt] & 0xFF;
            break;

        // add immediate instruction
        case 0b001000:
            rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            immediate = (short)(machineCode & 0x0000FFFF);

            regFile[rt] = regFile[rs] + immediate;
            break;

        // branch instruction (bge type logic)
        case 0b000001:
        case 0b000101:
            rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            immediate = (short)(machineCode & 0x0000FFFF);

            // Branch if condition is satisfied
            if ((signed int)regFile[rs] >= (signed int)regFile[rt]) {
                PCRegister += (immediate << 2) - 4;
            }
            break;

        // jump instruction
        case 0b000010:
            address = machineCode & 0x03FFFFFF;

            // Jump to target address
            PCRegister = (PCRegister & 0xF0000000) | (address << 2);

            // Adjust for automatic PC increment
            PCRegister -= 4;
            break;

        default:
            printf("Wrong instruction! You need to fix this instruction %02X %08X\n", opcode, machineCode);
            system("PAUSE");
            exit(3);
            break;
    }
}

// Lab 8 - Step 3. Print all the 32 registers in regFile and names saved in
//         regNameTab.
void printRegisterFiles(){
    int i;
    printf("\n--- MIPS Register Files ---\n");

    for (i = 0; i < N_REG; i++) {
        printf("%s\t= 0x%08X\n", regNameTab[i], regFile[i]);
    }
}

// Lab 8 - Step 4. Call function memory_dump and pass the proper parameters to dump the first 256
//          bytes from Data section.
void printDataMemoryDump(char *mem){
    printf("\n--- Data Memory Dump ---\n");

    memory_dump(mem, DATASECTION, 256);
}
