#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE (1024)


typedef struct polinomi* Position;
typedef struct polinomi {
    int coeff;
    int expo;
    Position next;
} Polinom;

Position CreateEl (int coeff,int expo);
int CitajDatoteku(Position p1, Position p2);
int CitanjeRedaka(char* unos,Position p);
int zbroj(Position a,Position b,Position sum);
int umnozak(Position a,Position b,Position umn); 
int Print(Position a);
int InsertSorted(Position head, Position newElement);
int DeleteAfter(Position prev);
int InsertAfter(Position position, Position newElement);
int MergeAfter(Position position, Position newElement);

int main()
{
    Polinom Head1= {.coeff=0, .expo=0, .next=NULL};
    Polinom Head2= {.coeff=0, .expo=0, .next=NULL};
    Polinom HeadS= {.coeff=0, .expo=0, .next=NULL};
    Polinom HeadU= {.coeff=0, .expo=0, .next=NULL};
    Position a=&Head1;
    Position b=&Head2;
    Position sum=&HeadS;
    Position umn=&HeadU;
    
    CitajDatoteku(a,b);
    
    printf("Prvi: \n");
    Print(Head1.next);
    printf("Drugi: \n");
    Print(Head2.next);
    zbroj(&Head1,&Head2,&HeadS);
    printf("Zbroj: \n");
    Print(HeadS.next);
    umnozak(&Head1,&Head2,&HeadU);
    printf("Umnozak: \n");
    Print(HeadU.next);
    return 0;
}
int CitajDatoteku(Position p1, Position p2) 
{
    FILE* fp;
    char imeDat[50]= { 0 };
    char buffer[MAX_LINE] = { 0 };
    printf("\nNaziv datoteke :");
    scanf(" %s", imeDat);

    fp = fopen(imeDat, "r");
    if (!fp) return -1;

    fgets(buffer, MAX_LINE, fp);
    CitanjeRedaka(buffer, p1);

    fgets(buffer, MAX_LINE, fp);
    CitanjeRedaka(buffer, p2);

    fclose(fp);
    return EXIT_SUCCESS;
}
int CitanjeRedaka(char* unos,Position p)
{
    char* temp = unos;
    int ppomak = 0, upomak = 0;
    Position new = NULL;

    int coeff = 0, expo = 0;
    while (strlen(temp + upomak) > 0) {
        sscanf(temp+upomak, " %d %d %n", &coeff, &expo, &ppomak);
        upomak += ppomak;
        if (coeff != 0) {
            new = CreateEl(coeff, expo);
            InsertSorted(p, new);
        }
    }
    return EXIT_SUCCESS;
}
Position CreateEl(int coeff,int expo)
{
    Position new=(Position)malloc(sizeof(Polinom));
    if(new==NULL){
        printf("Greska kod stvaranja novog el.\n");
        return NULL;
    }
    new->coeff=coeff;
    new->expo=expo;
    return new;
}
int Print(Position a)
{
    while(a != NULL){
        printf("Koef: %d Eksponent: %d \n", a->coeff, a->expo);
        a=a->next;
    }
    return 0;
}
int DeleteAfter(Position prev)
{
	Position Brisi = NULL;

	Brisi = prev->next;
	prev->next = Brisi->next;
	free(Brisi);

	return EXIT_SUCCESS;
}

int InsertAfter(Position position, Position newElement)
{
	newElement->next = position->next;
	position->next = newElement;

	return EXIT_SUCCESS;
}

int MergeAfter(Position position, Position newElement)
{
	if (position->next == NULL || position->next->expo != newElement->expo)
	{
		InsertAfter(position, newElement);
	}
	else 
	{
		int resultCoefficient = position->next->coeff + newElement->coeff;
		if (resultCoefficient == 0) 
		{
			DeleteAfter(position);
		}
		else
		{
			position->next->coeff = resultCoefficient;
		}
		free(newElement);
	}

	return EXIT_SUCCESS;
}

int InsertSorted(Position head, Position newElement)
{
	Position temp = head;

	while (temp->next != NULL && temp->next->expo < newElement->expo)
	{
		temp = temp->next;
	}

	MergeAfter(temp, newElement);

	return EXIT_SUCCESS;
}
int umnozak(Position a,Position b,Position umn)
{
    Position b2=b;
    Position new=NULL;
    for (a; a != NULL; a = a->next) 
    {
        b2 = b;
        for (b2; b2 != NULL; b2 = b2->next)
        {
            new = CreateEl(a->coeff * b2->coeff, a->expo + b2->expo);
            InsertSorted(umn, new);
        }
    }
    return EXIT_SUCCESS;
}
int zbroj(Position a,Position b,Position sum)
{
    Position new=NULL;
    Position temp=NULL;
    while(a && b){
        if(a->expo == b->expo){
            new=CreateEl(a->coeff+b->coeff,a->expo);
            a=a->next;
            b=b->next;
        }
        else if(a->expo > b->expo){
            new=CreateEl(a->coeff,a->expo);
            a=a->next;
        }
        else{
            new=CreateEl(b->coeff,b->expo);
            b=b->next;
        }
         InsertSorted(sum,new);
    }
    if(a && !b) temp=a;
    else temp=b;
    while(temp){
        new=CreateEl(temp->coeff,temp->expo);
        InsertSorted(sum,new);
        temp=temp->next;
    }
    return EXIT_SUCCESS;
}
