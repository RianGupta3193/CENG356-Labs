/*
 ============================================================================
 Name        : memory.c
 Author      : Rian Gupta
 Revised by  :
 Version     :
 Copyright   : Copyright 2023
 Description : Lab 6 in C, ANSI-C Style
 ============================================================================
 */

#include "header.h"

// This is the menu that shows up when the program runs
// It lists all the options the user can choose from
char *menu =    "\n" \
                " ***********Please select the following options**********************\n" \
                " *    This is the memory operation menu (Lab 6)                     *\n" \
                " ********************************************************************\n" \
                " *    1. Write a double-word (32-bit) to the memory                 *\n"  \
                " ********************************************************************\n" \
                " *    2. Read a byte (8-bit) data from the memory                   *\n" \
                " *    3. Read a double-word (32-bit) data from the memory           *\n" \
                " ********************************************************************\n" \
                " *    4. Generate a memory dump from any memory location            *\n" \
                " ********************************************************************\n" \
                " *    e. To Exit, Type 'e'  or 'E'                                  *\n" \
                " ********************************************************************\n";

//---------------------------------------------------------------
// This function generates a random number between 0 and 255 (0x00 to 0xFF)
// We use this to fill the memory with random data at the start
unsigned char rand_generator()
{
    return rand()%255;  // gives us a random byte value
}

//---------------------------------------------------------------
// This function frees (releases) the memory we allocated
// We call this at the end so we don't have a memory leak
void free_memory(char *base_address)
{
    free(base_address);  // release the memory back to the system
    return;
}

//---------------------------------------------------------------
// This function sets up the 1MB memory space
// It allocates the memory using malloc and fills it with random numbers
char *init_memory()
{
    char *mem = malloc(MEM_SIZE);  // allocate 1MB of memory (MEM_SIZE = 1024*1024)

    // Loop through every byte in memory and fill it with a random number
    // This simulates real memory that has random leftover data in it
    int i;
    for (i = 0; i < MEM_SIZE; i++) {
        mem[i] = rand_generator();  // fill each byte with a random value
    }

    return mem;  // return the pointer to the start of our memory block
}

//---------------------------------------------------------------
// This function writes a 32-bit value (double word) to a specific memory location
// base_address is the start of memory, offset is how far in we want to write
void write_dword(const char *base_address, const int offset, const unsigned int dword_data){
    // We cast the target address to an unsigned int pointer
    // so we can write all 4 bytes at once instead of one by one
    unsigned int *target = (unsigned int *)(base_address + offset);
    *target = dword_data;  // write the 32-bit value to that address
    printf("Wrote 0x%08X to offset 0x%X\n", dword_data, offset);  // confirm what was written
}

//---------------------------------------------------------------
// This function reads 1 byte from a specific memory location and prints it
// base_address is the start of memory, offset is how far in we want to read
unsigned char read_byte(const char *base_address, const int offset){
    // Read 1 byte by dereferencing the pointer at base_address + offset
    unsigned char byte_data = (unsigned char)(*(base_address + offset));
    printf("Byte at offset 0x%X: 0x%02X\n", offset, byte_data);  // print the byte in hex
    return byte_data;  // return the byte value
}

//---------------------------------------------------------------
// This function reads a 32-bit value (double word) from a specific memory location
// base_address is the start of memory, offset is how far in we want to read
unsigned int read_dword(const char *base_address, const int offset){
    // Cast to unsigned int pointer so we read all 4 bytes at once
    unsigned int dword_data = *(unsigned int *)(base_address + offset);
    printf("Double-word at offset 0x%X: 0x%08X\n", offset, dword_data);  // print it in hex
    return dword_data;  // return the 32-bit value
}

//---------------------------------------------------------------
// This function prints a memory dump starting from base_address + offset
// It shows the contents of memory in hex AND as ASCII characters side by side
// This is useful to visually inspect what is stored in memory
void memory_dump(const char *base_address, const int offset, unsigned int dumpsize){

    // Make sure dumpsize is valid -- if too small or too big, set it to minimum (256)
    if (dumpsize < MIN_DUMP_SIZE || dumpsize > MEM_SIZE)
        dumpsize = MIN_DUMP_SIZE;

    unsigned int i, j;
    const char *start = base_address + offset;  // pointer to where we start dumping

    // Loop through memory row by row, 16 bytes per row (DUMP_LINE = 16)
    for (i = 0; i < dumpsize; i += DUMP_LINE)
    {
        // Print the memory address of the first byte in this row
        printf("%p: ", (void *)(start + i));

        // Print each byte in the row as a 2-digit hex number
        for (j = 0; j < DUMP_LINE; j++)
        {
            printf("%02X ", (unsigned char)(start[i + j]));
        }

        printf("--- ");  // separator between the hex side and ASCII side

        // Print each byte as an ASCII character
        // If the byte is a printable character (0x20 to 0x7E), show it
        // Otherwise show a dot '.' to represent non-printable bytes
        for (j = 0; j < DUMP_LINE; j++)
        {
            unsigned char c = (unsigned char)(start[i + j]);
            if (c >= 0x20 && c <= 0x7E)
                printf("%c", c);   // printable character
            else
                printf(".");       // non-printable, show dot instead
        }

        printf("\n");  // move to next row
    }

    return;
}

//---------------------------------------------------------------
// This is the main memory controller function
// It shows a menu and lets the user choose to read, write, or dump memory
// It keeps looping until the user presses 'e' to exit
void setup_memory()
{
    char *mem = init_memory();  // initialize and get our 1MB memory block
    char options = 0;           // stores the user's menu choice
    unsigned int offset, dumpsize;
    char tempchar;
    unsigned int dword_data;    // stores the 32-bit value for read/write operations

    do{
        // Only show the menu if the last input wasn't just a newline/enter key
        if (options != 0x0a)
        {
            puts(menu);  // print the menu options
            printf ("\nThe base address of your memory is: %I64Xh (HEX)\n", (long long unsigned int)(mem));  // show where memory starts
            puts("Please make a selection:");
        }
            options = getchar();  // get the user's choice

            switch (options)
            {
                case '1':  // user wants to write a double word
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", (unsigned int*)&offset);    // get the offset address
                    puts("Please input your DOUBLE WORD data to be written (in HEX):");
                    scanf("%x", (unsigned int*)&dword_data);    // get the data to write
                    write_dword(mem, offset, dword_data);   // write it to memory
                    continue;

                case '2':  // user wants to read a single byte
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", &offset);    // get the offset address
                    read_byte(mem, offset);  // read and print the byte
                    continue;

                case '3':  // user wants to read a double word (32-bit)
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", &offset);     // get the offset address
                    read_dword(mem, offset);  // read and print the double word
                    continue;

                case '4':  // user wants to see a memory dump
                    puts("Please input your memory's offset address (in HEX, should be a multiple of 0x10h):");
                    scanf("%x", &offset);    // get the starting offset
                    puts("Please input the size of the memory to be dumped (a number between 256 and 1024 ):");
                    scanf("%d", &dumpsize);  // get how many bytes to dump
                    memory_dump(mem, offset, dumpsize);  // print the memory dump
                    continue;

                case 'e':
                case 'E':  // user wants to exit
                    puts("Code finished, press any key to exit");
                    free_memory(mem);  // free the memory before exiting
                    while ((tempchar = getchar()) != '\n' && tempchar != EOF);  // clear input buffer
                    tempchar = getchar();  // wait for final keypress
                    return;  // exit the function

                default:
                    // if input is not valid, just loop back to the menu
                    continue;
            }
    }while (1);  // keep looping forever until user exits with 'e'

    return;
}
