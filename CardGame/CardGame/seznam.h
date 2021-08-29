#include <stdlib.h>
#include "karta.h"

typedef struct seznam {
	char nazevSeznamu[50];//
	tKarta *prvni;
	int pocetKaret;
} tSeznam;

tSeznam *vytvorSeznam(char *nazevSeznamu); // vrací adresu dynamické alokace prázdného seznamu

void vlozNaPosledniMisto(tSeznam * s, tKarta * k);
// Použití: Na zaèátku hry naplnit nový balíèek, èi pøi naètení ze souboru
// Použití: Vložit do seznamu VyloženéKarty

tKarta *zpristupniDlePoradi(tSeznam *s, int offset);
// Použití: Práce s kartami hráèe a poèítaèe

tKarta *odeberDlePoradi(tSeznam *s, int offset);
// Použití: Vyložení karty hráèe èi uživatele

tKarta *zpristupniPosledniKartu(tSeznam *s);

tKarta *odeberPosledniKartu(tSeznam *s);

tKarta *zpristupniPrvniKartu(tSeznam *s);

tKarta *odeberPrvniKartu(tSeznam *s);

int dejPocetKaret(tSeznam *s);
// Vrátí poèet karet v seznamu
// Použití: Kontrola vítìzství, pomocná pro vykreslení karet 

void zamichat(tSeznam *s);
// Použití: Pøed zaèátkem hry

int jeVSeznamu(tSeznam *s, tKarta *k);
// Použití: Kontrola proti podvádìní, pøi naètení z CSV

int dejPoradiVSeznamu(tSeznam *s, tKarta *k);
// Vrátí poøadí karty v seznamu

void vyprazdnit(tSeznam *s);
// Vyprázdní seznam

void vypisSeznam(tSeznam *s);
// Vykreslí na stdout karty