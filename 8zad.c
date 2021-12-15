#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

struct _stablo;
typedef struct _stablo* Position;
typedef struct _stablo {
	int num;
	Position left;
	Position right;
}stablo;

Position insert(Position current, Position q);
Position NewElement(int num);

int INorder(Position current);
int PREorder(Position current);
int POSTorder(Position current);

Position find(Position current, int trazi);
Position delete(Position current, int brisi);
Position FindMax(Position current);
void find_op(Position current);

int main()
{
	Position root = NULL;
	Position q;
	int brisi = 0;
	int choice = 0;
	int broj = 0;
	do {
		printf("\nOdaberi sljedecu akciju: \n"
			" (0)-Unos elementa\n (1)-Postorder ispis\n "
			"(2)-Inorder ispis\n (3)-Preorder ispis\n "
			"(4)-Izbrisi broj\n (5)-Pronadi broj\n (6)-Izlazak iz programa \n");
		scanf("%d", &choice);
		switch (choice) {
		case 0:
			printf("Unesi broj koji zelis upisati: ");
			scanf("%d", &broj);
			q = NewElement(broj);
			root = insert(root, q);
			break;
		case 1:
			printf("\n Postorder ispis: ");
			POSTorder(root);
			break;
		case 2:
			printf("\n Inorder ispis: ");
			INorder(root);
			break;
		case 3:
			printf("\n Preorder ispis: ");
			PREorder(root);
			break;

		case 4:
			printf("Unesi broj koji zelis izbrisati:");
			scanf("%d", &brisi);
			delete(root, brisi);
			break;
		case 5:
			find_op(root);
			break;
		case 6:
			break;
		default:
			printf("Krivi unos");
			break;
		}
	} while (choice != 6);
	return 0;
}

int INorder(Position current) {
	if (current == NULL)
		return 0;
	INorder(current->left);
	printf("%d", current->num);
	INorder(current->right);
	return 0;
}
int PREorder(Position current) {
	if (current == NULL)
		return 0;
	printf("%d", current->num);
	PREorder(current->left);
	PREorder(current->right);
	return 0;
}
int POSTorder(Position current) {
	if (current == NULL)
		return 0;
	POSTorder(current->left);
	POSTorder(current->right);
	printf("%d", current->num);
	return 0;
}
Position insert(Position current, Position q) {
	if (current == NULL)
		return q;

	else if (current->num < q->num)
		current->right = insert(current->right, q);
	else if (current->num > q->num)
		current->left = insert(current->left, q);
	else
		free(q);

	return current;
}
Position NewElement(int number)
{
	Position newElement = NULL;
	newElement = (Position)malloc(sizeof(stablo));
	if (!newElement)
	{
		printf("Can't allocate memory");
		return NULL;
	}

	newElement->num = number;
	newElement->left = NULL;
	newElement->right = NULL;

	return newElement;
}
void find_op(Position current) {

	int unos;
	Position vrijednost;
	printf("\n Upisite element koji trazite: ");
	scanf("%d", &unos);
	vrijednost = find(current, unos);

	if (vrijednost == NULL)
		printf("Nema trazenog broja unutar stabla!");
	else
		printf("Broj je pronaden unutar stabla!");
}
Position find(Position current, int unos) {

	if (current == NULL)
		return NULL;
	else if (current->num > unos)
		return find(current->left, unos);
	else if (current->num < unos)
		return find(current->right, unos);
	return current;

}
Position FindMax(Position current)
{
	if (NULL == current)
		return NULL;

	else if (current->right == NULL)
		return current;
	else
		return FindMax(current->left);
}
Position delete(Position current, int brisi)
{
	if (NULL == current)
		printf("Nema trazenog broja u stablu!");
	else if (current->num > brisi)
		current->left = delete(current->left, brisi);
	else if (current->num < brisi)
		current->right = delete(current->right, brisi);
	else {
		if (current->left != NULL && current->right != NULL)
		{
			Position temp = FindMax(current->right);
			current->num = temp->num;
			current->right = delete(current->right, current->num);
		}
		else
		{
			Position temp = current;
			if (NULL == current->left)
				current = current->right;
			else
				current = current->left;
			free(temp);
		}
	}
	return current;
}