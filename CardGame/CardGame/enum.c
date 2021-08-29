#include "enum.h"
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>

/** helping functions for getting enum **/

// return enum from barva string
tBarvaKarty findBarva(char *barva) {
	if (strcmp(barva, "cervena") == 0) return cervena;
	if (strcmp(barva, "zelena") == 0) return zelena;
	if (strcmp(barva, "kule") == 0) return kule;
	if (strcmp(barva, "zaludy") == 0) return zaludy;
	return cervena;
}

// return enum from typ string
tTypKarty findTyp(char *typ) {
	if (strcmp(typ, "K7") == 0) return K7;
	if (strcmp(typ, "K8") == 0) return K8;
	if (strcmp(typ, "K9") == 0) return K9;
	if (strcmp(typ, "K10") == 0) return K10;
	if (strcmp(typ, "SP") == 0) return SP;
	if (strcmp(typ, "SV") == 0) return SV;
	if (strcmp(typ, "KR") == 0) return KR;
	if (strcmp(typ, "ESO") == 0) return ESO;
	return K7;
}

/** enums have int values, so this functions return string names for them**/

// return string for enum name of barva
char *findSBarva(tBarvaKarty barva) {
	if (barva == 0) return "cervena";
	if (barva == 1) return "zelena";
	if (barva == 2) return "kule";
	if (barva == 3) return "zaludy";
	return "cervena";
}

// return string for enum name of typ
char *findSTyp(tTypKarty typ) {
	if (typ == 7) return "K7";
	if (typ == 8) return "K8";
	if (typ == 9) return "K9";
	if (typ == 10) return "K10";
	if (typ == 11) return "SP";
	if (typ == 12) return "SV";
	if (typ == 13) return "KR";
	if (typ == 14) return "ESO";
	return "K7";
}
