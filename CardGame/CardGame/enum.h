
typedef enum {
	cervena = 0,
	zelena,
	kule,
	zaludy
} tBarvaKarty;

typedef enum {
	K7 = 7,  //number seven
	K8,  	 //number eight
	K9,      //number nine
	K10,     //number ten
	SP,      //spodek
	SV,      //svrsek
	KR,      //king
	ESO      //eso
} tTypKarty;



tBarvaKarty findBarva(char *barva);

tTypKarty findTyp(char *typ);

char *findSBarva(tBarvaKarty barva);

char *findSTyp(tTypKarty typ);
