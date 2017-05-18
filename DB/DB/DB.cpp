// DB.cpp: ���������� ����� ����� ��� ����������� ����������.
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

int main()
{
	setlocale(LC_CTYPE, "rus");

	/*	DataList *list = NULL;
		for (int i = 0; i < 10; i++) {
			char *data = (char*)calloc(10, sizeof(char));
			strcpy(data, "123456789");
			dataList_Put(&list, data);
		}

		StrTreeNode *strTreeRoot = NULL;
		DataList *node = list;
		while (node) {
			strTree_Put(&strTreeRoot, (char*)node->data, node->data);
			node = node->next;
		}

		strTree_Free(&strTreeRoot);

		dataList_ListFree(&list, _freeTempData);
	*/
	initGenerator();

	char *str = (char*)calloc(255, sizeof(char));
	int count;
	printf("������� ���-�� ������������ ������� ��� ��� �����: ");
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
		printf("��������� ��������������� �����? [y, n] ");
		scanf("%s", str);
		if (str[0] == 'y') {
			printf("������� ��� �����: ");
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
			printf("\n--------------------------------------\nfirstName=\"�������\"\n");
			query_Put(&ql, query_Create("firstName", "�������", 0, 0));

			break;

		case 1:
			printf("\n--------------------------------------\nlastName=\"�������\"\n");
			query_Put(&ql, query_Create("lastName", "�������", 0, 0));
			break;

		case 2:
			printf("\n--------------------------------------\nfirstName=\"�������\" OR lastName=\"�������\"\n");
			query_Put(&ql, query_Create("firstName", "�������", 0, 0));
			query_Put(&ql, query_Create("lastName", "�������", 0, 1));
			break;

		case 3:
			printf("\n--------------------------------------\nfirstName=\"�������\" AND lastName=\"�������\"\n");
			query_Put(&ql, query_Create("firstName", "�������", 0, 0));
			query_Put(&ql, query_Create("lastName", "�������", 0, 0));
			break;

		case 4:
			printf("\n--------------------------------------\nfirstName=\"�������\" OR lastName=\"�������\" NOT middleName=\"������������\"\n");
			query_Put(&ql, query_Create("firstName", "�������", 0, 0));
			query_Put(&ql, query_Create("lastName", "�������", 0, 1));
			query_Put(&ql, query_Create("middleName", "������������", 0, 2));
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

