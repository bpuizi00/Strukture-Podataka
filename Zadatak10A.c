#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SIZE 50
#define MAX_LINE 1028

struct _CityTree;
typedef struct _CityTree* CityPosition;
typedef struct _CityTree {
	char city[50];
	int population;
	CityPosition left;
	CityPosition right;
}CityTree;

struct _CountryList;
typedef struct _CountryList* CountryPosition;
typedef struct _CountryList {
	char country[50];
	CountryPosition next;
	CountryPosition root;
}CountryList;

CityPosition NewCity(char* ime, int broj);
CountryPosition NewCountry(char* ime);
int InsertAfter(CountryPosition position, CountryPosition newPerson);
int Sort(CountryPosition first, CountryPosition newperson);
CityPosition insert(CityPosition current, CityPosition q);
int CompareCities(CityPosition current, CityPosition q);
int ReadFromFile(CountryPosition p, char* imedatoteke);
int Unos(CountryPosition new, char* drzavadat);
int PrintAll(CountryPosition pp);
int PrintCity(CityPosition p);
int FindLargerCity(CityPosition p, int PopulationSearch);
CountryPosition FindCountry(CountryPosition p, char* name);

int main()
{
	CountryList HeadEl = { .country = { 0 },.next = NULL,.root = NULL };
	CountryPosition pp = &HeadEl;
	char CountryName[30];
	int PopulationSearch = 0;
	CountryPosition p;

	char file[MAX_SIZE];
	strcpy(file, "drzava.txt");
	ReadFromFile(pp, file);
	PrintAll(pp->next);

	printf("\nUnesite drzave koju zelite pretrazit: \n");
	scanf("%s", CountryName);
	p = FindCountry(pp->next, CountryName);
	if (p == NULL)
	{
		printf("Nema trazene drzave!\n");
		return 0;
	}
	printf("Prag stanovnika: ");
	scanf("%d", &PopulationSearch);
	printf("\nGradovi te drzave sa vecim brojem stanovnika od %d:\n", PopulationSearch);
	FindLargerCity(pp->root, PopulationSearch);

	return 0;
}

int ReadFromFile(CountryPosition p, char* imedatoteke)
{
	FILE* file = NULL;
	char CountryName[MAX_SIZE];
	char drzavadat[MAX_SIZE];
	CountryPosition new;
	file = fopen(imedatoteke, "r");
	if (!file) {
		perror("Couldn't open file! \n");
		return EXIT_FAILURE;
	}
	while (!feof(file)) {
		fscanf(file, " %s %s", CountryName, drzavadat);
		new = NewCountry(CountryName);
		Unos(new, drzavadat);
		Sort(p, new);
	}
	fclose(file);
	return EXIT_SUCCESS;
}

CountryPosition NewCountry(char* ime)
{
	CountryPosition newEL = NULL;
	newEL = (CountryPosition)malloc(sizeof(CountryList));

	if (!newEL) {
		perror("Can't allocate memory!\n");
		return 0;
	}

	strcpy(newEL->country, ime);
	newEL->next = NULL;
	newEL->root = NULL;
	return newEL;
}

int Unos(CountryPosition new, char* drzavadat) {

	FILE* file = NULL;
	char cityname[MAX_SIZE];
	int population;
	CityPosition noviGrad;

	file = fopen(drzavadat, "r");
	if (!file) {
		perror("Could not open file! \n");
		return EXIT_FAILURE;
	}
	while (!feof(file)) {
		fscanf(file, " %s %d", cityname, &population);
		noviGrad = NewCity(cityname, population);
		insert(new->root, noviGrad);
	}
	fclose(file);
	return EXIT_SUCCESS;
}
CityPosition NewCity(char* ime, int broj)
{
	CityPosition newElement = NULL;
	newElement = (CityPosition)malloc(sizeof(CityTree));
	if (!newElement)
	{
		printf("Can't allocate memory");
		return NULL;
	}
	strcpy(newElement->city, ime);
	newElement->population = broj;
	newElement->left = NULL;
	newElement->right = NULL;

	return newElement;
}
int InsertAfter(CountryPosition position, CountryPosition newPerson)
{
	newPerson->next = position->next;
	position->next = newPerson;
	return EXIT_SUCCESS;
}
CityPosition insert(CityPosition current, CityPosition q)
{
	if (current == NULL)
		return q;
	else if (current->population < q->population)
		current->right = insert(current->right, q);
	else if (current->population > q->population)
		current->left = insert(current->left, q);
	else if (current->population == q->population)
		CompareCities(current, q);
	else
		free(q);
	return current;
}

int CompareCities(CityPosition current, CityPosition q)
{
	if (strcmp(current->city, q->city) > 0) {
		current->left = insert(current->left, q);
	}
	if (strcmp(current->city, q->city) < 0) {
		current->right = insert(current->right, q);
	}
	return EXIT_SUCCESS;
}

int Sort(CountryPosition first, CountryPosition newperson)
{
	CountryPosition temp = first->next;
	while (strcmp(newperson->country, temp->next->country) > 0)
	{
		temp = temp->next;
	}
	InsertAfter(temp, newperson);
	return EXIT_SUCCESS;
}

int PrintAll(CountryPosition pp)
{
	printf("Drzave i gradovi u datoteci :\n");
	while (pp != NULL)
	{
		printf("  %s\n", pp->country);
		PrintCity(pp->root);
		pp = pp->next;
	}
	return 0;
}

int PrintCity(CityPosition p)
{
	if (p == NULL)
		return 0;
	PrintCity(p->left);
	printf("%s\n", p->city);
	PrintCity(p->right);
	return 0;
}

int FindLargerCity(CityPosition p, int PopulationSearch)
{
	if (p == NULL)
		return 0;
	FindLargerCity(p->left, PopulationSearch);
	if (p->population >= PopulationSearch)
		printf(" %s %d\n", p->city, p->population);
	FindLargerCity(p->right, PopulationSearch);
	return 0;
}

CountryPosition FindCountry(CountryPosition p, char* name)
{
	while (p != NULL)
	{
		if (strcmp(p->country, name) == 0)
			return p;
		p = p->next;
	}
	return p;
}