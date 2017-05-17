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
		//Мужское имя
		list = manNames;
		i = rand() % manNamesCount;
	}
	else {
		//Женское имя
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

	StrUd *strL = getFirstNameWithUd();	//Запрашиваем имя с ударением;
	size_t l = strlen(strL->str);
	char *str = (char*)calloc(l + 1, sizeof(char));
	strcpy(str, strL->str);
	bool ud = strL->ud;
	strL = NULL;

	if (l == 0) {
		throw "Не удалось получить имя.";
		return NULL;
	}

	if (l > 1) {
		switch (str[l - 1]) {
		case 'й':
			if (ud) {
				str = strConcat(left(str, l - 1), sex ? "евич" : "евна");
			}
			else {
				if (str[l - 2] == 'и') {
					if (l > 3) {
						if ((str[l - 4] == 'а' || str[l - 4] == 'е' || str[l - 4] == 'ё' || str[l - 4] == 'и' || str[l - 4] == 'о' || str[l - 4] == 'у' || str[l - 4] == 'ы' || str[l - 4] == 'э' || str[l - 4] == 'ю' || str[l - 4] == 'я' ||
							(str[l - 4] == 'н' && str[l - 3] == 'т')) &&
							(str[l - 3] != 'к' || str[l - 3] != 'х' || str[l - 3] != 'ц')) {

							str = strConcat(left(str, l - 2), sex ? "ьевич" : "ьевна");
						}
						else {
							str = strConcat(left(str, l - 1), sex ? "евич" : "евна");
						}
					}
					else if (l > 2) {
						str = strConcat(left(str, l - 1), sex ? "евич" : "евна");
					}
					else {
						str = strConcat(str, sex ? "евич" : "евна");
					}
				}
				else {
					str = strConcat(left(str, l - 1), sex ? "евич" : "евна");
				}
			}
			break;

		case 'ь':
			str = strConcat(left(str, l - 1), sex ? "евич" : "евна");
			break;

		case 'б':case 'в':case 'г':case 'д':case 'з':case 'к':case 'л':case 'м':case 'н':case 'п':case 'р':case 'с':case 'т':case 'ф':case 'х':
			str = strConcat(str, sex ? "ович" : "овна");
			break;

		case 'ж':case 'ш':case 'ц':case 'ч':case 'щ':
			str = strConcat(str, sex ? "ович" : "овна");
			break;

		case 'а':case 'е':case 'ё':case 'и':case 'о':case 'у': case 'ы': case 'э': case 'ю': case 'я':
			if (l > 1) {
				if (str[l - 2] == 'а' && str[l - 1] == 'а' || str[l - 2] == 'а' && str[l - 1] == 'у' || str[l - 2] == 'е' && str[l - 1] == 'у' || str[l - 2] == 'э' && str[l - 1] == 'э' || str[l - 2] == 'у' && str[l - 1] == 'у') {
					str = strConcat(str, sex ? "евич" : "евна");
				}
				else if (str[l - 2] == 'е' && str[l - 1] == 'я' || str[l - 2] == 'и' && str[l - 1] == 'я') {
					str = strConcat(left(str, l - 1), sex ? "евич" : "евна");
				}
				else if (ud) {
					str = strConcat(str, sex ? "евич" : "евна");
				}
				else {
					if (str[l - 2] == 'ж' || str[l - 2] == 'ш' || str[l - 2] == 'ч' || str[l - 2] == 'щ' || str[l - 2] == 'ц') {
						str = strConcat(left(str, l - 1), sex ? "евич" : "евна");
					}
					else {
						switch (str[l - 1]) {
						case 'а':case 'у':case 'о':case 'ы':
							if (strcmp("Аникита", str) == 0 || strcmp("Никита", str) == 0 || strcmp("Мина", str) == 0 || strcmp("Савва", str) == 0 || strcmp("Сила", str) == 0 || strcmp("Фока", str) == 0) {
								str = strConcat(left(str, l - 1), sex ? "ич" : "ична");
							}
							else {
								str = strConcat(left(str, l - 1), sex ? "ович" : "овна");
							}
							break;

						case 'е':
							str = strConcat(left(str, l - 1), sex ? "евич" : "евна");
							break;

						case 'и':
							str = strConcat(left(str, l - 1), sex ? "евич" : "евна");
							break;

						default:
							str = strConcat(left(str, l - 1), sex ? "евич" : "евна");
							break;
						}
					}
				}

			}
			else {
				str = strConcat(str, sex ? "евич" : "евна");
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
		name = (char*)calloc(strlen((char*)list->data) + 1, sizeof(char));	//Длина строки +1 символ терминатора
		name = strcpy(name, (char*)list->data);

		if (!sex) {
			//Превращаем в женскую фамилию
			size_t l = strlen(name);
			char *_last2 = right(name, 2);
			char *_last4 = right(name, 4);

			if (strcmp(_last2, "ов") == 0 || strcmp(_last2, "ев") == 0) {
				name = strConcat(name, "а");
			}
			else if (strcmp(_last2, "ой") == 0) {
				char *s = left(name, l - 2);
				free(name);
				name = s;
				name = strConcat(name, "а");
			}
			else if (strcmp(_last4, "ский") == 0) {
				char *s = left(name, l - 2);
				free(name);
				name = s;
				name = strConcat(name, "ая");
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
			throw "Не смог открыть файл";
			return NULL;
		}
	}
	else {
		throw "не передано имя файла";
		return NULL;
	}
}

void initGenerator() {
	debug_Log("Инициализация генератора...");

	srand((int)time(NULL));	//Инициализация ценератора псевдослучайного числа 

	manNamesCount = 0;
	womanNamesCount = 0;
	lastNamesCount = 0;

	manNames = readStrListFromFile("manNames.txt", true);
	manNamesCount = dataList_Length(manNames);
	womanNames = readStrListFromFile("womanNames.txt", false);
	womanNamesCount = dataList_Length(womanNames);
	lastNames = readStrListFromFile("lastNames.txt", false);
	lastNamesCount = dataList_Length(lastNames);

	debug_Log("Генератор инициализирован.");
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
	debug_Log("Удаление генератора...");

	dataList_ListFree(&manNames, _freeStrUdData);
	dataList_ListFree(&womanNames, _freeStrData);
	dataList_ListFree(&lastNames, _freeStrData);

	manNamesCount = 0;
	womanNamesCount = 0;
	lastNamesCount = 0;

	debug_Log("Генератор удалён.");
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
	debug_Log("Создание %d элементов...", count);
	DataList *last = NULL;
	for (int i = 0; i < count; i++) {
		last = dataList_Put(&list, generateData(), last);
	}
	debug_Log("Элементы созданы.", count);

	return list;
}

void saveDataToFile(char *fileName, DataList *dataList) {
	if (strlen(fileName) > 0) {
		debug_Log("Сохраняем список в файл \"%s\"...", fileName);

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
			debug_Log("Список сохранён.");
		}
		else {
			throw "Не могу моздать файл";
		}
	}
	else {
		throw "Не передано имя файла";
	}
}

DataList *loadDataFromFile(char *fileName) {
	if (strlen(fileName) > 0) {
		debug_Log("Считываем список из файла \"%s\"", fileName);
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
			debug_Log("Список считан.");

			return list;
		}
		else {
			throw "Не могу создать файл";
			return NULL;
		}
	}
	else {
		throw "Не передано имя файла";
		return NULL;
	}
}