/*
Usage: ConvertTo <Quantity> <unit> <output unit>
	- Gives the given quantity in the given units in the output units
Usage: ConvertTo <Quantity> <Unit>
	- Gives the given quantity in a selection of units (preset)
*/

struct conversion {
    //char* unit1; // The first unit in the conversion (unit1 -> unit2)
    //char* unit2; // The second unit in the conversion
    float factor; // unit2 = unit1 * factor + offset. The conversion from 2 to 1 is therefore (1/conversion)
    float offset; // unit2 = unit1 * factor + offset. The conversion from 2 to 1 is therefore -offset
};

void runTests();

int convert(char *, char *, double *, char *);

struct conversion getConversion(char *in_units, char *out_units);

void displayHelp();