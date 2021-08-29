#include "seznam.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

// ! INDEXING for seznam -> from 0 to pocetKaret-1 !
// decks are linked lists -> each card points to next one


// create seznam
tSeznam * vytvorSeznam(char *nazevSeznamu) {
	tSeznam* seznam = (tSeznam*)malloc(sizeof(tSeznam));
	strcpy(seznam->nazevSeznamu, nazevSeznamu);
	seznam->prvni = NULL;
	seznam->pocetKaret = 0;

	return seznam;
}


// put on last position of deck
void vlozNaPosledniMisto(tSeznam * s, tKarta * k) {
	tKarta *curr = s->prvni;
	if (s->prvni == NULL) {
		s->prvni = k;
	}
	else {
		while (curr->dalsi) {
			curr = curr->dalsi;
		}

		curr->dalsi = k;
	}
	s->pocetKaret++;
}

// return card at certain position (OFFSET), but don't remove it from deck
tKarta *zpristupniDlePoradi(tSeznam *s, int offset) { // OFFSET : from 0 to pocetKaret-1
	tKarta *curr = s->prvni;
	int i = 0;
	if (offset == 0) return zpristupniPrvniKartu(s); // if OFFSET is 0, use function for returning first card
	else if (offset <= s->pocetKaret - 1) {
		for (i = 0; i < offset; i++) {
			curr = curr->dalsi;
		}
		return curr;
	}
	else return NULL;

}

// return card at certain position (OFFSET) and remove it from deck
tKarta *odeberDlePoradi(tSeznam *s, int offset) {
	tKarta *curr = s->prvni;
	int i = 0;
	if (offset == 0) return odeberPrvniKartu(s); // if OFFSET is 0, use function for returning first card
	else if (offset <= s->pocetKaret - 1) {
		for (i = 0; i < offset - 1; i++) {
			curr = curr->dalsi;
		}
		tKarta *chosen = curr->dalsi;
		curr->dalsi = chosen->dalsi;
		chosen->dalsi = NULL;
		s->pocetKaret--;
		return chosen;
	}
	else return NULL;
}

// return last card, but don't remove it from deck
tKarta *zpristupniPosledniKartu(tSeznam *s) {
	tKarta *curr = s->prvni;
	while (curr->dalsi) {
		curr = curr->dalsi;
	}
	return curr;
}

// return last card and remove it from deck
tKarta *odeberPosledniKartu(tSeznam *s) {
	tKarta *curr = s->prvni;
	tKarta *pre = NULL;
	int i = 0;
	while (curr->dalsi) {
		pre = curr;
		curr = curr->dalsi;
	}
	pre->dalsi = NULL;
	s->pocetKaret--;
	return curr;

}

// return first card, but don't remove it from deck
tKarta *zpristupniPrvniKartu(tSeznam *s) {
	return s->prvni;
}


// return first card and remove it from deck
tKarta *odeberPrvniKartu(tSeznam *s) {
	tKarta *first = s->prvni;
	tKarta *second = first->dalsi;
	s->prvni = second;
	first->dalsi = NULL;
	s->pocetKaret--;
	return first;


}

// return number of cards in deck
int dejPocetKaret(tSeznam *s) {
	return s->pocetKaret;
}

// SHUFFLE deck
// create new tmp deck
// take cards at rand position from original deck -> put it at last position of tmp deck
// tmp deck contains shuffled cards 
// -> return them to original deck (put them back one by one at last position of deck)
void zamichat(tSeznam *s) {
	tSeznam *tmp = vytvorSeznam("tmp");
	int index = 0;
	tKarta *tmpK = NULL;
	while (s->pocetKaret != 1) { // leave only one card
		index = rand() % (s->pocetKaret - 1); // we can use rand for max 2 cards left
		tmpK = odeberDlePoradi(s, index); // take card at rand position
		vlozNaPosledniMisto(tmp, tmpK); // put it at last spot in tmp deck
	}
	// last card left -> can't use rand for only one card so we need to do it separately
	tmpK = odeberPrvniKartu(s);
	vlozNaPosledniMisto(tmp, tmpK);

	// shuffled tmp -> put card back to s (original deck)
	while (tmp->pocetKaret != 0) {
		tmpK = odeberPrvniKartu(tmp);
		vlozNaPosledniMisto(s, tmpK);
	}
}

// check if deck contains certain card
int jeVSeznamu(tSeznam *s, tKarta *k) {
	int find = 0;

	tKarta *curr = s->prvni;
	while (curr->dalsi) {
		if (strcmp(k->nazev, curr->nazev) == 0) {
			find++;
		}
	}

	return find;

}

// finds index of card in deck
// if card is not in deck, returns -1
int dejPoradiVSeznamu(tSeznam *s, tKarta *k) {
	int index = 0;

	tKarta *curr = s->prvni;
	while (curr->dalsi) {
		index++;
		if (strcmp(k->nazev, curr->nazev) == 0) {
			return index;
		}
	}

	return (-1);

}

// removes all cards from deck
void vyprazdnit(tSeznam *s) {
	tKarta *curr = s->prvni;
	tKarta *prev = NULL;
	while (curr->dalsi) {
		prev = curr;
		curr = curr->dalsi;
		free(prev);
	}
	free(curr);
	s->pocetKaret = 0;

	/*
	tKarta *curr = s->prvni;
	tKarta *help = NULL;
	while(curr->dalsi){
	help = curr;
	curr = curr->dalsi;
	help->dalsi = NULL;
	}
	s->pocetKaret = 0;
	*/
}

// prints all cards from deck, and total number of cards
void vypisSeznam(tSeznam *s) {
	tKarta *curr = s->prvni;
	printf("\nDECK: %s\n", s->nazevSeznamu);
	printf("Number of cards: %d\n", s->pocetKaret);
	while (curr->dalsi) {
		vypisKartu(curr);
		curr = curr->dalsi;
	}
	vypisKartu(curr);
	printf("\n");
}
