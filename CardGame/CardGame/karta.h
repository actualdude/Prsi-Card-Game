#include <stdlib.h>
#include <time.h>
#include "enum.h"

typedef struct karta {
	tBarvaKarty barvaKarty;
	tTypKarty typKarty;
	char nazev[50];// "Lidsk� n�zev" karty pro v�pisy, p�.: "�erven� eso"
	struct karta *dalsi;
} tKarta;

tKarta * vytvorKartu(tBarvaKarty barvaKarty, tTypKarty typKarty, char *nazev); // vrac� adresu dynamick� alokace karty, ukazatel dalsi se nastav� na NULL
void vypisKartu(tKarta *karta); //vyp�e kartu na obrazovku
