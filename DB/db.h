#pragma once

#include "dataList.h"
#include "strTree.h"
#include "AVLTree.h"

typedef unsigned char byte;
typedef struct Data {
	char *firstName = NULL;
	char *middleName = NULL;
	char *lastName = NULL;

	bool sex;
	byte age;
	byte weight;
	byte group;
} Data;

DataList *dataListRoot = NULL;
DataList *dataListLast = NULL;

StrTreeNode *firstNameRoot = NULL;
StrTreeNode *middleNameRoot = NULL;
StrTreeNode *lastNameRoot = NULL;

AVLTreeNode *ageRoot = NULL;
AVLTreeNode *weightRoot = NULL;

void pushDataInList(void *data) {
	if (dataListRoot == NULL) {
		dataListRoot = createDataList(data);
		dataListLast = dataListRoot;
	}
	else {
		dataListLast = putInDataList(dataListRoot, data);
	}
}

void removeDataInList(void *data) {
	removeDataFromDataList(dataListRoot, data);
}

DataList *findInDataList(void *data) {
	return findDataInDataList(dataListRoot, data);
}

void putData(Data *data) {
	if (dataListLast == NULL) {
		dataListRoot = (DataList*)calloc(1, sizeof(DataList));
		dataListLast = dataListRoot;
	}
	else {
		DataList *prev = dataListLast;
		dataListLast = (DataList*)calloc(1, sizeof(DataList));
		prev->next = dataListLast;
	}
	dataListLast->data = data;
	putStrDataInTree(firstNameRoot, data->firstName, data);
	putStrDataInTree(middleNameRoot, data->middleName, data);
	putStrDataInTree(lastNameRoot, data->lastName, data);

	ageRoot = insertDataToAVLTree(ageRoot, data->age, data);
	weightRoot = insertDataToAVLTree(weightRoot, data->weight, data);
}

DataList *queryData(Data *data) {
	DataList *result = NULL;
	if (data != NULL) {
		if (data->firstName != NULL && strlen(data->firstName) > 0) {
			result = findInStrTree(firstNameRoot, data->firstName);
		}
		else if (data->middleName != NULL && strlen(data->middleName) > 0) {
			result = findInStrTree(middleNameRoot, data->middleName);
		}
		else if (data->lastName != NULL && strlen(data->lastName) > 0) {
			result = findInStrTree(lastNameRoot, data->lastName);
		}
		else {
			result = dataListRoot;
		}
	}

	return result;
}

void fillChar(char *word) {
	if (word != NULL) {
		for (size_t i = 0; i < strlen(word); i++) {
			word[i] = '*';
		}
	}
}

char *freeAndNULLStr(char *str) {
	if (str != NULL) {
		fillChar(str);
		free(str);
	}
	return NULL;
}

void removeData(Data *data) {
	findDataInDataList(dataListRoot, data);
	if (data != NULL) {
		removeDataInStrTree(firstNameRoot, data->firstName, data);
		removeDataInStrTree(middleNameRoot, data->middleName, data);
		removeDataInStrTree(lastNameRoot, data->lastName, data);

		removeDataFromAVLTree(ageRoot, data->age, data);
		removeDataFromAVLTree(weightRoot, data->weight, data);

		dataListRoot = removeDataFromDataList(dataListRoot, data);

		data->firstName = freeAndNULLStr(data->firstName);
		data->middleName = freeAndNULLStr(data->middleName);
		data->lastName = freeAndNULLStr(data->lastName);
		free(data);
	}
}

void initDB() {
	firstNameRoot = (StrTreeNode*)calloc(1, sizeof(StrTreeNode));
	middleNameRoot = (StrTreeNode*)calloc(1, sizeof(StrTreeNode));
	lastNameRoot = (StrTreeNode*)calloc(1, sizeof(StrTreeNode));
	dataListRoot = NULL;
	dataListLast = NULL;
}

void dataFreeFunc(void *data) {
	if (data != NULL) {
		Data *d = (Data*)data;
		//d->firstName = freeAndNULLStr(d->firstName);
		//d->middleName = freeAndNULLStr(d->middleName);;
		//d->lastName = freeAndNULLStr(d->lastName);;

		free(d);
		d = NULL;
	}
}

void clearDB() {
	freeStrTree(firstNameRoot); firstNameRoot = NULL;
	freeStrTree(middleNameRoot); middleNameRoot = NULL;
	freeStrTree(lastNameRoot); lastNameRoot = NULL;
	freeAVLTreeNode(ageRoot); ageRoot = NULL;
	freeAVLTreeNode(weightRoot); weightRoot = NULL;

	clearDataList(dataListRoot, dataFreeFunc); dataListRoot = NULL; dataListLast = NULL;
}

void printData(Data *data) {
	if (data != NULL) {
		printf("%20s\t%20s\t%30s\t%1d\t%3d\t%3d\t%2d\n", data->firstName, data->middleName, data->lastName, data->sex, data->age, data->weight, data->group);
	}
}

void printDataList(DataList *dataList) {
	DataList *dl = dataList;
	while (dl != NULL) {
		printData(((Data*)dl->data));
		dl = dl->next;
	}
}