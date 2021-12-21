#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 89
#define MIN 11

struct _Node;
typedef struct _Node* Pos;
typedef struct _Node {
	int number;
	Pos left;
	Pos right;
}Node;

struct _ListNode;
typedef struct _ListNode* ListPos;
typedef struct _ListNode {
	int number;
	ListPos next;
}ListNode;

Pos Insert(Pos current, Pos newElem);
Pos CreateNewElem(int num);
ListPos NewElem(int nodeNUM);
int UserINPUT();
int InsertIntoList(ListPos listHead, int newNUM);
int AddTreeToList(Pos root, ListPos listHead);
int Replace(Pos current);
int PrintToFile(ListPos initialListHead, ListPos replacedListHead);

int main()
{
	Pos root = NULL;
	ListPos ListHead = NULL;
	ListPos replacedListHead = NULL;
	time_t t;
	ListNode headListPrvi = { .number = 0,.next = NULL };
	ListHead = &headListPrvi;
	ListNode headListDrugi = { .number = 0,.next = NULL };
	replacedListHead = &headListDrugi;

	int choice = 0;
	int newNumber = 0;
	srand((unsigned)time(&t));

	do
	{
		printf("Choose your next action! \n");
		printf(" (1) - Add random number between 10 and 90 \n"
			" (2) - Replace each node value with the sum of it's descendants"
			"and print into a file\n (0) - Exit \n");
		choice = UserINPUT();
		switch (choice)
		{
		case 1:
		{
			Pos newElem = NULL;
			ListPos newListElem = NULL;
			if (!root)
			{
				newNumber = rand() % (MAX - MIN + 1) + MIN;
				Node headElem = { .number = newNumber,.left = NULL,.right = NULL };
				root = &headElem;
			}
			else
			{
				newNumber = rand() % (MAX - MIN + 1) + MIN;
				newElem = CreateNewElem(newNumber);
				Insert(root, newElem);
			}
			printf("Inserted number: %d \n", newNumber);
			break;
		}
		case 2:
		{
			AddTreeToList(root, ListHead);
			Replace(root);
			AddTreeToList(root, replacedListHead);
			PrintToFile(ListHead, replacedListHead);
			break;
		}
		default:
		{
			if (choice != 0)
			{
				printf("Wrong input! Try again \n \n");
				break;
			}
		}
		}
	} while (choice != 0);

	return 0;
}

Pos Insert(Pos current, Pos newElement)
{
	if (!current)
		return newElement;

	else if (newElement->number < current->number)
		current->right = Insert(current->right, newElement);

	else if (newElement->number >= current->number)
		current->left = Insert(current->left, newElement);

	return current;
}

Pos CreateNewElem(int value)
{
	Pos newElem = NULL;
	newElem = (Pos)malloc(sizeof(Node));
	if (!newElem)
	{
		perror("Couldn't allocate memory!");
		return NULL;
	}
	newElem->number = value;
	newElem->right = NULL;
	newElem->left = NULL;
	return newElem;
}

ListPos NewElem(int nodeNumber)
{
	ListPos newElement = NULL;
	newElement = (ListPos)malloc(sizeof(ListNode));
	if (!newElement)
	{
		perror("Could not allocate memory!");
		return NULL;
	}
	newElement->number = nodeNumber;
	newElement->next = NULL;
	return newElement;
}
int UserINPUT()
{
	char userInput[100] = { 0 };
	int userInputInt = 0;
	int Status = 0;
	do {

		scanf("%s", userInput);
		for (int i = 0; i < strlen(userInput); i++)
		{
			Status = 0;

			if (isdigit(userInput[i]) == 0)
			{
				Status = 1;
				printf("Wrong input, please enter a number \n");
				break;
			}
		}

	} while (Status == 1);

	userInputInt = atoi(userInput);
	return userInputInt;
}

int InsertIntoList(ListPos listHead, int newNumber)
{
	ListPos temp = listHead;
	ListPos newElement = NULL;
	newElement = NewElem(newNumber);

	if (!newElement)
	{
		perror("Could not allocate memory");
		return EXIT_FAILURE;
	}
	while (temp->next)
		temp = temp->next;

	newElement->next = temp->next;
	temp->next = newElement;
	return EXIT_SUCCESS;
}
int AddTreeToList(Pos current, ListPos listHead)
{
	Pos temp = current;
	if (temp && temp->left)
		AddTreeToList(temp->left, listHead);
	if (temp)
		InsertIntoList(listHead, temp->number);
	if (temp && temp->right)
		AddTreeToList(temp->right, listHead);
	return EXIT_SUCCESS;
}
int Replace(Pos current)
{
	int temp = 0;
	if (!(current->left || current->right))
	{
		temp = current->number;
		current->number = 0;
		return temp;
	}
	else if (current->left && current->right)
	{
		temp = current->number;
		current->number = Replace(current->left) + Replace(current->right);
		temp += current->number;
		return temp;
	}
	else if (current->left && !current->right)
	{
		temp = current->number;
		current->number = Replace(current->left);
		temp += current->number;
		return temp;
	}
	else
	{
		temp = current->number;
		current->number = Replace(current->right);
		temp += current->number;
		return temp;
	}
	return current->number;
}
int PrintToFile(ListPos initialListHead, ListPos replacedListHead)
{
	FILE* file = NULL;
	ListPos temp = initialListHead->next;
	ListPos temp2 = replacedListHead->next;

	file = fopen("tree.txt", "w");
	if (!file)
	{
		perror("Could not open file");
		return EXIT_FAILURE;
	}
	fprintf(file, "Inputed tree: \n");
	while (temp)
	{
		fprintf(file, "%d ", temp->number);
		temp = temp->next;
	}
	fprintf(file, "\n");

	fprintf(file, "Binary tree after replacing values: \n");
	while (temp2)
	{
		fprintf(file, "%d ", temp2->number);
		temp2 = temp2->next;
	}
	fclose(file);
	return EXIT_SUCCESS;
}