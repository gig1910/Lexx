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


int main()
{
	setlocale(LC_CTYPE, "rus");

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
			str = strcpy(str, "D:\\Lexx\\001.txt");
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

	db_Init();
	db_FillData(dataList);
	dataList_ListFree(&dataList);

	str = strcpy(str, "repeat");
	int i = 0;
	while (i < 5) {	//strcmp(str, "exit") != 0

		QueryList *queryList = (QueryList*)calloc(1, sizeof(QueryList));
		QueryList *ql = queryList;
		switch (i) {
		case 0:
			printf("\n--------------------------------------\nfirstName=\"Алексей\"\n");
			ql->query = (Query*)calloc(1, sizeof(Query));
			ql->query->field = "firstName";
			ql->query->query = "Алексей";
			ql->query->condition = 0;

			break;

		case 1:
			printf("\n--------------------------------------\nlastName=\"Куликов\"\n");
			ql->query = (Query*)calloc(1, sizeof(Query));
			ql->query->field = "lastName";
			ql->query->query = "Куликов";
			ql->condition = 1;
			break;

		case 2:
			printf("\n--------------------------------------\nfirstName=\"Алексей\" OR lastName=\"Куликов\"\n");
			ql->query = (Query*)calloc(1, sizeof(Query));
			ql->query->field = "firstName";
			ql->query->query = "Алексей";
			ql->query->condition = 0;

			ql->next = (QueryList*)calloc(1, sizeof(QueryList));
			ql = ql->next;

			ql->query = (Query*)calloc(1, sizeof(Query));
			ql->query->field = "lastName";
			ql->query->query = "Куликов";
			ql->condition = 1;
			break;

		case 3:
			printf("\n--------------------------------------\nfirstName=\"Алексей\" AND lastName=\"Куликов\"\n");
			ql->query = (Query*)calloc(1, sizeof(Query));
			ql->query->field = "firstName";
			ql->query->query = "Алексей";
			ql->query->condition = 0;

			ql->next = (QueryList*)calloc(1, sizeof(QueryList));
			ql = ql->next;

			ql->query = (Query*)calloc(1, sizeof(Query));
			ql->query->field = "lastName";
			ql->query->query = "Куликов";
			ql->condition = 0;
			break;

		case 4:
			printf("\n--------------------------------------\nfirstName=\"Алексей\" OR lastName=\"Куликов\" NOT middleName=\"Бориславович\"\n");
			ql->query = (Query*)calloc(1, sizeof(Query));
			ql->query->field = "firstName";
			ql->query->query = "Алексей";
			ql->query->condition = 0;

			ql->next = (QueryList*)calloc(1, sizeof(QueryList));
			ql = ql->next;

			ql->query = (Query*)calloc(1, sizeof(Query));
			ql->query->field = "lastName";
			ql->query->query = "Куликов";
			ql->condition = 1;

			ql->next = (QueryList*)calloc(1, sizeof(QueryList));
			ql = ql->next;

			ql->query = (Query*)calloc(1, sizeof(Query));
			ql->query->field = "middleName";
			ql->query->query = "Бориславович";
			ql->condition = 2;
			break;
		}

		DataList *res = query(queryList);
		db_PrintList(res);
		printf("--------------------------------------\n");

		dataList_ListFree(&res);
		clearQueryList(queryList);

		i++;
		//		scanf("%s", str);
	}

	free(str);
	db_Free();

	system("pause");

	return 0;
}

