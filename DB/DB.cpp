// DB.cpp: ���������� ����� ����� ��� ����������� ����������.
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
	printf("������� ���-�� ������������ ������� ��� ��� �����: ");
	scanf("%s", str);

	char* end_ptr;
	count = strtol(str, &end_ptr, 10);

	DataList *dataList;
	if (*end_ptr) {
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

	initDB();

	DataList *d = dataList;
	while (d != NULL) {
		putData(((Data*)d->data));
		d = d->next;
	}
	clearDataList(dataList, false);

	Data *query = (Data*)calloc(1, sizeof(Data));
	query->firstName = "�������";
	printDataList(queryData(query));
	printf("--------------------------------------");
	scanf("%s", end_ptr);

	clearDB();

	return 0;
}

