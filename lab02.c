/*
 ============================================================================
 Name        : Lab2.c
 Author      : Rian Gupta
 Description : Lab 2 – Convert 8-bit signed binary to decimal
               and 32-bit binary to floating point number
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Max size of a 32-bit binary number
#define SIZE 32

// Predefined 32-bit floating point test numbers (from lab instructions)
#define number1 "11000001010010000000000000000000"  // -12.5
#define number2 "01000001010101000000000000000000"  // 13.25

// Function prototypes
void convert_binary_to_signed(const char *binary);
void convert_binary_to_float(const char *binary);

// Menu displayed to user
char *menu =
    "\n"
    "===================================================================\n"
    "************Please select the following options********************\n"
    " * 1. Binary number to signed decimal number conversion (Lab 2)    *\n"
    " * 2. Binary number to Floating number conversion (Lab 2)          *\n"
    " ***************************************************************** \n"
    " * e. To Exit, Type 'e'                                            *\n"
    " ***************************************************************** \n";

int main(void)
{
    char option;         // To store user's menu choice
    char inputs[33] = {0}; // Buffer to store binary input as string

    while (1)
    {
        // Display menu
        puts(menu);

        fflush(stdin); // Clear input buffer
        option = getchar(); // Get user choice

        switch (option)
        {
            case '1':
                // Option 1: 8-bit signed binary to decimal
                puts("Please input your 8-bit BINARY number:");
                scanf("%s", inputs); // Read user input
                convert_binary_to_signed(inputs); // Call function to convert
                break;

            case '2':
                // Option 2: 32-bit binary to floating point
                // Using predefined numbers as suggested in lab
                strcpy(inputs, number1); // First test number
                convert_binary_to_float(inputs);

                strcpy(inputs, number2); // Second test number
                convert_binary_to_float(inputs);
                break;

            case 'e':
                // Exit the program
                puts("Code finished, exit now");
                return EXIT_SUCCESS;

            default:
                puts("Not a valid entry"); // Invalid input
        }
    }
}

/*
 * Function: convert_binary_to_signed
 * -----------------------------------
 * Converts an 8-bit binary string to signed decimal number.
 * Uses two's complement logic for negative numbers.
 */
void convert_binary_to_signed(const char *binary)
{
    int value = 0;

    // Loop through each bit and build decimal value
    for (int i = 0; i < 8; i++)
    {
        value <<= 1; // Shift left to make room for next bit
        if (binary[i] == '1')
            value |= 1; // Set last bit if current bit is 1
    }

    // If first bit is 1, it's a negative number (two's complement)
    if (binary[0] == '1')
        value -= 256;

    // Display result
    printf("Signed decimal value: %d\n", value);
}

/*
 * Function: convert_binary_to_float
 * ---------------------------------
 * Converts a 32-bit binary string to a floating-point number (IEEE-754 format).
 * Steps:
 * 1. Determine sign (first bit)
 * 2. Calculate exponent (next 8 bits) with bias 127
 * 3. Calculate mantissa (last 23 bits) with implicit leading 1
 * 4. Combine to get final decimal value
 */
void convert_binary_to_float(const char *binary)
{
    int sign;
    int exponent = 0;
    double mantissa = 1.0; // Start with hidden leading 1
    double result;

    // Determine sign
    sign = (binary[0] == '1') ? -1 : 1;

    // Convert exponent bits (bits 1-8)
    for (int i = 1; i <= 8; i++)
    {
        exponent <<= 1;
        if (binary[i] == '1')
            exponent |= 1;
    }
    exponent -= 127; // Remove bias

    // Convert mantissa bits (bits 9-31)
    double weight = 0.5; // First fractional bit is 1/2
    for (int i = 9; i < 32; i++)
    {
        if (binary[i] == '1')
            mantissa += weight; // Add fractional value
        weight /= 2; // Move to next fractional bit
    }

    // Combine sign, exponent, and mantissa
    result = sign * mantissa * pow(2, exponent);

    // Display result
    printf("Floating-point value: %f\n", result);
}
