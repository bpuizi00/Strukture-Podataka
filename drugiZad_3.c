﻿#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SIZE (50)

struct _Person;
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SIZE (50)

struct _Person;
typedef struct _Person* Position;
typedef struct _Person
{
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;
	Position next;
}Person;

int PrependList(Position head, char* name, char* surname, int brithYear);
int PrintList(Position first);
Position CreatePerson(char* name, char* surname, int birthYear);
int InsertAfter(Position position, Position newPerson);
Position FindLast(Position head);
int AppendList(Position head, char* name, char* surname, int brithYear);
Position FindPerson(Position first, char* surname);
Position FindBefore(Position first, Position target);
int DeleteAfter(Position head, char* surname);

int InsertBefore(Position first, Position new_person, Position chosenpos);
int InsertToFile(Position first, char* nazivDatoteke);
int InsertFromFile(Position first, char* nazivDatoteke);


int main(int argc, char** argv)
{
	Person head = { .next = NULL,.name = { 0 },.surname = { 0 },.birthYear = 0 };
	Position p = &head;
	Position find = NULL;
	Position new_person = NULL;
	Position pos = NULL;
	int unos = 0;
	int temp = 1;
	char psurname[MAX_SIZE] = { 0 };
	char name[MAX_SIZE] = { 0 };
	char surname[MAX_SIZE] = { 0 };
	int birthyear = 0;
	char imedat[MAX_SIZE] = "zadatak.txt";

	while (temp)
	{
		printf("1- Dodaj novi el. na pocetak:\n"
			"2- Ispisi listu \n3- dodaj el na kraj liste \n"
			"4- Pronadi po prezimenu\n5- Brisi elem\n"
			"6- Dodaj nakon el.\n 7- Dodaj prije el.\n"
			"8- Upisi u dat\n 9- Upisi iz dat\n "
			"10-Sortiraj\n" // nisan stiga 
			"0-izlaz iz programa\n");
		scanf("%d", &unos);
		if (unos == 1) {
			printf("Ime: ");
			scanf("%s", name);
			printf("Prezime: ");
			scanf("%s", surname);
			printf("Godina rodenja: ");
			scanf("%d", &birthyear);
			PrependList(p, name, surname, birthyear);
		}
		else if (unos == 2) {
			PrintList(p->next);
		}
		else if (unos == 3) {
			printf("Ime: ");
			scanf("%s", name);
			printf("Prezime: ");
			scanf("%s", surname);
			printf("Godina rođenja: ");
			scanf("%d", &birthyear);
			AppendList(p, name, surname, birthyear);
		}
		else if (unos == 4) {
			printf("Unesite prezime: ");
			scanf(" %s", surname);
			find = FindPerson(p->next, surname);
			if (find == 0) printf("Trazena osoba nije pronadena\n");
			else printf("Ime: %s \n Prezime %s \n Godina Rodenja: %d \n",
				find->name, find->surname, find->birthYear);
		}
		else if (unos == 5) {
			printf("Prezime elem. koji zelite uklonit: ");
			scanf("%s", surname);
			DeleteAfter(p, surname);
		}
		else if (unos == 6) {
			printf("Unesite prezime: ");
			scanf("%s", psurname);
			printf("Ime: ");
			scanf("%s", name);
			printf("Prezime:");
			scanf("%s", surname);
			printf("Godina rođenja:");
			scanf("%d", &birthyear);
			new_person = CreatePerson(name, surname, birthyear);
			pos = FindPerson(p, psurname);
			if (pos != NULL) {
				InsertAfter(pos, new_person);
			}
		}
		else if (unos == 7) {
			printf("Unesite prezime: ");
			scanf("%s", psurname);
			printf("Ime: ");
			scanf("%s", name);
			printf("Prezime: ");
			scanf("%s", surname);
			printf("Godina rođenja:");
			scanf("%d", &birthyear);

			new_person = CreatePerson(name, surname, birthyear);
			pos = FindPerson(p, psurname);
			if (pos != NULL) {
				InsertBefore(p, new_person, pos);
			}
		}
		else if (unos == 8) {
			InsertToFile(p, imedat);
		}
		else if (unos == 9) {
			InsertFromFile(p, imedat);
		}
		else if (unos == 10) {
			printf("treba dovrsit ovu funkciju");
		}
		else if (unos == 0) {
			temp = 0;
			printf("Kraj programa");
		}
		else {
			printf("Pogresan unos\n");
		}
	}


	return EXIT_SUCCESS;
}

