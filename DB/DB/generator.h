#pragma once

#include <stdlib.h>
#include <string.h>

#include "time.h"
#include "math.h"

#include "db.h"
#include "dataList.h"

typedef struct StrUd {
	char *str = NULL;
	bool ud = false;
} StrUd;

DataList *manNames;
DataList *womanNames;
DataList *lastNames;
int manNamesCount = 0;
int womanNamesCount = 0;
int lastNamesCount = 0;

char *strConcat(char *dest, const char *source) {
	dest = (char*)realloc(dest, (strlen(dest) + strlen(source) + 1) * sizeof(char));
	return strcat(dest, source);
}

char *left(char *word, size_t i) {
	size_t j = strlen(word);
	j = i < j ? i : j;
	char *res = NULL;
	if (j > 0) {
		res = (char*)calloc(j + 1, sizeof(char));
		strncpy(res, word, j);
	}

	return res;
}

char *right(char *word, size_t i) {
	size_t j = strlen(word);
	size_t k = i < j ? j - i : 0;
	char *res = NULL;
	if (j > 0) {
		res = (char*)calloc(j - k + 1, sizeof(char));
		strcpy(res, &word[k]);
	}

	return res;
}

char *getFirstName(bool sex) {
	DataList *list = NULL;
	int i = -1, j = -1;

	if (sex) {
		if (manNamesCount == 0) {
			return NULL;
		}
		//������� ���
		list = manNames;
		i = rand() % manNamesCount;
	}
	else {
		//������� ���
		if (womanNamesCount == 0) {
			return NULL;
		}
		list = womanNames;
		i = rand() % womanNamesCount;
	}

	j = 0;
	while (i > j && list != NULL) {
		list = list->next;
		j++;
	}

	if (list != NULL) {
		char *str = NULL;
		if (sex) {
			str = (char*)calloc(strlen(((StrUd*)list->data)->str) + 1, sizeof(char));
			str = strcpy(str, ((StrUd*)list->data)->str);
		}
		else {
			str = (char*)calloc(strlen((char*)list->data) + 1, sizeof(char));
			str = strcpy(str, (char*)list->data);
		}

		return str;
	}
	else {
		return NULL;
	}
}

StrUd *getFirstNameWithUd() {
	if (manNamesCount == 0) {
		return NULL;
	}

	DataList *list = manNames;
	int i;
	i = rand() % manNamesCount;

	int j = 0;
	while (i > j && list != NULL) {
		list = list->next;
		j++;
	}

	return (StrUd*)list->data;
}

