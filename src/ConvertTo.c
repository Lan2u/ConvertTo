#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include "ConvertTo.h"

#define CONVERSIONS_FILE_PATH "../data/conversions"

int main(int argc, char *argv[] ){
	printf("ConvertTo Started!\n");
	
	argc = argc - 1;
	
	printf("%d Arguments!\n", argc);
	
	switch (argc) {
        case 1: {
            if (strcmp(argv[1], "-h") == 0) {
                displayHelp();
                return 0;
            } else if (strcmp(argv[1], "-test") == 0) {
                runTests();
                return 0;
            }else{

            }
        }
        case 3: {
            double outMag;
            convert(argv[1], argv[2], &outMag, argv[3]);
            printf("%s%s is equivalent to %f%s\n", argv[1], argv[2], outMag, argv[3]);
            return 0;
        }
        default:break;
    }

    printf("ConvertTo -h for help\n");
    return 1;
}

void runTests() {
    printf("Starting Tests\n");
    printf("Finished Tests\n");
}

void displayHelp(){
	printf("To get help, wait until I write the documentation\n");
    printf("Usage: ConvertTo <Quantity> <unit> <output unit>\n");
}

// Returns 0 if successful
// Returns 1 if not
// Result is put into the memory location at outMagnitude
int convert(char* quantity, char* in_units, double* outMagnitude, char* out_units){
    printf("Starting conversion on quantity=%s in_units=%s out_units=%s\n",quantity,in_units,out_units);
	double magnitude;
    magnitude = atof(quantity);

    if (magnitude <= 0.0){
		printf("Quantity isn't a double number > 0!");
		return 1;
	}
	
	struct conversion convert;
	convert = getConversion(in_units, out_units);

    if (convert.factor == 0.0 && convert.offset == 0.0){
        // The conversion wasn't found
        printf("Failed to convert units\n");
        return 1;
    }else{
        printf("DEBUG factor = %f, offset = %f\n", convert.factor, convert.offset);
    }

	*outMagnitude = magnitude * convert.factor + convert.offset;

    printf("DEBUG val = %f , magnitude = %f\n", outMagnitude, magnitude);

    return 0;
}

struct conversion getConversion(char* in_units, char* out_units){
	// Read from file unitl a conversion is found
	// Return the conversion factor if it is found as in_unit to out_unit or (1/conversion_factor) if it is found as out_unit to in_unit
	// Eg. kg g 1000
	// If you were going from kg to g then 1000 is returned. If you are going from g to kg then 1/1000 = 0.001 is returned.
	char* mode = "r";
    FILE* file;
    file = fopen(CONVERSIONS_FILE_PATH,mode); // File opened

    if (file == NULL){
        printf("File couldn't be opened\n");
        struct conversion conv;
        conv.factor = 0.0;
        conv.offset = 0.0;
        fclose(file);
        return conv;
    }

    // Buffer to hold data read from file
    size_t BUF_SIZE = 64; // size_t = unsigned int
    char buf[BUF_SIZE];

    do {
        fscanf(file, "%s", buf); // Load the first string (either # or a unit1)
        if (buf[0] == '#'){ // A line starting with # means it is ignored
            printf("Ignoring line as starts with #\n");

            int val = fgetc(file);
            while(val != '\n'){
                printf("%c",val);
                // Skip to end of line
                val = fgetc(file);
            }
        }else{ // The line should be processed
            printf("Processing line\n");

            char unit1[BUF_SIZE];
            char unit2[BUF_SIZE];

           // copyArrayUntil(unit1, NULL);

            strncpy(unit1, buf, BUF_SIZE);
            fscanf(file,"%s", buf); // Load the second units

            strncpy(unit2, buf, BUF_SIZE);

            if (strcmp(&unit1[0], in_units) == 0 && strcmp(&unit2[0], out_units) == 0) {
                // The conversion has been found the right way around
                printf("Unit found right way around\n");

                struct conversion conv;
                fscanf(file, "%f", &conv.factor);
                fscanf(file, "%f", &conv.offset);

                fclose(file);
                return conv;
            }else if (strcmp(&unit1[0],out_units) == 0 && strcmp(&unit2[0], in_units) == 0) {
                // The conversion has been found the wrong way around
                printf("Unit found wrong way around\n");

                struct conversion conv;

                float factor;
                fscanf(file, "%f", &factor);
                // The conversion is found the other way (unit2 -> unit1)
                // around so need to reverse the coefficient and offset
                conv.factor = (float) (1.0 / factor);

                float offset;
                fscanf(file, "%f", &offset);
                conv.offset = -offset;

                fclose(file);
                return conv;
            }
        }
    } while (1); // Loop until end of file

    // If we reach end of file without finding any units
    printf("Reached EOF!\n");

    printf("Those units were not recognised!\n");
    // No units were found
    struct conversion conv;
    conv.factor = 0.0;
    conv.offset = 0.0;
    fclose(file);
    return conv;
}

// Copy the given array1 into a new array stopping once the terminating character is reached
void copyArrayUntil(char* array1, char terminateCharacter) {
    int endPos;
    int arraySize = sizeof(array1);
    for (endPos = 0; array1[endPos] != terminateCharacter && endPos < arraySize; endPos++);
    printf("The end pos is %d\n", endPos);
}