int PrependList(Position head, char* name, char* surname, int birthYear)
{
	Position newPerson = NULL;
	newPerson = CreatePerson(name, surname, birthYear);
	if (!newPerson)
	{
		return -1;
	}
	InsertAfter(head, newPerson);


	return EXIT_SUCCESS;
}

int PrintList(Position first)
{
	Position temp = first;
	/*for (Position temp = first; temp != NULL; temp = temp->next)
	{
	printf("Name: %s, surname= %s,""birth year: %d\n", temp->name, temp->surname, temp->birthYear); //for verzija donjeg whilea
	}*/
	while (temp)
	{
		printf("Name: %s, surname= %s,""birth year: %d\n", temp->name, temp->surname, temp->birthYear); // 2 "" "" samo se spoje
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}

Position CreatePerson(char* name, char* surname, int birthYear) {

	Position newPerson = NULL;
	newPerson = (Position)malloc(sizeof(Person));

	if (!newPerson)
	{
		perror("Can't allocate memory! \n");
		return NULL;
	}

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;
	newPerson->next = NULL;

	return newPerson;

}

int InsertAfter(Position position, Position newPerson)
{
	newPerson->next = position->next;
	position->next = newPerson;
	return EXIT_SUCCESS;
}
Position FindLast(Position head)
{
	Position temp = head;
	while (temp->next)
	{
		temp = temp->next;
	}

	return temp;
}

int AppendList(Position head, char* name, char* surname, int birthYear) {
	Position newPerson = NULL;
	Position last = NULL;

	newPerson = CreatePerson(name, surname, birthYear);
	if (!newPerson)
	{
		return -1;
	}
	last = FindLast(head);
	InsertAfter(last, newPerson);


	return EXIT_SUCCESS;
}

Position FindPerson(Position first, char* surname)
{
	Position temp = first;
	while (temp)
	{
		if (strcmp(temp->surname, surname) == 0)
		{
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

Position FindBefore(Position first, Position target)
{
	Position temp = first;
	while (temp->next != target) {
		temp = temp->next;
	}
	return temp;
}
int DeleteAfter(Position head, char* surname)
{
	Position find = NULL;
	Position before = NULL;
	find = FindPerson(head->next, surname);

	before = FindBefore(head, find);
	before->next = find->next;

	return EXIT_SUCCESS;
}


int InsertFromFile(Position first, char* nazivDatoteke) {
	FILE* dat = NULL;
	Position temp = first;
	dat = fopen(nazivDatoteke, "r");
	char ime[MAX_SIZE] = { 0 };
	char prezime[MAX_SIZE] = { 0 };
	int god = 0;
	if (!dat)
		return -1;
	while (!feof(dat)) {
		fscanf(dat, " %s %s %d \n", ime, prezime, &god);
		AppendList(first, ime, prezime, god);
	}
	fclose(dat);

}
int InsertToFile(Position first, char* nazivDatoteke) {
	FILE* dat = NULL;
	Position temp = first;
	dat = fopen(nazivDatoteke, "w");
	if (!dat)
		return -1;

	while (temp) {
		if (temp != first)
			fprintf(dat, " %s %s %d", temp->name, temp->surname, temp->birthYear);

		temp = temp->next;
	}
	fclose(dat);
	return 0;
}

int InsertBefore(Position first, Position new_person, Position chosenpos) {
	Position temp = NULL;
	if (chosenpos != NULL) {
		temp = FindBefore(first, chosenpos);
		if (temp != 0) {
			InsertAfter(temp, new_person);

		}
	}
}