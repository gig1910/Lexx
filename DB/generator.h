#pragma once

#include "time.h"
#include "math.h"

#include "db.h"
#include "dataList.h"

typedef struct StrList {
	char *str;
	bool ud;
	struct StrList *next;
} StrList;

StrList *manNames;
StrList *womanNames;
StrList *lastNames;
int manNamesCount = 0;
int womanNamesCount = 0;
int lastNamesCount = 0;

char *left(char *word, size_t i) {
	size_t j = strlen(word);
	j = i < j ? i : j;
	char *res = NULL;
	if (j > 0) {
		res = (char*)calloc(j + 1, sizeof(char));
		memcpy(res, word, j);
		res[j + 1] = 0;
	}

	return res;
}

char *right(char *word, size_t i) {
	size_t j = strlen(word);
	size_t k = i < j ? j - i : 0;
	char *res = NULL;
	if (j > 0) {
		res = (char*)calloc(k + 1, sizeof(char));
		memcpy(res, &word[k], k + 1);
	}

	return res;
}

StrList *readStrListFromFile(char *fileName, bool getUd) {
	if (strlen(fileName) > 0) {
		FILE *f;
		f = fopen(fileName, "r");

		if (f != NULL) {
			char *str;
			int ud;
			StrList *root = NULL;
			StrList *tmp = NULL;
			str = (char*)calloc(255, sizeof(char));
			while (fscanf(f, (getUd ? "%s\t%d\n" : "%s\n"), str, &ud) != EOF) {
				if (tmp == NULL) {
					root = (StrList*)calloc(1, sizeof(StrList));
					tmp = root;
				}
				else {
					tmp->next = (StrList*)calloc(1, sizeof(StrList));
					tmp = tmp->next;
				}
				size_t l = strlen(str);
				tmp->str = (char*)calloc(l + 1, sizeof(char));
				strcpy(tmp->str, str);
				tmp->ud = getUd ? ud == 1 : false;
			}

			free(str);
			str = NULL;
			fclose(f);
			f = NULL;

			return root;
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

int getListLength(StrList *root) {
	int count = 0;
	StrList *tmp = root;
	while (tmp != NULL) {
		count++;
		tmp = tmp->next;
	}

	return count;
}

char *getFirstName(bool sex) {
	StrList *tmp;
	int i, j;
	if (sex) {
		//������� ���
		tmp = manNames;
		i = rand() % manNamesCount;
	}
	else {
		//������� ���
		tmp = womanNames;
		i = rand() % womanNamesCount;
	}

	j = 0;
	while (i > j && tmp != NULL) {
		tmp = tmp->next;
		j++;
	}

	if (tmp != NULL) {
		char *name = (char*)calloc(strlen(tmp->str) + 1, sizeof(char));
		strcpy(name, tmp->str);
		return name;
	}
	else {
		return NULL;
	}
}

StrList *getFirstNameWithUd(bool sex) {
	StrList *tmp = NULL;
	int i, j;
	if (sex) {
		//������� ���
		tmp = manNames;
		i = rand() % manNamesCount;
	}
	else {
		//������� ���
		tmp = womanNames;
		i = rand() % womanNamesCount;
	}

	j = 0;
	while (i > j && tmp != NULL) {
		tmp = tmp->next;
		j++;
	}

	StrList *res = (StrList*)calloc(1, sizeof(StrList));
	if (tmp != NULL) {
		res->str = (char*)calloc(strlen(tmp->str) + 1, sizeof(char));
		strcpy(res->str, tmp->str);
		res->ud = tmp->ud;
	}

	return res;
}

char *getMiddleName(bool sex) {
	StrList *strL = getFirstNameWithUd(true);	//����������� ��� � ���������;
	char *tmp = strL->str;
	bool ud = strL->ud;

	strL->str = NULL;
	free(strL);
	strL = NULL;

	size_t l = strlen(tmp);

	if (l == 0) {
		throw "�� ������� �������� ���.";
		return NULL;
	}

	char *middleName = NULL;

	if (l > 1) {
		switch (tmp[l - 1]) {
		case '�':
			if (ud) {
				middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
			}
			else {
				if (tmp[l - 2] == '�') {
					if (l > 3) {
						if ((tmp[l - 4] == '�' || tmp[l - 4] == '�' || tmp[l - 4] == '�' || tmp[l - 4] == '�' || tmp[l - 4] == '�' || tmp[l - 4] == '�' || tmp[l - 4] == '�' || tmp[l - 4] == '�' || tmp[l - 4] == '�' || tmp[l - 4] == '�' ||
							(tmp[l - 4] == '�' && tmp[l - 3] == '�')) &&
							(tmp[l - 3] != '�' || tmp[l - 3] != '�' || tmp[l - 3] != '�')) {

							middleName = strcat(left(tmp, l - 2), sex ? "�����" : "�����");
						}
						else {
							middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
						}
					}
					else if (l > 2) {
						middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
					}
					else {
						middleName = strcat(tmp, sex ? "����" : "����");
					}
				}
				else {
					middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
				}
			}
			break;

		case '�':
			middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
			break;

		case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':case '�':
			middleName = strcat(tmp, sex ? "����" : "����");
			break;

		case '�':case '�':case '�':case '�':case '�':
			middleName = strcat(tmp, sex ? "����" : "����");
			break;

		case '�':case '�':case '�':case '�':case '�':case '�': case '�': case '�': case '�': case '�':
			if (l > 1) {
				if (tmp[l - 2] == '�' && tmp[l - 1] == '�' || tmp[l - 2] == '�' && tmp[l - 1] == '�' || tmp[l - 2] == '�' && tmp[l - 1] == '�' || tmp[l - 2] == '�' && tmp[l - 1] == '�' || tmp[l - 2] == '�' && tmp[l - 1] == '�') {
					middleName = strcat(tmp, sex ? "����" : "����");
				}
				else if (tmp[l - 2] == '�' && tmp[l - 1] == '�' || tmp[l - 2] == '�' && tmp[l - 1] == '�') {
					middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
				}
				else if (ud) {
					middleName = strcat(tmp, sex ? "����" : "����");
				}
				else {
					if (tmp[l - 2] == '�' || tmp[l - 2] == '�' || tmp[l - 2] == '�' || tmp[l - 2] == '�' || tmp[l - 2] == '�') {
						middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
					}
					else {
						switch (tmp[l - 1]) {
						case '�':case '�':case '�':case '�':
							if (strcmp("�������", tmp) == 0 || strcmp("������", tmp) == 0 || strcmp("����", tmp) == 0 || strcmp("�����", tmp) == 0 || strcmp("����", tmp) == 0 || strcmp("����", tmp) == 0) {
								middleName = strcat(left(tmp, l - 1), sex ? "��" : "����");
							}
							else {
								middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
							}
							break;

						case '�':
							middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
							break;

						case '�':
							middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
							break;

						default:
							middleName = strcat(left(tmp, l - 1), sex ? "����" : "����");
							break;
						}
					}
				}

			}
			else {
				middleName = strcat(tmp, sex ? "����" : "����");
			}
			break;
		}
	}

	return middleName;
}

char *getLastName(bool sex) {
	StrList *tmp = lastNames;
	int i, j;
	char *name = NULL;

	i = rand() % lastNamesCount;
	j = 0;
	while (i > j && tmp != NULL) {
		tmp = tmp->next;
		j++;
	}

	if (tmp != NULL) {
		name = (char*)calloc(strlen(tmp->str) + 1, sizeof(char));	//����� ������ +1 ������ �����������
		strcpy(name, tmp->str);

		if (!sex) {
			//���������� � ������� �������
			size_t l = strlen(name);
			char *_last2 = right(name, 2);
			char *_last4 = right(name, 4);

			if (strcmp(_last2, "��") == 0 || strcmp(_last2, "��") == 0) {
				name = strcat(name, "�");
			}
			else if (strcmp(_last2, "��") == 0) {
				name = strcat(left(name, l - 2), "��"); ; // (char*)calloc(l + 3, sizeof(char));
			}
			else if (strcmp(_last4, "����") == 0) {
				name = strcat(left(name, l - 2), "��"); ; // (char*)calloc(l + 1, sizeof(char));
			}
		}
	}

	return name;
}

void initGenerator() {
	srand((int)time(NULL));	//������������� ���������� ���������������� ����� 

	manNamesCount = 0;
	womanNamesCount = 0;
	lastNamesCount = 0;

	manNames = readStrListFromFile("C:\\Lexx\\DB\\manNames.txt", true);
	manNamesCount = getListLength(manNames);
	womanNames = readStrListFromFile("C:\\Lexx\\DB\\womanNames.txt", false);
	womanNamesCount = getListLength(womanNames);
	lastNames = readStrListFromFile("C:\\Lexx\\DB\\lastNames.txt", false);
	lastNamesCount = getListLength(lastNames);
}

void freeGenerator() {
	while (manNames != NULL) {
		StrList *tmp = manNames;
		manNames = manNames->next;
		free(tmp->str);
		tmp->str = NULL;
		free(tmp);
		tmp = NULL;
	}

	while (womanNames != NULL) {
		StrList *tmp = womanNames;
		womanNames = womanNames->next;
		free(tmp->str);
		tmp->str = NULL;
		free(tmp);
		tmp = NULL;
	}

	while (lastNames != NULL) {
		StrList *tmp = lastNames;
		lastNames = lastNames->next;
		free(tmp->str);
		tmp->str = NULL;
		free(tmp);
		tmp = NULL;
	}

	manNamesCount = -1;
	womanNamesCount = -1;
	lastNamesCount = -1;
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
	int i = 0;
	DataList *root = NULL;
	DataList *prev = NULL;
	while (i < count) {
		DataList *dataList = (DataList*)calloc(1, sizeof(DataList));
		if (prev != NULL) {
			prev->next = dataList;
		}
		else {
			if (root == NULL) {
				root = dataList;
			}
		}
		prev = dataList;
		dataList->data = generateData();
		i++;
	}

	return root;
}

void saveDataToFile(char *fileName, DataList *dataList) {
	if (strlen(fileName) > 0) {
		FILE *f;
		f = fopen(fileName, "w");
		if (f != NULL) {
			DataList *tmp = dataList;
			while (tmp != NULL) {
				fprintf(f, "%s\t%s\t%s\t%d\t%d\t%d\t%d\n",
					((Data*)tmp->data)->firstName,
					((Data*)tmp->data)->middleName,
					((Data*)tmp->data)->lastName,
					((Data*)tmp->data)->sex ? 1 : 0,
					((Data*)tmp->data)->age,
					((Data*)tmp->data)->weight,
					((Data*)tmp->data)->group);
				tmp = tmp->next;
			}

			fclose(f);
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
		FILE *f;
		f = fopen(fileName, "r");
		if (f != NULL) {
			DataList *root = NULL;
			DataList *prev = NULL;

			char firstName[255], middleName[255], lastName[255];
			int sex, age, weight, group;
			while (fscanf(f, "%s\t%s\t%s\t%d\t%d\f%d\t%d\n", firstName, middleName, lastName, &sex, &age, &weight, &group) != EOF) {
				DataList *tmp;
				tmp = (DataList*)calloc(1, sizeof(DataList));
				tmp->data = (Data*)calloc(1, sizeof(Data));

				((Data*)tmp->data)->firstName = (char*)calloc(strlen(firstName) + 1, sizeof(char));
				memcpy(((Data*)tmp->data)->firstName, firstName, strlen(firstName) + 1);

				((Data*)tmp->data)->middleName = (char*)calloc(strlen(middleName) + 1, sizeof(char));
				memcpy(((Data*)tmp->data)->middleName, middleName, strlen(middleName) + 1);

				((Data*)tmp->data)->lastName = (char*)calloc(strlen(lastName) + 1, sizeof(char));
				memcpy(((Data*)tmp->data)->lastName, lastName, strlen(lastName) + 1);

				((Data*)tmp->data)->sex = sex == 1;
				((Data*)tmp->data)->age = age % 255;
				((Data*)tmp->data)->weight = weight % 255;
				((Data*)tmp->data)->group = group % 255;

				if (prev == NULL) {
					root = tmp;
				}
				else {
					prev->next = tmp;
				}

				prev = tmp;
			}

			fclose(f);

			return root;
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