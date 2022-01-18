#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256
#define MAX_SIZE 40
#define TABLE_SIZE 11

struct _CountryList;
struct _CityTree;
struct _hashTable;
typedef struct _CountryList* ListPos;
typedef struct _CountryList* List;
typedef struct _CityTree* StabloPos;
typedef struct _hashTable* hashTab;

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

typedef struct _hashTable {
	int TableSize;
	List* hashList;
}hashTable;

int UserInput(char* file);
int ReadFromFile(char* file, hashTab H);
int SortedInsert(hashTab H, char* country, char* cities);
ListPos createListEl(char* country);
int InsertAfter(ListPos head, ListPos newEl);
StabloPos AddCityToTree(char* CitiesFile, StabloPos root);
StabloPos Insert(StabloPos root, char* city, int population);
StabloPos createNewNode(char* city, int brojSt);
int PrintAll(hashTab H);
int InOrder(StabloPos root);
int PrintCities(StabloPos root, int PopulationInput);
ListPos SearchCountry(char* country, hashTab H);
int HashFunction(char* country, int TableSize);
hashTab CreateHashTab(int TableSize);


int main() {
	char file[MAX] = "";
	hashTab H= NULL;
	ListPos current = NULL;
	int popInput;
	H = CreateHashTab(TABLE_SIZE);
	printf("Unesite ime datoteke:");
	UserInput(file);
	ReadFromFile(file, H);
	PrintAll(H);

	char Input[MAX] = "";
	char country[MAX] = "";

	do {
		printf("Upisite ime drzave:\n");
		UserInput(country);
		current = SearchCountry(country, H);
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

int UserInput(char* str)
{
	fgets(str, MAX, stdin);
	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';
	return 0;
}

int ReadFromFile(char* file, hashTab H) {
	char buffer[MAX], drzava[MAX], gradoviFile[MAX];
	FILE* fp = NULL;
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
			SortedInsert(H, drzava, gradoviFile);
	}
	fclose(fp);
	return 0;
}
int SortedInsert(hashTab H, char* country, char* cities) {
	ListPos newElement;
	List A = NULL;
	int hashKey = HashFunction(country, H->TableSize);
	newElement = createListEl(country);
	if (newElement == NULL) {
		return -1;
	}
	newElement->root = AddCityToTree(cities, newElement->root);
	if (!H->hashList[hashKey]) {
		H->hashList[hashKey] = newElement;
	}
	else {
		A = H->hashList[hashKey];
		while (A->next != NULL && strcmp(A->next->CountryName, country) < 0)
			A = A->next;
		InsertAfter(A, newElement);
	}
	return 0;
}

ListPos createListEl(char* country)
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

StabloPos AddCityToTree(char* CitiesFile, StabloPos root)
{
	char buffer[MAX], city[MAX];
	int population;
	FILE* fp = NULL;
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

StabloPos Insert(StabloPos root, char* city, int population)
{
	StabloPos node = root;
	if (node == NULL) {
		StabloPos temp = NULL;
		temp = createNewNode(city, population);
		return temp;
	}
	else if (node->population < population) {
		node->left = Insert(node->left, city, population);
	}
	else if (node->population > population) {
		node->right = Insert(node->right, city, population);
	}
	else {
		printf("Vec imamo istu drzavu\n");
	}
	return root;
}

StabloPos createNewNode(char* city, int brojSt)
{
	StabloPos q = NULL;
	q = (StabloPos)malloc(sizeof(CityTree));
	if (!q) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	strcpy(q->CityName, city);
	q->population = brojSt;
	q->left = NULL;
	q->right = NULL;
	return q;
}
int PrintAll(hashTab H) {
	List A = NULL;
	for (int i = 0; i < H->TableSize; i++) {
		A = H->hashList[i];
		if (A != NULL) {
			printf("\nDrzava: %s\n", A->CountryName);
			printf("Grad - Broj Stanovnika:\n ");
			InOrder(A->root);
			A = A->next;
		}
	}
}

int InOrder(StabloPos root)
{
	StabloPos temp = root;
	if (temp != NULL) {
		InOrder(temp->left);
		printf(" %s %d\n", temp->CityName, temp->population);
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
			printf(" %s %d\n", temp->CityName, temp->population);
		PrintCities(temp->right, PopulationInput);
	}
	return 0;
}

ListPos SearchCountry(char* country, hashTab H)//
{
	List A = NULL;
	A = H->hashList[HashFunction(country, H->TableSize)];
	while (A != NULL) {
		if (strcmp(A->CountryName, country) == 0) {
			return A;
		}
		A = A->next;
	}
	if (A == NULL) {
		printf("Nije pronadena drzava %s\n", country);
	}
	return NULL;
}

hashTab CreateHashTab(int TableSize) {
	hashTab H;
	H = (hashTab)malloc(sizeof(hashTab));
	if (H == NULL) {
		printf("Can't allocate memory!");
		return NULL;
	}
	H->TableSize = TableSize;
	H->hashList = (ListPos*)malloc(sizeof(ListPos) * H->TableSize);
	if (H->hashList == NULL) {
		printf("Greska kod inicijalizacije memorije!");
		return NULL;
	}
	for (int i = 0; i < TableSize; i++) {
		H->hashList[i] = NULL;
	}
	return H;
}
int HashFunction(char* country, int TableSize) {
	int res = 0;
	for (int i = 0; i < 5; i++) {
		if (country[i] == '\0')
			break;
		res += country[i];
	}
	return res % TableSize;
}