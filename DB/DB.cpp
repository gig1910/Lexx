// DB.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <clocale>

#include "generator.h"
#include "db.h"
#include "query.h"

int main()
{
	setlocale(LC_CTYPE, "rus");

	initGenerator();

	char str[255];
	int count;
	printf("Введите кол-во генерируемых записей или имя файла: ");
	scanf("%s", str);

	char* end_ptr;
	count = strtol(str, &end_ptr, 10);

	DataList *dataList;
	if (*end_ptr) {
		dataList = loadDataFromFile(str);
	}
	else {
		dataList = generateDataList(count);
		printf("Созранить сгененированный набор? [y, n] ");
		scanf("%s", str);
		if (str[0] == 'y') {
			printf("Введите имя файла: ");
			scanf("%s", str);
			saveDataToFile(str, dataList);
		}
	}
	freeGenerator();

	initDB();

	DataList *dl = dataList;
	while (dl != NULL) {
		putData(((Data*)dl->data));
		dl = dl->next;
	}
	clearDataList(dataList, false);

	Data *query = (Data*)calloc(1, sizeof(Data));
	query->firstName = "Алексей";
	printDataList(queryData(query));
	printf("--------------------------------------\n");
	scanf("%s", end_ptr);

	clearDB();

	return 0;
}

