#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256
#define MAX_SIZE 40

struct _CountryList;
struct _CityTree;
typedef struct _CountryList *ListPos;
typedef struct _CityTree *StabloPos;

typedef struct _CountryList
{
	char CountryName[MAX_SIZE];
	ListPos next;
	StabloPos root;
}CountryList;

typedef struct _CityTree
{
	int population;
	char CityName[MAX_SIZE];
	StabloPos left;
	StabloPos right;
}CityTree;

int UserInput(char *file);
int ReadFromFile(char *file, ListPos head);
int SortedInsert(ListPos head, char *drzava, char *gradovi);
ListPos createListEl(char *country);
int InsertAfter(ListPos head, ListPos newEl);
StabloPos AddCityToTree(char *CitiesFile, StabloPos root);
StabloPos Insert(StabloPos root, char *grad, int population);
StabloPos createNewNode(char *grad, int brojSt);
int PrintAll(ListPos head);
int InOrder(StabloPos root);
int PrintCities(StabloPos root, int PopulationInput);
ListPos SearchCountry(char *country, ListPos head);

int main() {
	char file[MAX] = "";
	CountryList Head = { .CountryName = "",.next = NULL,.root = NULL };
	printf("Unesite ime datoteke:");
	UserInput(file);
	ReadFromFile(file, &Head);
	PrintAll(&Head);

	char Input[MAX] = "";
	char country[MAX] = "";
	int popInput;
	ListPos current = NULL;

	do {
		printf("Upisite ime drzave:\n");
		UserInput(country);
		current = SearchCountry(country, &Head);
		if (current != NULL) {
			printf("Unesite zeljenu granicu broja stanovnika gradova:\n");
			scanf("%d", &popInput);
			getchar();
			printf("Gradovi sa  brojem stanovnika vecim od %d:\n", popInput);
			PrintCities(current->root, popInput);
		}
		printf("za izlaz exit\n");
		UserInput(Input);

	} while (strcmp(Input, "exit") != 0);
	return 0;
}

int UserInput(char *str)
{
	fgets(str, MAX, stdin);
	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';
	return 0;
}

int ReadFromFile(char *file, ListPos head)
{
	char buffer[MAX], drzava[MAX], gradoviFile[MAX];
	FILE *fp = NULL;
	int res = 0;
	fp = fopen(file, "r");
	if (fp == NULL)
	{
		perror("Can't open file!\n");
		return -1;
	}
	while (fgets(buffer, MAX, fp) != NULL)
	{
		res = sscanf(buffer, "%s %s", drzava, gradoviFile);
		if (res == 2)
			SortedInsert(head, drzava, gradoviFile);
	}
	fclose(fp);
	return 0;
}

int SortedInsert(ListPos head, char *drzava, char *gradoviFile)
{
	ListPos current = head;
	ListPos newEl = NULL;
	newEl = createListEl(drzava);
	if (newEl == NULL)
		return -1;
	newEl->root = AddCityToTree(gradoviFile, newEl->root);

	while (current->next != NULL && strcmp(current->next->CountryName, drzava) < 0)
		current = current->next;
	InsertAfter(current, newEl);
	return 0;
}

ListPos createListEl(char *country)
{
	ListPos q = NULL;
	q = (ListPos)malloc(sizeof(CountryList));
	if (!q) {
		perror("Couldn't allocate memory!\n");
		return NULL;
	}
	strcpy(q->CountryName, country);
	q->next = NULL;
	q->root = NULL;
	return q;
}

int InsertAfter(ListPos head, ListPos newEl)
{
	newEl->next = head->next;
	head->next = newEl;
	return 0;
}

StabloPos AddCityToTree(char *CitiesFile, StabloPos root)
{
	char buffer[MAX], city[MAX];
	int population;
	FILE *fp = NULL;
	fp = fopen(CitiesFile, "r");
	if (fp == NULL) {
		perror("Can't open file! \n");
		return NULL;
	}
	while (fgets(buffer, MAX, fp) != NULL)
	{
		sscanf(buffer, "%s %d", city, &population);
		root = Insert(root, city, population);
	}
	fclose(fp);
	return root;
}

StabloPos Insert(StabloPos root, char *grad, int population)
{
	StabloPos node = root;
	if (node == NULL) {
		StabloPos temp = NULL;
		temp = createNewNode(grad, population);
		return temp;
	}
	else if (node->population < population) {
		node->left = Insert(node->left, grad, population);
	}
	else if (node->population > population) {
		node->right = Insert(node->right, grad, population);
	}
	else {
		printf("Vec imamo istu drzavu\n");
	}
	return root;
}

StabloPos createNewNode(char *grad, int brojSt)
{
	StabloPos q = NULL;
	q = (StabloPos)malloc(sizeof(CityTree));
	if (!q) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	strcpy(q->CityName, grad);
	q->population = brojSt;
	q->left = NULL;
	q->right = NULL;
	return q;
}

int PrintAll(ListPos head)
{
	ListPos current = head->next;
	while (current != NULL)
	{
		printf("Drzava: %s\n", current->CountryName);
		printf(" Grad:                Broj Stanovnika:\n");
		InOrder(current->root);
		current = current->next;
	}
	return 0;
}

int InOrder(StabloPos root)
{
	StabloPos temp = root;
	if (temp != NULL) {
		InOrder(temp->left);
		printf(" %-20.20s %d\n", temp->CityName, temp->population);
		InOrder(temp->right);
	}
	return 0;
}

int PrintCities(StabloPos root, int PopulationInput)
{
	StabloPos temp = root;
	if (temp != NULL) {
		PrintCities(temp->left, PopulationInput);
		if (temp->population >= PopulationInput)
			printf(" %-20.20s %d\n", temp->CityName, temp->population);
		PrintCities(temp->right, PopulationInput);
	}
	return 0;
}

ListPos SearchCountry(char *country, ListPos head)
{
	ListPos current = head->next;
	while (current != NULL) {
		if (strcmp(current->CountryName, country) == 0) {
			return current;
		}
		current = current->next;
	}
	if (current == NULL) {
		printf("Nije pronadena drzava %s\n", country);
	}
	return NULL;
}