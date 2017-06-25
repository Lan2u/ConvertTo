/*
Usage: ConvertTo <Quantity> <unit> <output unit>
	- Gives the given quantity in the given units in the output units
Usage: ConvertTo <Quantity> <Unit>
	- Gives the given quantity in a selection of units (preset)
*/

// Max number of bytes read for each piece of information in the conversions file
#define BUF_SIZE 64
// Should the program print debug messages
#define DEBUG_OUTPUT 1
// Relative path to the conversions file
#define CONVERSIONS_FILE_PATH "../data/conversions"

/* Converts the given quantity in the given units (in units) into the
 * quantity (outMagnitude) in the desired units(out_units)
 *
 * Return 0 if successful
 * Return 1 if not successful
 *
 * Result is put into the memory location at outMagnitude
 * int convert(char *quantity, char *in_units, double *outMagnitude, char *out_units)
 */
int convert(char *, char *, double *, char *);

/*
 * Displays the help message.
 */
void displayHelp();

// Skips to the place in the file after the next newline character in the line
void skipLine(FILE *);