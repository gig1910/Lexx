// HW.cpp: определяет точку входа для консольного приложения.
// Алексей Горяйнов

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <windows.h>

char *path = "d:\\tmp\\database.txt";

typedef struct Data
{
	struct Person *link;
	struct Data *next;
} Data;

typedef struct Node_L {
	struct Letter *list;
	struct Data *data;
} Node_L;

typedef struct Letter {
	char l;
	struct Letter *next;
	struct Node_L *node;
} Letter;

typedef struct Person {
	char *LName;
	char *FName;
	int age;
	int tabNum;
	struct Person *next;
} Person;

Person *persons;

int readData()
{
	FILE *in;
	Person *tmp = NULL;
	in = fopen(path, "r");
	if (in == NULL) {
		printf("Cannot open file\n");
		return EXIT_FAILURE;
	}
	char buf[255];
	while (!feof(in)) {
		persons = (Person*)calloc(sizeof(Person), 1);
		fscanf(in, "%s", buf);
		persons->LName = (char*)calloc(strlen(buf), sizeof(char));
		strcpy(persons->LName, buf);
		fscanf(in, "%s", buf);
		persons->FName = (char*)calloc(strlen(buf), sizeof(char));
		strcpy(persons->FName, buf);
		fscanf(in, "%d", &persons->age);
		fscanf(in, "%d", &persons->tabNum);
		persons->next = tmp;
		tmp = persons;
	}
}

void printData()
{
	Person *tmp = persons;
	while (tmp->next != NULL) {
		printf("%s ", tmp->LName);
		printf("%s\n", tmp->FName);
		printf("возраст: %d\n", tmp->age);
		printf("таб номер: %d\n", tmp->tabNum);
		tmp = tmp->next;
	}
}

Node_L *createNode()
{
	Node_L *node = (Node_L*)calloc(sizeof(Node_L), 1);
	node->data = NULL;
	node->list = NULL;
	return node;
}

Letter *createLetter(char l)
{
	Letter *link = (Letter*)calloc(sizeof(Letter), 1);
	link->l = l;
	link->next = NULL;
	link->node = createNode();
	return link;
}

Node_L *createTree()
{
	Person *P = persons;
	Node_L *node = (Node_L*)calloc(sizeof(Node_L), 1);
	Node_L *Root;
	Node_L *tmpNode;
	Letter *currentLetter;
	Letter *tmpLink;
	Root = node;
	while (P != NULL) {
		for (int i = 0; i < strlen(P->LName); i++) {
			tmpNode = NULL;
			tmpLink = NULL;
			if (node->list != NULL) {
				currentLetter = node->list;
				while (currentLetter != NULL && tmpNode == NULL) {
					if (P->LName[i] == currentLetter->l) {
						tmpNode = currentLetter->node;
					}
					else {
						tmpLink = currentLetter;
						currentLetter = currentLetter->next;
					}
				}
			}
			if (tmpNode == NULL) {
				if (tmpLink == NULL) {
					node->list = createLetter(P->LName[i]);
					tmpNode = node->list->node;
				}
				else {
					tmpLink->next = createLetter(P->LName[i]);
					tmpNode = tmpLink->next->node;
				}
			}
			node = tmpNode;
		}

		if (node->data == NULL) {
			node->data = (Data*)calloc(sizeof(Data), 1);
			node->data->link = P;
		}
		else {
			Data *data = NULL;
			data = node->data;
			while (data->next != NULL) {
				data = data->next;
			}
			data->next = (Data*)calloc(sizeof(Data), 1);
			data->next->link = P;
		}
		P = P->next;
		node = Root;
	}
	return Root;
}

Data *searchText(Node_L *Tree, char *search)
{
	Letter *letter = Tree->list;
	Data *res = NULL;
	int s = 0;
	while (search[s] != '\0' && letter != NULL) {
		if (letter->l == search[s]) {
			s++;
			if (search[s] != '\0') {
				if (letter->node->list) {
					letter = letter->node->list;
				}
			}
			else { res = letter->node->data; }
		}
		else {
			letter = letter->next;
		}
	}
	return res;
}

int main()
{
	Node_L *Tree;
	Tree = createNode();
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	readData();
	printData();
	Tree = createTree();
	Data *res = NULL;
	res = searchText(Tree, "Петров");
	if (res) {
		do {
			printf("%s %s\n", res->link->LName, res->link->FName);
			res = res->next;
		} while (res != NULL);
	}
	else { printf("не нашлось\n"); }
	system("pause");
	return 0;
}
