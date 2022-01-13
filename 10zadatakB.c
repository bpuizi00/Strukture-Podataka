#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256
#define MAX_SIZE 40
struct _CityList;
struct _CountryTree;
typedef struct _CityList *ListPosition;
typedef struct _CountryTree *TreePos;

typedef struct _CityList
{
	char city[MAX_SIZE];
	int population;
	ListPosition next;

}CityList;

typedef struct _CountryTree
{
	char country[MAX_SIZE];
	TreePos left;
	TreePos right;
	ListPosition head;
}CountryTree;

int UserInput(char *file);
TreePos ReadFromFile(char *file, TreePos root);
int SortedInsert(ListPosition current, ListPosition newEl, int brStan);
ListPosition createListEl(char *grad, int population);
int InsertAfter(ListPosition head, ListPosition newEl);
int AddCityToList(char *gradoviFile, ListPosition head);
TreePos Insert(TreePos root, char *drzava, char *gradoviFile);
TreePos CreateNewNode(char *drzava, char *gradoviFile);
int InOrder(TreePos root);
TreePos SearchCountry(char *country, TreePos root);
int PrintCities(ListPosition head);
int PrintBiggerCities(ListPosition head, int populationInput);

int main() {
	char file[MAX] = "";
	TreePos root = NULL;
	printf("Unesite ime datoteke:");
	UserInput(file);
	root = ReadFromFile(file, root);

	InOrder(root);

	char Input[MAX] = "";
	char country[MAX] = "";
	int PopInput;
	TreePos current = NULL;
	do {
		printf("Upisite drzavu koju zelite pretrazit:\n");
		UserInput(country);
		current = SearchCountry(country, root);
		if (current != NULL) {
			printf("Unesite zeljenu granicu broja stanovnika gradova:\n");
			scanf("%d", &PopInput);
			getchar();
			printf("Gradovi sa  brojem stanovnika vecim od %d:\n", PopInput);
			PrintBiggerCities(current->head, PopInput);
		}
		printf("Za izlaz unesite rijec exit\n");
		UserInput(Input);

	} while (strcmp(Input, "exit") != 0);
	system("PAUSE");
	return 0;
}

int UserInput(char *str)
{
	fgets(str, MAX, stdin);

	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';
	return 0;
}

TreePos ReadFromFile(char *file, TreePos root)
{

	FILE *fp = NULL;
	char buffer[MAX] = { 0 };
	char drzava[MAX] = { 0 };
	char gradoviFile[MAX] = { 0 };
	int res = 0;
	fp = fopen(file, "r");
	if (fp == NULL)
	{
		perror("Can't open file!\n");
		return NULL;
	}

	while (!feof(fp))
	{
		fgets(buffer, MAX, fp);
		res = sscanf(buffer, "%s %s", drzava, gradoviFile);
		if (res == 2)
			root = Insert(root, drzava, gradoviFile);
	}

	fclose(fp);
	return root;
}

ListPosition createListEl(char *grad, int population)
{
	ListPosition q = NULL;
	q = (ListPosition)malloc(sizeof(CityList));
	if (!q) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	strcpy(q->city, grad);
	q->population = population;
	q->next = NULL;

	return q;
}

int InsertAfter(ListPosition head, ListPosition newEl)
{
	newEl->next = head->next;
	head->next = newEl;
	return 0;
}

int AddCityToList(char *gradoviFile, ListPosition head)
{
	ListPosition newEl = NULL;
	ListPosition current = head;
	char buffer[MAX] = { 0 };
	char city[MAX] = { 0 };
	int population = 0;
	int res = 0;
	FILE *fp = NULL;

	fp = fopen(gradoviFile, "r");
	if (fp == NULL) {
		perror("Can't open file!\n");
		return -1;
	}

	while (!feof(fp))
	{
		fgets(buffer, MAX, fp);
		res = sscanf(buffer, "%s %d", city, &population);
		if (res == 2) {
			newEl = createListEl(city, population);
			if (!newEl) {
				fclose(fp);
				return -2;
			}
			SortedInsert(current, newEl, population);
		}
	}
	fclose(fp);
	return 0;
}

int SortedInsert(ListPosition current, ListPosition newEl, int brStan) {
	while (current->next != NULL && current->next->population > brStan)
		current = current->next;
	InsertAfter(current, newEl);
	return 0;
}

TreePos Insert(TreePos root, char *drzava, char *gradoviFile)
{
	TreePos temp = NULL;
	TreePos node = root;
	if (node == NULL) {
		temp = CreateNewNode(drzava, gradoviFile);
		return temp;
	}
	else if (strcmp(node->country, drzava) > 0) {
		node->left = Insert(node->left, drzava, gradoviFile);
	}
	else if (strcmp(node->country, drzava) < 0) {
		node->right = Insert(node->right, drzava, gradoviFile);
	}
	else {
		printf("Postoji ista drzava.\n");
	}
	return root;
}

TreePos CreateNewNode(char *drzava, char *gradoviFile)
{
	TreePos q = NULL;
	q = (TreePos)malloc(sizeof(CountryTree));
	if (!q) {
		perror("Can't allocate memory!\n");
		return NULL;
	}
	strcpy(q->country, drzava);
	q->left = NULL;
	q->right = NULL;
	q->head = malloc(sizeof(CityList));
	if (!q->head) {
		perror("Can't allocate memory! \n");
		free(q);
		return NULL;
	}
	else {
		q->head->next = NULL;
		q->head->population = 0;
		strcpy(q->head->city, "");
	}

	AddCityToList(gradoviFile, q->head);
	return q;
}
int InOrder(TreePos root)
{
	TreePos temp = root;
	if (temp != NULL) {
		InOrder(temp->left);
		printf("Drzava: %s\n", temp->country);
		PrintCities(temp->head);
		InOrder(temp->right);
	}
	return 0;
}

TreePos SearchCountry(char *country, TreePos root)
{
	TreePos node = root;
	if (node == NULL) {
		return NULL;
	}
	else if (strcmp(node->country, country) > 0) {
		SearchCountry(country, root->left);
	}
	else if (strcmp(node->country, country) < 0) {
		SearchCountry(country, root->right);
	}
	else {
		return node;
	}
}

int PrintCities(ListPosition head)
{
	ListPosition current = head->next;
	printf(" Grad:                Broj Stanovnika:\n");
	while (current != NULL) {
		printf(" %-20.20s %d\n", current->city, current->population);
		current = current->next;
	}
	return 0;
}

int PrintBiggerCities(ListPosition head, int populationInput)
{
	ListPosition current = head->next;
	printf(" Grad:                Broj Stanovnika:\n");
	while (current != NULL) {
		if (current->population >= populationInput)
			printf(" %-20.20s %d\n", current->city, current->population);
		current = current->next;
	}
	return 0;
}