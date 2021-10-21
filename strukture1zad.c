#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE (128)
#define MAX_LINE (1024)

typedef struct _student {
	char ime[MAX_SIZE];
	char prezime[MAX_SIZE];
	double bodovi;
}student;

int ProcitajBrojRedakaizDatoteke(char* nazivDatoteke);
student* AlocirajMemorijuIUcitajStudente(int brojStudenata, char* nazivDatoteke);
int maxBod(student *studenti, int brojStudenata);

int main()
{
	int brRedaka = 0, i = 0, maxBodovi;
	student *studenti;
	char *nazivDat = "lista.txt";
	double relBod = 0;
	brRedaka = ProcitajBrojRedakaizDatoteke(nazivDat);
	studenti = AlocirajMemorijuIUcitajStudente(nazivDat, brRedaka);
	maxBodovi = maxBod(studenti, brRedaka);
	printf("Ime \t Prezime \t APSOLUTNI BODOVI \t RELATIVNI BODOVI \n");
	for (i = 0; i < brRedaka; i++)
	{
		relBod = 100 * studenti[i].bodovi / (double)maxBodovi;
		printf(" %s %s %d %.0lf \n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relBod);
		
	}
	return 0;
}

int ProcitajBrojRedakaizDatoteke(char* nazivDatoteke) {
	int brojac = 0;
	FILE* datoteka = NULL;
	char buffer[MAX_LINE] = { 0 };

	datoteka = fopen(nazivDatoteke, "r");
	if (!datoteka) {
		printf("Greska pri otvaranju datoteke! \n");
		return -1;
	}
	while (!feof(datoteka))
	{
		fgets(buffer, MAX_LINE, datoteka);
		brojac++;
	}

	fclose(datoteka);
	return brojac;
}

student* AlocirajMemorijuIUcitajStudente(int brojStudenata, char* nazivDatoteke)
{
	int brojac = 0;
	FILE* datoteka = NULL;
	student* studenti = NULL;

	studenti = (student*)malloc(brojStudenata * sizeof(student));
	if (!studenti) {
		printf("Neuspjesna alokacija memorije.\n");
		return NULL;
	}
	datoteka = fopen(nazivDatoteke, "r");
	if (!datoteka) {
		printf("Greska pri otvaranju datoteke!");
		return NULL;
	}
	while (!feof(datoteka))
	{
		fscanf(datoteka, " %s %s %lf", studenti[brojac].ime, studenti[brojac].prezime, &studenti[brojac].bodovi);
		brojac++;
	}
	fclose(datoteka);
	return studenti;
}

int maxBod(student *studenti, int brojStudenata)
{
	int maxBod=0;
	for (int i = 0; i < brojStudenata; i++)
	{
		if (studenti[i].bodovi > maxBod) {
			maxBod = studenti[i].bodovi;
		}
	}
	return maxBod;
}