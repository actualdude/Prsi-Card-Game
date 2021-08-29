#include <stdio.h>	
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "seznam.h"


tSeznam *kartyVBalicku;     // undivided cards
tSeznam *vylozeneKarty;     // played cards
tSeznam *kartyHrace;        // player's cards
tSeznam *kartyPocitace;     // PC's cards

tBarvaKarty barvaCh = cervena;  // global var for card's color -> used when svrsek is played
int count7 = 0;                 // counts played SEVEN -> used to calculate how much cards should be taken from deck
int whoPlays = 0;               // O = PLAYER, 1 = PC -> used to change who is playing
int waitEso = 0;                // = 1 -> used to wait a turn without taking new card


								// helping function for splitting line by ; -> num decides which part to return
								// example: line = aa;bb;cc;dd 
								// getfield(line,2) = bb
const char* getfield(char* line, int num)
{
	const char* tok;
	for (tok = strtok(line, ";");
		tok && *tok;
		tok = strtok(NULL, ";\n"))
	{
		if (!--num)
			return tok;
	}
	return NULL;
}

// save to CSV file
void saveToCSV(tSeznam *s) {
	FILE *f = fopen("played.csv", "a"); // a is for appending to existing content of file
	tKarta *karta = NULL;
	for (int i = 0; i < s->pocetKaret; i++) {
		karta = zpristupniDlePoradi(s, i);
		fprintf(f, "%s;%s;%s;%s\n", s->nazevSeznamu, findSTyp(karta->typKarty), findSBarva(karta->barvaKarty), karta->nazev);
	}
	fclose(f);
}


// read default.csv file and create deck
// used for new game
void createSeznamFromFile(const char *filename) {
	FILE* stream = fopen(filename, "r");

	char nazevSeznamu[50];
	tBarvaKarty barvaKarty;
	tTypKarty typKarty;
	char nazev[50];// "Lidský název" karty pro výpisy, pø.: "Èervené eso"
	char typ[50] = "";
	char barva[50] = "";

	int i = 0;
	char line[1024];
	fgets(line, 1024, stream); // reads line by line, uses getfield to split lines
	int lineNum = 32;
	while (lineNum)
	{
		fgets(line, 1024, stream); // reads a line
		lineNum--;
		char* tmp1 = strdup(line); // takes a copy of line
		char* tmp2 = strdup(line);
		char* tmp3 = strdup(line);
		char* tmp4 = strdup(line);

		// split line and save to vars
		strcpy(nazevSeznamu, getfield(tmp1, 1));
		strcpy(typ, getfield(tmp2, 2));
		strcpy(barva, getfield(tmp3, 3));
		strcpy(nazev, getfield(tmp4, 4));

		// line contains strings, we need ints for creating enums
		// functions return enum from string names of typ and barva
		barvaKarty = findBarva(barva);
		typKarty = findTyp(typ);

		// create card from each line
		tKarta *karta = vytvorKartu(barvaKarty, typKarty, nazev);

		// find in which deck we should put that card
		// put it on last position
		if (strcmp(kartyVBalicku->nazevSeznamu, nazevSeznamu) == 0) {
			vlozNaPosledniMisto(kartyVBalicku, karta);
		}
		else if (strcmp(vylozeneKarty->nazevSeznamu, nazevSeznamu) == 0) {
			vlozNaPosledniMisto(vylozeneKarty, karta);
		}
		else if (strcmp(kartyHrace->nazevSeznamu, nazevSeznamu) == 0) {
			vlozNaPosledniMisto(kartyHrace, karta);
		}
		else if (strcmp(kartyPocitace->nazevSeznamu, nazevSeznamu) == 0) {
			vlozNaPosledniMisto(kartyPocitace, karta);
		}

		free(tmp1);
		free(tmp2);
		free(tmp3);
		free(tmp4);
	}
}



// check if move is valid
int checkValid(tKarta *topKarta, tKarta *kartaToPlay) {
	int valid = 0;
	// ESO is played
	if (topKarta->typKarty == ESO && waitEso == 1) { // if ESO is played, only valid move is to play ESO
		if (kartaToPlay->typKarty == ESO) valid = 1;
	}
	// K7 is played
	else if (topKarta->typKarty == K7 && count7 != 0) { // only valid move is to play 7
		if (kartaToPlay->typKarty == K7) valid = 1;
	}
	// SV is played
	else if (topKarta->typKarty == SV && vylozeneKarty->pocetKaret != 1) { // valid move is to play with chosen color or another SV
		if (kartaToPlay->barvaKarty == barvaCh || kartaToPlay->typKarty == SV) valid = 1;
	}
	else { // everything else: valid move is same type or same color
		if (topKarta->barvaKarty == kartaToPlay->barvaKarty || topKarta->typKarty == kartaToPlay->typKarty || kartaToPlay->typKarty == SV)
			valid = 1;
	}
	return valid;
}