char *getMiddleName(bool sex) {
	if (manNamesCount == 0) {
		return NULL;
	}

	StrUd *strL = getFirstNameWithUd();	//����������� ��� � ���������;
	size_t l = strlen(strL->str);
	char *str = (char*)calloc(l + 1, sizeof(char));
	strcpy(str, strL->str);
	bool ud = strL->ud;
	strL = NULL;

	if (l == 0) {
		throw "�� ������� �������� ���.";
		return NULL;
	}

	if (l > 1) {
		switch (str[l - 1]) {
		case '�':
			if (ud) {
				str = strConcat(left(str, l - 1), sex ? "����" : "����");
			}
			else {
				if (str[l - 2] == '�') {
					if (l > 3) {
						if ((str[l - 4] == '�' || str[l - 4] == '�' || str[l - 4] == '�' || str[l - 4] == '�' || str[l - 4] == '�' || str[l - 4] == '�' || str[l - 4] == '�' || str[l - 4] == '�' || str[l - 4] == '�' || str[l - 4] == '�' ||
							(str[l - 4] == '�' && str[l - 3] == '�')) &&
							(str[l - 3] != '�' || str[l - 3] != '�' || str[l - 3] != '�')) {

							str = strConcat(left(str, l - 2), sex ? "�����" : "�����");
						}
						else {
							str = strConcat(left(str, l - 1), sex ? "����" : "����");
						}
					}
					else if (l > 2) {
						str = strConcat(left(str, l - 1), sex ? "����" : "����");
					}
					else {
						str = strConcat(str, sex ? "����" : "����");
					}
				}
				else {
					str = strConcat(left(str, l - 1), sex ? "����" : "����");
				}
			}
			break;

		case '�':
			str = strConcat(left(str, l - 1), sex ? "����" : "����");
			break;

		case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':
			str = strConcat(str, sex ? "����" : "����");
			break;

		case '�':case '�':case '�':case '�':case '�':
			str = strConcat(str, sex ? "����" : "����");
			break;

		case '�':case '�':case '�':case '�':case '�':case '�': case '�': case '�': case '�': case '�':
			if (l > 1) {
				if (str[l - 2] == '�' && str[l - 1] == '�' || str[l - 2] == '�' && str[l - 1] == '�' || str[l - 2] == '�' && str[l - 1] == '�' || str[l - 2] == '�' && str[l - 1] == '�' || str[l - 2] == '�' && str[l - 1] == '�') {
					str = strConcat(str, sex ? "����" : "����");
				}
				else if (str[l - 2] == '�' && str[l - 1] == '�' || str[l - 2] == '�' && str[l - 1] == '�') {
					str = strConcat(left(str, l - 1), sex ? "����" : "����");
				}
				else if (ud) {
					str = strConcat(str, sex ? "����" : "����");
				}
				else {
					if (str[l - 2] == '�' || str[l - 2] == '�' || str[l - 2] == '�' || str[l - 2] == '�' || str[l - 2] == '�') {
						str = strConcat(left(str, l - 1), sex ? "����" : "����");
					}
					else {
						switch (str[l - 1]) {
						case '�':case '�':case '�':case '�':
							if (strcmp("�������", str) == 0 || strcmp("������", str) == 0 || strcmp("����", str) == 0 || strcmp("�����", str) == 0 || strcmp("����", str) == 0 || strcmp("����", str) == 0) {
								str = strConcat(left(str, l - 1), sex ? "��" : "����");
							}
							else {
								str = strConcat(left(str, l - 1), sex ? "����" : "����");
							}
							break;

						case '�':
							str = strConcat(left(str, l - 1), sex ? "����" : "����");
							break;

						case '�':
							str = strConcat(left(str, l - 1), sex ? "����" : "����");
							break;

						default:
							str = strConcat(left(str, l - 1), sex ? "����" : "����");
							break;
						}
					}
				}

			}
			else {
				str = strConcat(str, sex ? "����" : "����");
			}
			break;
		}
	}

	return str;
}

char *getLastName(bool sex) {
	if (lastNamesCount == 0) {
		return NULL;
	}

	DataList *list = lastNames;
	int i, j;
	char *name = NULL;

	i = rand() % lastNamesCount;
	j = 0;
	while (i > j && list != NULL) {
		list = list->next;
		j++;
	}

	if (list != NULL) {
		name = (char*)calloc(strlen((char*)list->data) + 1, sizeof(char));	//����� ������ +1 ������ �����������
		name = strcpy(name, (char*)list->data);

		if (!sex) {
			//���������� � ������� �������
			size_t l = strlen(name);
			char *_last2 = right(name, 2);
			char *_last4 = right(name, 4);

			if (strcmp(_last2, "��") == 0 || strcmp(_last2, "��") == 0) {
				name = strConcat(name, "�");
			}
			else if (strcmp(_last2, "��") == 0) {
				char *s = left(name, l - 2);
				free(name);
				name = s;
				name = strConcat(name, "�");
			}
			else if (strcmp(_last4, "����") == 0) {
				char *s = left(name, l - 2);
				free(name);
				name = s;
				name = strConcat(name, "��");
			}

			free(_last2);
			free(_last4);
		}
	}

	return name;
}

DataList *readStrListFromFile(char *fileName, bool getUd) {
	if (strlen(fileName) > 0) {
		FILE *f;
		f = fopen(fileName, "r");

		if (f != NULL) {
			char str[255];
			int ud;
			DataList *list = NULL;
			DataList *last = NULL;

			while (fscanf(f, (getUd ? "%s\t%d\n" : "%s\n"), str, &ud) != EOF) {
				size_t l = strlen(str);
				void *data = NULL;
				if (getUd) {
					data = calloc(1, sizeof(StrUd));
					((StrUd*)data)->str = (char*)calloc(l + 1, sizeof(char));
					((StrUd*)data)->str = strcpy(((StrUd*)data)->str, str);
					((StrUd*)data)->ud = ud;
				}
				else {
					data = (char*)calloc(l + 1, sizeof(char));
					data = strcpy((char*)data, str);
				}

				last = dataList_Put(&list, data, last);
			}
			fclose(f);

			return list;
		}
		else {
			throw "�� ���� ������� ����";
			return NULL;
		}
	}
	else {
		throw "�� �������� ��� �����";
		return NULL;
	}
}

