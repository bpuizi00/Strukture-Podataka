#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE 1024

struct _Directory;
typedef struct _Directory* Position;
typedef struct _Directory
{
	char name[MAX_LINE];
	Position sibling;
	Position child;
}Directory;

struct _StackElement;
typedef struct _StackElement* StackPos;
typedef struct _StackElement
{
	Position dir;
	StackPos next;
}StackElement;

Position NewElement(char name[]);
Position Insert(Position current, Position newEl);
Position EnterDirectory(Position parent, char name[], StackPos head);
Position GoToParent(StackPos head);
int List(Position parent);
int DeleteDirAfter(Position del);
int DeleteDirAll(Position first);
int InsertAfter(Position first, Position new);

StackPos NewStackElement(Position dir);
int Pop(StackPos head, Position* mem);
int Push(StackPos head, StackPos newS);
int DelStack(StackPos head);

int main()
{
	Directory cHead = { .name = "C",.sibling = NULL,.child = NULL };
	Position c = &cHead;
	StackElement sHead = { .dir = c,.next = NULL };
	StackPos s = &sHead;
	Position currentDir = c;
	int flag = 0;
	int select = 0;
	char name[MAX_LINE] = { 0 };

	while (flag == 0)
	{
		printf("Select the action you want to do:\n"
			"1 - md -make directory \n2 - dir - print the contents of the directory\n"
			"3 - cd dir - enter the directory\n4 - go to parent directory\n5 - exit\n\n");
		scanf("%d", &select);
		printf("\n");

		switch (select)
		{
		case 1:
		{
			printf("Enter name of the new directory:\n");
			printf("\n");
			scanf("%s", name);
			Position new = NewElement(name);
			currentDir->child = Insert(currentDir->child, new);
			break;
		}
		case 2:
		{
			printf("%s:", currentDir->name);
			List(currentDir);
			break;
		}
		case 3:
		{
			printf("Which directory do you want to enter:\n");
			printf("%s:", currentDir->name);
			scanf_s("%s", name, MAX_LINE);
			Position prev = currentDir;
			currentDir = EnterDirectory(currentDir, name, s);
			if (!currentDir)
			{
				currentDir = prev;
			}
			break;
		}
		case 4:
		{
			if (currentDir == c)break;
			currentDir = GoToParent(s);
			break;
		}
		case 5:
		{
			if (c->child) {
				DeleteDirAll(c->child);
			}
			DelStack(s);
			flag = 1;
			break;
		}
		default:
		{
			printf("Wrong input!\n");
			break;
		}
		}
	}
	return 0;
}

int Push(StackPos head, StackPos newS)
{
	newS->next = head->next;
	head->next = newS;
	return 0;
}
int Pop(StackPos head, Position* mem)
{
	if (head->next == NULL)
	{
		printf("Stack is empty!\n");
		return 1;
	}

	StackPos temp = head->next;
	head->next = head->next->next;
	*mem = temp->dir;
	free(temp);
	return 0;
}
StackPos NewStackElement(Position dir)
{
	StackPos q = (StackPos)malloc(sizeof(StackElement));
	if (!q)
	{
		perror("Couldn't allocate memory\n");
		return NULL;
	}
	q->next = NULL;
	q->dir = dir;
	return q;
}
int DelStack(StackPos head)
{
	Position mem = NewElement("");
	while (head->next)
	{
		Pop(head, &mem);
	}

	return EXIT_SUCCESS;
}

Position NewElement(char name[])
{
	Position q = (Position)malloc(sizeof(Directory));
	if (!q)
	{
		perror("Couldn't allocate memory\n");
		return NULL;
	}

	strcpy(q->name, name);
	q->sibling = NULL;
	q->child = NULL;
	return q;
}

Position Insert(Position current, Position newEl)
{
	if (!current)
	{
		return newEl;
	}
	else if (strcmp(current->name, newEl->name) < 0)
	{
		current->sibling = Insert(current->sibling, newEl);
	}
	else if (strcmp(current->name, newEl->name) == 0)
	{
		free(newEl);
		return current;
	}
	else
	{
		newEl->sibling = current;
		return newEl;
	}

	return current;
}

int List(Position parent)
{
	Position temp = parent->child;
	while (temp)
	{
		printf("%s\n", temp->name);
		temp = temp->sibling;
	}

	printf("\n");

	return 0;
}

Position EnterDirectory(Position parent, char name[], StackPos head)
{
	Position temp = parent->child;
	StackPos q = NULL;
	while (temp != NULL && strcmp(temp->name, name) != 0)
	{
		temp = temp->sibling;
	}

	if (temp == NULL)
	{
		printf("Couldn't find directory\n");
		return NULL;
	}
	q = NewStackElement(parent);
	Push(head, q);

	return temp;
}

int DeleteDirAfter(Position del)
{
	Position temp = del->sibling;
	del->sibling = del->sibling->sibling;
	free(temp);
	return EXIT_SUCCESS;
}

int DeleteDirAll(Position first)
{
	if (!first)
		return 0;
	else if (first->sibling)
	{
		DeleteDirAll(first->sibling);
	}
	else if (first->child)
	{
		DeleteDirAll(first->child);
	}
	free(first);
	return 0;
}

int InsertAfter(Position first, Position new)
{
	new->sibling = first->sibling;
	first->sibling = new;
	return EXIT_SUCCESS;
}

Position GoToParent(StackPos head)
{
	Position mem = NewElement("");
	if (Pop(head, &mem))
		return head->dir;
	return mem;
}