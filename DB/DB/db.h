#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dataList.h"
#include "strTree.h"
#include "AVLTree.h"
#include "types.h"

#include "debug.h"

DataList *dbListRoot = NULL;
DataList *dbListLast = NULL;

StrTreeNode *firstNameRoot = NULL;
StrTreeNode *middleNameRoot = NULL;
StrTreeNode *lastNameRoot = NULL;

AVLTreeNode *ageRoot = NULL;
AVLTreeNode *weightRoot = NULL;

void db_Put(void *data) {
	dbListLast = dataList_Put(&dbListRoot, data, dbListLast);

	debug_Log("---------------------");
	debug_Log("Добавляем запись в БД");
	debug_PrintData(data);

	strTree_Put(&firstNameRoot, ((Data*)data)->firstName, data);
	debug_Print("Дерево имён:\n");
	strTree_DebugPrint(firstNameRoot, 0);

	strTree_Put(&middleNameRoot, ((Data*)data)->middleName, data);
	debug_Print("Дерево отчеств:\n");
	strTree_DebugPrint(middleNameRoot, 0);

	strTree_Put(&lastNameRoot, ((Data*)data)->lastName, data);
	debug_Print("Дерево фамилий:\n");
	strTree_DebugPrint(lastNameRoot, 0);

	//ageRoot = insertDataToAVLTree(ageRoot, ((Data*)data)->age, data);
	//weightRoot = insertDataToAVLTree(weightRoot, ((Data*)data)->weight, data);
}

void db_Remove(void *data) {
	dataList_Remove(&dbListRoot, data);

	strTree_Remove(&firstNameRoot, ((Data*)data)->firstName, data);
	strTree_Remove(&middleNameRoot, ((Data*)data)->middleName, data);
	strTree_Remove(&lastNameRoot, ((Data*)data)->lastName, data);
	//ageRoot = removeDataToAVLTree(ageRoot, ((Data*)data)->age, data);
	//weightRoot = removeDataToAVLTree(weightRoot, ((Data*)data)->weight, data);
}

DataList *db_Find(void *data) {
	return dataList_Find(dbListRoot, data);
}

void db_Init() {
	firstNameRoot = NULL;
	middleNameRoot = NULL;
	lastNameRoot = NULL;
	dbListRoot = NULL;
}

void db_FillData(DataList *root) {
	DataList *node = root;
	debug_dataListPrint(root, debug_PrintData);

	unsigned int i = 0;
	while (node) {
		db_Put(node->data);
		node = node->next;
		i++;
	}
}

void _db_FreeFunc(void **data) {
	if (data && *data) {
		free(((Data*)*data)->firstName);
		((Data*)*data)->firstName = NULL;
		free(((Data*)*data)->middleName);
		((Data*)*data)->middleName = NULL;
		free(((Data*)*data)->lastName);
		((Data*)*data)->lastName = NULL;

		free(*data);
		*data = NULL;
	}
}

void db_Free() {
	strTree_Free(&firstNameRoot); firstNameRoot = NULL;
	strTree_Free(&middleNameRoot); middleNameRoot = NULL;
	strTree_Free(&lastNameRoot); lastNameRoot = NULL;

	//freeAVLTreeNode(ageRoot); ageRoot = NULL;
	//freeAVLTreeNode(weightRoot); weightRoot = NULL;

	dataList_ListFree(&dbListRoot, _db_FreeFunc);
}

void db_Print(Data *data) {
	if (data != NULL) {
		printf("%20s\t%20s\t%30s\t%1d\t%3d\t%3d\t%2d\n", data->firstName, data->middleName, data->lastName, data->sex, data->age, data->weight, data->group);
	}
}

void db_PrintList(DataList *dataList) {
	DataList *dl = dataList;
	while (dl != NULL) {
		db_Print(((Data*)dl->data));
		dl = dl->next;
	}
}