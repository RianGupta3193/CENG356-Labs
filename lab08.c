/*
 * This file contains functions for Lab 8 and Lab 9.
 * In Lab 8, we fetch, decode, and prepare instructions.
 * In Lab 9, we execute those instructions and print results.
 */

#include "header.h"
#include "lab8header.h"

extern char *regNameTab[N_REG];
extern unsigned int PCRegister; // program counter (points to next instruction)

void CPU(char *mem){
    unsigned int machineCode = 0;
    unsigned char opcode = 0;

    // initialize PC to start of code section
    PCRegister = CODESECTION;

    do{
      printf("\nPC:%x\n", PCRegister);

      // get instruction from memory
      machineCode = CPU_fetchCode(mem, PCRegister);

      // if instruction is 0, stop execution
      if (machineCode == 0)
          break;

      // move PC to next instruction (each instruction is 4 bytes)
      PCRegister += 4;

      // decode instruction to get opcode
      opcode = CPU_Decode(machineCode);
      printf("Decoded Opcode is: %02X. \n", opcode);

      // Lab 9: execute instruction (after decode is working)
      // CPU_Execution(opcode, machineCode, mem);

    }while (1);  // runs until break condition

    // print results after execution
    printRegisterFiles();
    printDataMemoryDump(mem);
}


// Step 1: fetch 32-bit instruction from memory using PC
unsigned int CPU_fetchCode(char *mem, int codeOffset){
    // read 4 bytes (1 instruction) from memory
    return read_dword(mem, codeOffset);
}


// Step 2: decode instruction to get opcode (top 6 bits)
unsigned char CPU_Decode(unsigned int machineCode){
    // shift right 26 bits and mask to get 6-bit opcode
    unsigned char opcode = (unsigned char)((machineCode >> 26) & 0x3F);
    return opcode;
}


// Lab 9: execute instructions based on opcode
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char *mem){
    unsigned char rt = 0;

    switch (opcode)
    {
        // example: la instruction
        case 0b101111:
            // get rt register index (bits 16–20)
            rt = (machineCode & 0x001F0000) >> 16;

            // store immediate value (last 16 bits) into register
            regFile[rt] = machineCode & 0x0000FFFF;

            // debug print if enabled
            if (DEBUG_CODE){
                printf("Code Executed: %08X\n", machineCode);
                printf("PC Register is %08X\n", PCRegister);
            }
            break;

        // lb instruction (not implemented yet)
        case 0b100000:
            // TODO: implement lb
            break;

        // more instructions go here (add, lw, sw, addi, j, etc.)

        default:
            // error if opcode is unknown
            printf("Wrong instruction! Opcode: %02X Code: %08X\n", opcode, machineCode);
            system("PAUSE");
            exit(3);
            break;
    }
}


// Step 3: print all registers with their names
void printRegisterFiles(){
    int i;
    printf("\n--- MIPS Register Files ---\n");

    // loop through all registers and print value
    for (i = 0; i < N_REG; i++) {
        printf("%s\t= 0x%08X\n", regNameTab[i], regFile[i]);
    }
}


// Step 4: print first 256 bytes of data memory
void printDataMemoryDump(char *mem){
    printf("\n--- Data Memory Dump ---\n");

    // dump memory starting from data section
    memory_dump(mem, DATASECTION, 256);
}
