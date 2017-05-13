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

char *strConcat(char *dest, const char *source) {
	dest = (char*)realloc(dest, (strlen(dest) + strlen(source) + 1) * sizeof(char));
	return strcat(dest, source);;
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

			str = freeAndNULLStr(str);
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
		char *str = (char*)calloc(strlen(tmp->str), sizeof(char));
		strcpy(str, tmp->str);
		return str;
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

	return tmp;
}

char *getMiddleName(bool sex) {
	StrList *strL = getFirstNameWithUd(true);	//����������� ��� � ���������;
	size_t l = strlen(strL->str);
	char *str = (char*)calloc(l + 1, sizeof(char));
	strcat(str, strL->str);
	bool ud = strL->ud;

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
		name = strcpy(name, tmp->str);

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
				name = freeAndNULLStr(name);
				name = s;
				name = strConcat(name, "�");
			}
			else if (strcmp(_last4, "����") == 0) {
				char *s = left(name, l - 2);
				name = freeAndNULLStr(name);
				name = s;
				name = strConcat(name, "��");
			}

			_last2 = freeAndNULLStr(_last2);
			_last4 = freeAndNULLStr(_last4);
		}
	}

	return name;
}

void initGenerator() {
	srand((int)time(NULL));	//������������� ���������� ���������������� ����� 

	manNamesCount = 0;
	womanNamesCount = 0;
	lastNamesCount = 0;

	manNames = readStrListFromFile("manNames.txt", true);
	manNamesCount = getListLength(manNames);
	womanNames = readStrListFromFile("womanNames.txt", false);
	womanNamesCount = getListLength(womanNames);
	lastNames = readStrListFromFile("lastNames.txt", false);
	lastNamesCount = getListLength(lastNames);
}

/*void fillChar(char *word) {
	if (word != NULL) {
		for (size_t i = 0; i < strlen(word); i++) {
			word[i] = '*';
		}
	}
}*/

void freeGenerator() {
	while (manNames != NULL) {
		StrList *tmp = manNames;
		manNames = manNames->next;

		tmp->str = freeAndNULLStr(tmp->str);
		tmp->next = NULL;
		free(tmp);
	}

	while (womanNames != NULL) {
		StrList *tmp = womanNames;
		womanNames = womanNames->next;

		tmp->str = freeAndNULLStr(tmp->str);
		tmp->next = NULL;
		free(tmp);
	}

	while (lastNames != NULL) {
		StrList *tmp = lastNames;
		lastNames = lastNames->next;

		tmp->str = freeAndNULLStr(tmp->str);
		tmp->next = NULL;
		free(tmp);
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