void initGenerator() {
	debug_Log("������������� ����������...");

	srand((int)time(NULL));	//������������� ���������� ���������������� ����� 

	manNamesCount = 0;
	womanNamesCount = 0;
	lastNamesCount = 0;

	manNames = readStrListFromFile("manNames.txt", true);
	manNamesCount = dataList_Length(manNames);
	womanNames = readStrListFromFile("womanNames.txt", false);
	womanNamesCount = dataList_Length(womanNames);
	lastNames = readStrListFromFile("lastNames.txt", false);
	lastNamesCount = dataList_Length(lastNames);

	debug_Log("��������� ���������������.");
}

void _freeStrUdData(void **data) {
	if (data && *data) {
		free(((StrUd*)*data)->str);
		((StrUd*)*data)->str = NULL;
		free(*data);
		*data = NULL;
	}
}

void _freeStrData(void **data) {
	if (data && *data) {
		free((char*)*data);
		*data = NULL;
	}
}

void freeGenerator() {
	debug_Log("�������� ����������...");

	dataList_ListFree(&manNames, _freeStrUdData);
	dataList_ListFree(&womanNames, _freeStrData);
	dataList_ListFree(&lastNames, _freeStrData);

	manNamesCount = 0;
	womanNamesCount = 0;
	lastNamesCount = 0;

	debug_Log("��������� �����.");
}

Data *generateData() {
	Data *data = (Data*)calloc(1, sizeof(Data));

	data->sex = (rand() % 100) > 50;
	data->firstName = getFirstName(data->sex);
	data->middleName = getMiddleName(data->sex);
	data->lastName = getLastName(data->sex);
	data->age = 18 + (rand() % 70);
	data->weight = 40 + (rand() % 70);
	data->group = rand() % 10;

	return data;
}

DataList *generateDataList(int count) {
	DataList *list = NULL;
	debug_Log("�������� %d ���������...", count);
	DataList *last = NULL;
	for (int i = 0; i < count; i++) {
		last = dataList_Put(&list, generateData(), last);
	}
	debug_Log("�������� �������.", count);

	return list;
}

void saveDataToFile(char *fileName, DataList *dataList) {
	if (strlen(fileName) > 0) {
		debug_Log("��������� ������ � ���� \"%s\"...", fileName);

		FILE *f;
		f = fopen(fileName, "w");
		if (f != NULL) {
			DataList *list = dataList;
			while (list != NULL) {
				fprintf(f, "%s\t%s\t%s\t%d\t%d\t%d\t%d\n",
					((Data*)list->data)->firstName,
					((Data*)list->data)->middleName,
					((Data*)list->data)->lastName,
					((Data*)list->data)->sex ? 1 : 0,
					((Data*)list->data)->age,
					((Data*)list->data)->weight,
					((Data*)list->data)->group);
				list = list->next;
			}

			fclose(f);
			debug_Log("������ �������.");
		}
		else {
			throw "�� ���� ������� ����";
		}
	}
	else {
		throw "�� �������� ��� �����";
	}
}

DataList *loadDataFromFile(char *fileName) {
	if (strlen(fileName) > 0) {
		debug_Log("��������� ������ �� ����� \"%s\"", fileName);
		FILE *f;
		f = fopen(fileName, "r");
		if (f != NULL) {
			DataList *list = NULL;
			DataList *last = NULL;

			char firstName[255], middleName[255], lastName[255];
			int sex, age, weight, group;
			while (fscanf(f, "%s\t%s\t%s\t%d\t%d\f%d\t%d\n", firstName, middleName, lastName, &sex, &age, &weight, &group) != EOF) {
				Data *data = (Data*)calloc(1, sizeof(Data));
				data->firstName = (char*)calloc(strlen(firstName) + 1, sizeof(char));
				strcpy(data->firstName, firstName);

				data->middleName = (char*)calloc(strlen(middleName) + 1, sizeof(char));
				strcpy(data->middleName, middleName);

				data->lastName = (char*)calloc(strlen(lastName) + 1, sizeof(char));
				strcpy(data->lastName, lastName);

				data->sex = sex == 1;
				data->age = age % 255;
				data->weight = weight % 255;
				data->group = group % 255;

				last = dataList_Put(&list, data, last);
			}

			fclose(f);
			debug_Log("������ ������.");

			return list;
		}
		else {
			throw "�� ���� ������� ����";
			return NULL;
		}
	}
	else {
		throw "�� �������� ��� �����";
		return NULL;
	}
}