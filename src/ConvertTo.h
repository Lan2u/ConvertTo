/*
Usage: ConvertTo <Quantity> <unit> <output unit>
	- Gives the given quantity in the given units in the output units
Usage: ConvertTo <Quantity> <Unit>
	- Gives the given quantity in a selection of units (preset)
*/

#define CONVERSIONS_FILE_PATH "../data/conversions"

struct conversion {
    float factor; // unit2 = unit1 * factor + offset. The conversion from 2 to 1 is therefore (1/conversion)
    float offset; // unit2 = unit1 * factor + offset. The conversion from 2 to 1 is therefore -offset
};

int convert(char *, char *, double *, char *);

// Read from file until a conversion is found
// Return the conversion factor if it is found as in_unit to out_unit or (1/conversion_factor) if it is found as out_unit to in_unit
// Eg. kg g 1000
// If you were going from kg to g then 1000 is returned. If you are going from g to kg then 1/1000 = 0.001 is returned.
struct conversion getConversion(char *in_units, char *out_units);

void displayHelp();