// used before taking new cards from deck
// calculate how much cards should be taken from deck
int checkHowMany(tKarta *topKarta) {
	// ESO
	if (topKarta->typKarty == ESO && waitEso == 1) { // ESO is played -> just skip turn
		return 0;
	}
	// K7
	else if (topKarta->typKarty == K7 && count7 != 0) { // take +2 for each 7
		return 2 * count7;
	}
	else return 1; // default: take one
}

// check if there is no cards left in deck for dividing
// if all cards are used, take played cards (vylozeneKarty) and put them to kartyVBalicku
// shuffle that deck
void checkForLast() {
	if (kartyVBalicku->pocetKaret == 0) {
		// from table to balicet
		tKarta *karta = NULL;
		int count = dejPocetKaret(vylozeneKarty);
		for (int i = 0; i < count - 1; i++) {
			karta = odeberPrvniKartu(vylozeneKarty);
			vlozNaPosledniMisto(kartyVBalicku, karta);
		}
		zamichat(kartyVBalicku);
	}
}

// GAME LOGIC
void playTheGame(int turn) {
	tKarta *topKarta = NULL; // last played card -> card on top of vylozeneKarty

							 /// *** PLAYER *** ///
	if (turn == 0) {
		printf("\nNOW PLAYING: - YOU - \n");
		tKarta *kartaToPlay = NULL; // card we want to play
		topKarta = zpristupniPosledniKartu(vylozeneKarty);
		int valid = 0; // flag is set if we made valid move, if not repeat the option menu
		while (valid == 0) {

			printf("\nCARD ON TOP: --> %s <-- \n\n", topKarta->nazev);
			printf("--- OPTIONS: ---\n\n");

			for (int i = 0; i < kartyHrace->pocetKaret; i++) {
				printf("%d) ", i + 1);
				vypisKartu(zpristupniDlePoradi(kartyHrace, i));
			}
			printf("\n");

			if (topKarta->typKarty == ESO && waitEso == 1) printf("A) Wait one turn\n"); // if eso is on top, wait one turn instead of taking new card
			else printf("A) Take new card\n");
			printf("B) Exit\n");
			printf("C) Save game\n");
			printf("D) DEV option\n\n");

			printf("Choose option:\n");
			char dec;
			scanf(" %c", &dec);
			if (isdigit(dec)) {
				int decInt = dec - '0'; // dec is char, we need int -> conversion from char to int
				kartaToPlay = zpristupniDlePoradi(kartyHrace, decInt - 1); // -1 because options starts at 1, and cards in deck starts at 0
				vypisKartu(kartaToPlay);
				valid = checkValid(topKarta, kartaToPlay); // check if we can play chosen card
														   // play that karta, check for victory
				if (valid == 1) {
					kartaToPlay = odeberDlePoradi(kartyHrace, decInt - 1);
					vlozNaPosledniMisto(vylozeneKarty, kartaToPlay);
					if (dejPocetKaret(kartyHrace) == 0) { // WIN -> if last card is played
						printf("\n\n !!! GAME OVER !!!\n\n");
						printf(" ");
						printf("\n\n !!! Y O U   W O N !!!\n\n");
						system("pause");
						exit(0);
					}
					else {
						// SV
						if (kartaToPlay->typKarty == SV) {
							printf("Choose a color:\n");
							printf("1) CERVENA\n");
							printf("2) ZELENA\n");
							printf("3) KULE\n");
							printf("4) ZALUDY\n");
							int color = 0;
							scanf("%d", &color);
							barvaCh = color - 1; // set global card color (-1 because enums start at 0)
						}
						// K7
						else if (kartaToPlay->typKarty == K7) {
							count7++; // 7 is played, increment global var 
						}
						//ESO
						else if (kartaToPlay->typKarty == ESO) {
							waitEso = 1; // eso is played, set that next player should wait a turn
						}
					}
				}


			}
			else if (isalpha(dec)) {
				// TAKE A NEW CARD
				if (tolower(dec) == 'a') {
					valid = 1;
					int count = checkHowMany(topKarta);
					// new card is taken, reset all the count flags
					if (topKarta->typKarty == K7) count7 = 0;
					if (topKarta->typKarty == ESO) waitEso = 0;
					tKarta *newKarta = NULL;
					for (int i = 0; i < count; i++) {
						newKarta = odeberPrvniKartu(kartyVBalicku);
						vlozNaPosledniMisto(kartyHrace, newKarta);
						// check if that is last card in balicet
						checkForLast();
					}
				}
				// EXIT
				else if (tolower(dec) == 'b') {
					system("pause");
					exit(0);
				}
				// SAVE GAME
				else if (tolower(dec) == 'c') {
					// save to csv all balicek;
					FILE *f = fopen("played.csv", "w"); // w is for deleting content of file and writing to it (but not appending)
														// save first line
					fprintf(f, "balicek;typ;barva;popisKarty\n"); // default - that should be as first line
					fclose(f);

					// save played cards from each deck
					saveToCSV(kartyVBalicku);
					saveToCSV(kartyHrace);
					saveToCSV(kartyPocitace);
					saveToCSV(vylozeneKarty);

					exit(0);
				}
				// DEV OPTION - prints all decks, all cards and total number of cards
				else if (tolower(dec) == 'd') {
					printf("ALL DECKS: \n");
					vypisSeznam(kartyVBalicku);
					vypisSeznam(kartyHrace);
					vypisSeznam(kartyPocitace);
					vypisSeznam(vylozeneKarty);
					printf("\n\n\n");
				}

			}



		}


	}
	/// *** PC *** /// 
	// PC LOGIC - checks cards one by one, and plays first valid one
	else {
		printf("\nNOW PLAYING: - PC - \n");
		int findValid = 0; // find valid move
		int numCard = kartyPocitace->pocetKaret;
		tKarta *kartaPC = NULL; // card which PC wants to play
		topKarta = zpristupniPosledniKartu(vylozeneKarty);
		for (int i = 0; i < numCard; i++) { // check all cards one by one to find valid one
			kartaPC = zpristupniDlePoradi(kartyPocitace, i); // take card at certain position
			findValid = checkValid(topKarta, kartaPC); // check if it is valid
			if (findValid == 1) {
				kartaPC = odeberDlePoradi(kartyPocitace, i); // choose that card and play it
				printf("PLAYED: PC is playing card -> ");
				vypisKartu(kartaPC);
				vlozNaPosledniMisto(vylozeneKarty, kartaPC);
				if (dejPocetKaret(kartyPocitace) == 0) { // WIN
					printf("\n\n !!! G A M E   O V E R !!!\n\n");
					printf(" ");
					printf("\n\n !!! PC   W O N !!!\n\n");
					system("pause");
					exit(0);
				}
				else {
					// SV
					if (kartaPC->typKarty == SV) {
						barvaCh = rand() % 4; // choose new color randomly
						char nColor[15];
						if (barvaCh == 0) strcpy(nColor, "cervena");
						else if (barvaCh == 1) strcpy(nColor, "zelena");
						else if (barvaCh == 2) strcpy(nColor, "kule");
						else strcpy(nColor, "zaludy");
						printf("! SVRSEK IS PLAYED !        NEW Color = %s", nColor);
					}
					// K7
					else if (kartaPC->typKarty == K7) {
						count7++; // set count 7 variable
					}
					else if (kartaPC->typKarty == ESO) {
						waitEso = 1; // set that other player should wait a turn
					}
				}
				break;
			}
		}

		// if there's no valid cards, take a new one
		if (findValid == 0) {
			printf("PLAYED: PC is taking cards from deck\n");
			int count = checkHowMany(topKarta); // find how much cards PC should take
												// reset all counting flags
			if (topKarta->typKarty == K7) count7 = 0;
			if (topKarta->typKarty == ESO) waitEso = 0;
			tKarta *newKarta = NULL;
			// put new cards in deck
			for (int i = 0; i < count; i++) {
				newKarta = odeberPrvniKartu(kartyVBalicku);
				vlozNaPosledniMisto(kartyPocitace, newKarta);
				// check if that is last card in balicet 
				checkForLast();
			}

		}


	}


}

