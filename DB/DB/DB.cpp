// DB.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include "clocale"

#include "generator.h"
#include "db.h"
#include "query.h"

#include "debug.h"

void _freeTempData(void **data) {
	if (data && *data) {
		free(*data);
		*data = NULL;
	}
}

void _prindTempData(void *data) {
	if (!_debug) return;

	if (data != NULL) {
		Data *d = (Data*)data;
		debug_Print("%d\n", *(unsigned char*)data);
	}
}

int main()
{

	setlocale(LC_CTYPE, "rus");

	debug_ON();

	DataList *list = NULL;
	for (int i = 0; i < 10; i++) {
		unsigned char *data = (unsigned char*)calloc(1, sizeof(unsigned char));
		*data = rand() % 255;
		dataList_Put(&list, data);
	}
	debug_dataListPrint(list, _prindTempData);

	AVLTreeNode *avlTreeRoot = NULL;
	DataList *node = list;
	while (node) {
		avlTreeRoot = AVLTree_Put(avlTreeRoot, *(unsigned char*)node->data, node->data);
		debug_Print("-------------------------\n");
		debug_AVLTreePrint(avlTreeRoot, 0);

		node = node->next;
	}

	AVLTree_Free(&avlTreeRoot);
	dataList_ListFree(&list, _freeTempData);

	//**************************************************

	initGenerator();

	char *str = (char*)calloc(255, sizeof(char));
	int count;
	printf("Введите кол-во генерируемых записей или имя файла: ");
	scanf("%s", str);

	char *end_ptr = NULL;
	count = strtol(str, &end_ptr, 10);

	DataList *dataList;
	if (*end_ptr != NULL) {
		if (strcmp(str, "-") == 0) {
			str = strcpy(str, "test_data.txt");
		}
		dataList = loadDataFromFile(str);
	}
	else {
		dataList = generateDataList(count);
		printf("Сохранить сгененированный набор? [y, n] ");
		scanf("%s", str);
		if (str[0] == 'y') {
			printf("Введите имя файла: ");
			scanf("%s", str);
			saveDataToFile(str, dataList);
		}
	}
	freeGenerator();

	free(str);
	str = NULL;
	end_ptr = NULL;

	db_Init();
	db_FillData(dataList);
	dataList_ListFree(&dataList);

	int i = 0;
	while (i < 5) {	//strcmp(str, "exit") != 0

		DataList *ql = NULL;
		switch (i) {
		case 0:
			printf("\n--------------------------------------\nfirstName=\"Алексей\"\n");
			query_Put(&ql, query_Create("firstName", "Алексей", 0, 0));

			break;

		case 1:
			printf("\n--------------------------------------\nlastName=\"Куликов\"\n");
			query_Put(&ql, query_Create("lastName", "Куликов", 0, 0));
			break;

		case 2:
			printf("\n--------------------------------------\nfirstName=\"Алексей\" OR lastName=\"Куликов\"\n");
			query_Put(&ql, query_Create("firstName", "Алексей", 0, 0));
			query_Put(&ql, query_Create("lastName", "Куликов", 0, 1));
			break;

		case 3:
			printf("\n--------------------------------------\nfirstName=\"Алексей\" AND lastName=\"Куликов\"\n");
			query_Put(&ql, query_Create("firstName", "Алексей", 0, 0));
			query_Put(&ql, query_Create("lastName", "Куликов", 0, 0));
			break;

		case 4:
			printf("\n--------------------------------------\nfirstName=\"Алексей\" OR lastName=\"Куликов\" NOT middleName=\"Бориславович\"\n");
			query_Put(&ql, query_Create("firstName", "Алексей", 0, 0));
			query_Put(&ql, query_Create("lastName", "Куликов", 0, 1));
			query_Put(&ql, query_Create("middleName", "Бориславович", 0, 2));
			break;
		}

		DataList *res = query_Query(ql);
		db_PrintList(res);
		printf("--------------------------------------\n");

		dataList_ListFree(&res);
		query_ListFree(&ql);

		i++;
	}

	db_Free();

	system("pause");

	return 0;
}

