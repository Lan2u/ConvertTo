#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include "ConvertTo.h"

char debug_output = 0; // Should the program print debug messages

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
            if (convert(argv[1], argv[2], &outMag, argv[3])) {
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

// Returns 1 if successful
// Returns 0 if not
// Result is put into the memory location at outMagnitude
int convert(char *quantity, char *in_units, double *outMagnitude, char *out_units) {
    double magnitude;
    magnitude = atof(quantity);

    struct conversion convert;
    convert = getConversion(in_units, out_units);

    if (convert.factor == 0.0 && convert.offset == 0.0) {
        // The conversion wasn't found
        printf("Failed to convert units\n");
        return 0;
    }

    // Out magnitude = in_magnitude * factor + offset
    *outMagnitude = magnitude * convert.factor + convert.offset;
    return 1;
}

struct conversion getConversion(char *in_units, char *out_units) {
    char *mode = "r";
    FILE *file;
    file = fopen(CONVERSIONS_FILE_PATH, mode); // File opened

    if (file == NULL) {
        printf("File couldn't be opened\n");
        struct conversion conv;
        conv.factor = 0.0;
        conv.offset = 0.0;
        fclose(file);
        return conv;
    }

    if (debug_output){
        printf("The file was opened successfully!\n");
    }

    // Buffer to hold data read from file
    size_t BUF_SIZE = 64; // size_t = unsigned int
    char buf[BUF_SIZE];

    int charRead; // Stores the number of characters read each time the program starts a new line

    if (debug_output) {
        printf("The conversion units: in= %s out= %s\n", in_units, out_units);
    }
    do {

        charRead = fscanf(file, "%s", buf); // Load the first string (either # or a unit1)
        if (buf[0] == '#') { // A line starting with # means it is ignored

            int val = fgetc(file);
            while (val != '\n') {
                // Skip to end of line
                val = fgetc(file);
            }
        } else { // The line should be processed

            char unit1[BUF_SIZE];
            char unit2[BUF_SIZE];

            strncpy(unit1, buf, BUF_SIZE);
            fscanf(file, "%s", buf); // Load the second units

            strncpy(unit2, buf, BUF_SIZE);

            if (strcmp(&unit1[0], in_units) == 0 && strcmp(&unit2[0], out_units) == 0) {
                // The conversion has been found the right way around

                struct conversion conv;
                fscanf(file, "%f", &conv.factor);
                fscanf(file, "%f", &conv.offset);

                if (debug_output) {
                    printf("The conversion was found the right way around!\n");
                    printf("Unit1=%s Unit2=%s Factor =%f Offset =%f\n", unit1, unit2, conv.factor, conv.offset);
                }

                fclose(file);
                return conv;
            }
            /*
            else if (strcmp(&unit1[0], out_units) == 0 && strcmp(&unit2[0], in_units) == 0) {
                // The conversion has been found the wrong way around

                struct conversion conv;

                // The conversion is found the other way (unit2 -> unit1)
                // around so need to reverse the coefficient and offset:
                float factor;
                fscanf(file, "%f", &factor);
                conv.factor = (float) (1.0 / factor);

                float offset;
                fscanf(file, "%f", &offset);
                conv.offset = -offset;

                if (verbose_output) {
                    printf("The conversion was found the wrong way around!\n");
                    printf("Unit1=%s Unit2=%s Factor =%f Offset =%f\n", unit1, unit2, conv.factor, conv.offset);
                }

                fclose(file);
                return conv;
            }*/
        }
    } while (charRead > 0); // Loop until end of file
    // If we reach end of file without finding any units

    printf("Those units were not recognised!\n");
    // No units were found
    struct conversion conv;
    conv.factor = 0.0;
    conv.offset = 0.0;
    fclose(file);
    return conv;
}