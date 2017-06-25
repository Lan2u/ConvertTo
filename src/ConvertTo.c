#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include "ConvertTo.h"

#define BUF_SIZE 64
// Should the program print debug messages
#define DEBUG_OUTPUT 1

void skipLine(FILE *pIobuf);

int main(int argc, char *argv[]) {
    argc = argc - 1; // Ignoring the first argument since it is just the name of the program
    switch (argc) {
        case 1: {
            if (strcmp(argv[1], "-h") == 0) {
                displayHelp();
                return 0;
            }
        }
        case 3: {
            double outMag;
            if (!convert(argv[1], argv[2], &outMag, argv[3])) {
                printf("%s%s is equivalent to %f%s\n", argv[1], argv[2], outMag, argv[3]);
                return 0;
            } else {
                return 1;
            }
        }
        default:
            break;
    }

    printf("ConvertTo -h for help\n");
    return 1;
}

void displayHelp() {
    printf("Usage: ConvertTo <Quantity> <unit> <output unit>\n");
}

// Returns 0 if successful
// Returns 1 if not
// Result is put into the memory location at outMagnitude
/*
             *
             * // TODO Change how the program works so a different form of the conversion formular is used if the
             * // conversion is found the wrong way around which means don't need to store the conversion numbers
             * // twice.
             * // Unit1 = Factor * Unit2 + Coefficient
             * // Unit2 = (Unit1 - Coefficient) / Factor
             * // Factor and Coefficient don't change for going backwards or forwards for the same 2 units
             * // Result = getConversion(char[] unit1, char[] unit2, double factor*, double offset*);
             * // Result = 0 -> normal way round Unit1 = Factor * Unit2 + Coefficient
             * // Result = 1 -> Reverse conversion: Unit2 = (Unit1 - Coefficient) / Factor
             * // Result = -1 -> No conversion found
             */
int convert(char *quantity, char *in_units, double *outMagnitude, char *out_units) {
    if (DEBUG_OUTPUT) {
        printf("The conversion units: in= %s out= %s\n", in_units, out_units);
    }

    double magnitude;
    magnitude = atof(quantity);

    char *mode = "r";
    FILE *file;
    file = fopen(CONVERSIONS_FILE_PATH, mode); // File opened

    if (file == NULL) {
        printf("Conversions couldn't be loaded\n");
        return 1;
    }else if (DEBUG_OUTPUT){
        printf("The file was opened successfully!\n");
    }


    char buf[BUF_SIZE]; // Buffer to hold data read from file
    int charRead; // Stores the number of characters read each time the program starts a new line

    do {
        charRead = fscanf(file, "%s", buf); // Load the first string (either # or a unit1)
        if (buf[0] == '#') { // A line starting with # means it is ignored
            // Skip to end of line
            skipLine(file);
        } else { // The line should be processed

            char unit1[BUF_SIZE];
            char unit2[BUF_SIZE];

            strncpy(unit1, buf, BUF_SIZE);
            fscanf(file, "%s", buf); // Load the second units

            strncpy(unit2, buf, BUF_SIZE);

            if (strcmp(&unit1[0], in_units) == 0 && strcmp(&unit2[0], out_units) == 0) {
                // The conversion has been found the right way around

                float factor, offset;
                fscanf(file, "%f", &factor);
                fscanf(file, "%f", &offset);

                if (DEBUG_OUTPUT) {
                    printf("The conversion was found the right way around!\n");
                    printf("Unit1=%s Unit2=%s Factor =%f Offset =%f\n", unit1, unit2, factor, offset);
                }

                *outMagnitude = factor * magnitude +offset;

                fclose(file);
                return 0;
            } else if (strcmp(&unit1[0], out_units) == 0 && strcmp(&unit2[0], in_units) == 0){
                // The conversion has been found the wrong way around
                float factor, offset;
                fscanf(file, "%f", &factor);
                fscanf(file, "%f", &offset);

                if (DEBUG_OUTPUT) {
                    printf("The conversion was found the wrong way around!\n");
                    printf("Unit1=%s Unit2=%s Factor =%f Offset =%f\n", unit1, unit2, factor, offset);
                }

                *outMagnitude = (magnitude - offset)/factor;

                fclose(file);
                return 0;
            }
        }
    } while (charRead > 0); // Loop until end of file
    // If we reach end of file without finding any units

    printf("Those units were not recognised!\n");
    fclose(file);
    return 1;
}

void skipLine(FILE *file) {
    int val = fgetc(file);
    while (val != '\n') {
        val = fgetc(file);
    }
}