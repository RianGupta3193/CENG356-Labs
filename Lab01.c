/* Name: Rian Gupta */
/* ID: N01653193 */

#include <stdio.h>                                          // Include standard I/O library for printf

struct Account {                                            
    unsigned int account_number;                            
    char account_type;                                   
    char *last_name;                                        
    char *first_name;                                     
    float balance;                                       
    char reserved[6];                                       
};                                                         

void displaySizes() {                                       // Function to determine and show sizes
    char alphabet;                                          // Declare a char variable
    short small;                                            // Declare a short variable
    int number;                                             // Declare an int variable
    long calculate;                                         // Declare a long variable
    float decimal;                                          // Declare a float variable
    double precise;                                         // Declare a double variable
    struct Account acc;                                     // Declare an Account struct variable
                                                            
    char *pChar;                                            // Declare a pointer to a char
    short *pShort;                                          // Declare a pointer to a short
    int *pInt;                                              // Declare a pointer to an int
    long *pLong;                                            // Declare a pointer to a long
    float *pFloat;                                          // Declare a pointer to a float
    double *pDouble;                                        // Declare a pointer to a double
    struct Account *pAcc;                                   // Declare a pointer to an Account struct

    printf("--- Basic Data Types ---\n");                  // Print header for basic types
    printf("char:   %zu bytes\n", sizeof(alphabet));        // Display size of char variable
    printf("short:  %zu bytes\n", sizeof(small));           // Display size of short variable
    printf("int:    %zu bytes\n", sizeof(number));          // Display size of int variable
    printf("long:   %zu bytes\n", sizeof(calculate));       // Display size of long variable
    printf("float:  %zu bytes\n", sizeof(decimal));         // Display size of float variable
    printf("double: %zu bytes\n", sizeof(precise));         // Display size of double variable

    printf("\n--- Pointer Data Types ---\n");               // Print header for pointer types
    printf("char*:    %zu bytes\n", sizeof(pChar));         // Display size of char pointer variable
    printf("short*:   %zu bytes\n", sizeof(pShort));        // Display size of short pointer variable
    printf("int*:     %zu bytes\n", sizeof(pInt));          // Display size of int pointer variable
    printf("long*:    %zu bytes\n", sizeof(pLong));         // Display size of long pointer variable
    printf("float*:   %zu bytes\n", sizeof(pFloat));        // Display size of float pointer variable
    printf("double*:  %zu bytes\n", sizeof(pDouble));       // Display size of double pointer variable

    printf("\n--- Structure and Struct Pointer ---\n");     // Print header for struct section
    printf("Account Struct: %zu bytes\n", sizeof(acc));     // Display size of actual struct variable
    printf("Account Ptr:    %zu bytes\n", sizeof(pAcc));    // Display size of struct pointer variable
}                                                           // End of displaySizes function

int main() {                                                // Main function entry point
    displaySizes();                                         // Call the size display function
    return 0;                                               // Return 0 to indicate success
}                                                           // End of main function
