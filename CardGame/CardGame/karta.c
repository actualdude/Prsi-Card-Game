#include <stdlib.h>	
#include <string.h>
#include <stdio.h>
#include "karta.h"


tKarta * vytvorKartu(tBarvaKarty barvaKarty, tTypKarty typKarty, char *nazev) {
	tKarta* karta = (tKarta*)malloc(sizeof(tKarta));
	karta->barvaKarty = barvaKarty;
	karta->typKarty = typKarty;
	strcpy(karta->nazev, nazev);
	karta->dalsi = NULL;

	return karta;
}

void vypisKartu(tKarta *karta) {
	printf("%s\n", karta->nazev);

}