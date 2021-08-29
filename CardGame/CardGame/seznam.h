#include <stdlib.h>
#include "karta.h"

typedef struct seznam {
	char nazevSeznamu[50];//
	tKarta *prvni;
	int pocetKaret;
} tSeznam;

tSeznam *vytvorSeznam(char *nazevSeznamu); // vrac� adresu dynamick� alokace pr�zdn�ho seznamu

void vlozNaPosledniMisto(tSeznam * s, tKarta * k);
// Pou�it�: Na za��tku hry naplnit nov� bal��ek, �i p�i na�ten� ze souboru
// Pou�it�: Vlo�it do seznamu Vylo�en�Karty

tKarta *zpristupniDlePoradi(tSeznam *s, int offset);
// Pou�it�: Pr�ce s kartami hr��e a po��ta�e

tKarta *odeberDlePoradi(tSeznam *s, int offset);
// Pou�it�: Vylo�en� karty hr��e �i u�ivatele

tKarta *zpristupniPosledniKartu(tSeznam *s);

tKarta *odeberPosledniKartu(tSeznam *s);

tKarta *zpristupniPrvniKartu(tSeznam *s);

tKarta *odeberPrvniKartu(tSeznam *s);

int dejPocetKaret(tSeznam *s);
// Vr�t� po�et karet v seznamu
// Pou�it�: Kontrola v�t�zstv�, pomocn� pro vykreslen� karet 

void zamichat(tSeznam *s);
// Pou�it�: P�ed za��tkem hry

int jeVSeznamu(tSeznam *s, tKarta *k);
// Pou�it�: Kontrola proti podv�d�n�, p�i na�ten� z CSV

int dejPoradiVSeznamu(tSeznam *s, tKarta *k);
// Vr�t� po�ad� karty v seznamu

void vyprazdnit(tSeznam *s);
// Vypr�zdn� seznam

void vypisSeznam(tSeznam *s);
// Vykresl� na stdout karty