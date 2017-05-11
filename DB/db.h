#pragma once

#include "dataList.h"
#include "strTree.h"
#include "AVLTree.h"

typedef unsigned char byte;
typedef struct Data {
	char *firstName;
	char *middleName;
	char *lastName;
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

void removeData(Data *data) {
	void *data = findDataInDataList(dataListRoot, data);
	if (data != NULL) {
		removeDataInStrTree(firstNameRoot, data->firstName, data);
		removeDataInStrTree(middleNameRoot, data->middleName, data);
		removeDataInStrTree(lastNameRoot, data->lastName, data);
		removeDataFromAVLTree(ageRoot, data->age, data);
		removeDataFromAVLTree(weightRoot, data->weight, data);
		dataListRoot = removeDataFromDataList(dataListRoot, data);
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
		free(((Data*)data)->firstName);
		((Data*)data)->firstName = NULL;
		free(((Data*)data)->middleName);
		((Data*)data)->middleName = NULL;
		free(((Data*)data)->lastName);
		((Data*)data)->lastName = NULL;
		free(((Data*)data));
		data = NULL;
	}
}

void clearDB() {
	freeStrTree(firstNameRoot); firstNameRoot = NULL;
	freeStrTree(middleNameRoot); middleNameRoot = NULL;
	freeStrTree(lastNameRoot); lastNameRoot = NULL;
	freeAVLTreeNode(ageRoot);
	freeAVLTreeNode(weightRoot);

	clearDataList(dataListRoot, dataFreeFunc); dataListRoot = NULL; dataListLast = NULL;
}

void printData(Data *data) {
	if (data != NULL) {
		printf("%s\t%s\t%s\t%d\t%d\t%d\t%d\n", data->firstName, data->middleName, data->lastName, data->sex, data->age, data->weight, data->group);
	}
}

void printDataList(DataList *dataList) {
	DataList *dl = dataList;
	while (dl != NULL) {
		printData(((Data*)dl->data));
		dl = dl->next;
	}
}