int main() {

	printf("\n\n********************* - Welcome to Prsi - ********************\n\n\n");
	srand(time(NULL)); // for using rand() function

					   // create decks
	kartyVBalicku = vytvorSeznam("balicet");
	vylozeneKarty = vytvorSeznam("vylozeno");
	kartyHrace = vytvorSeznam("hrac");
	kartyPocitace = vytvorSeznam("pocitac");

	// Menu
	printf("------------ G A M E   M E N U -------------\n\n");
	printf("New game - press A\n");
	printf("Load saved game - press B:\n");
	printf("Exit - press C:\n");
	char decision;
	scanf("%c", &decision);

	if (tolower(decision) == 'a') { // new game
									// create package balicet
		createSeznamFromFile("default.csv");

		// shuffle
		zamichat(kartyVBalicku);

		// divide 4 to player and pc, 1 on the table

		// to player and pc
		tKarta* karta = NULL;
		for (int i = 0; i < 4; i++) {
			karta = odeberPrvniKartu(kartyVBalicku);
			vlozNaPosledniMisto(kartyHrace, karta);
			karta = odeberPrvniKartu(kartyVBalicku);
			vlozNaPosledniMisto(kartyPocitace, karta);
		}
		// on the table
		karta = odeberPrvniKartu(kartyVBalicku);
		if (karta->typKarty == K7) count7++;
		vlozNaPosledniMisto(vylozeneKarty, karta);

		// choose randomly who plays first
		whoPlays = rand() % 2;

		while (1) { // change turns until someone wins
			playTheGame(whoPlays);
			if (whoPlays == 0) whoPlays = 1;
			else whoPlays = 0;

		}


	}
	// LOAD GAME
	else if (tolower(decision) == 'b') {

		createSeznamFromFile("played.csv");

		int whoPlays = 0; // player's turn

		while (1) {
			playTheGame(whoPlays);
			if (whoPlays == 0) whoPlays = 1;
			else whoPlays = 0;

		}



	}
	// EXIT
	else if (tolower(decision) == 'c') {
		system("pause");
		exit(0);
	}



	return 0;
	system("pause");
}