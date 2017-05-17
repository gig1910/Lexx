#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct DataList {
	void *data;
	struct DataList *next;
} DataList;

typedef void(*DataFreeFunc)(void **data);
typedef bool(*DataFindFunc)(void *data, void *findData);
typedef void(*DataPrintFunc)(void *data);

/**
 * Создание элемента списка
 * @param {void*} data - Указатель на данные, хранимые в списке
 * @returns {DataList*}
 */
DataList *dataList_Create(void *data) {
	DataList *d = (DataList*)calloc(1, sizeof(DataList));
	d->next = NULL;
	d->data = data;
	return d;
}

/**
* Функция очистки элемента списка
* @param {DataList**} root		- Указатель элемент списка
* @param {?DataFreeFunc*} func	- Указатель на callback функцию очистки данных списка (если NULL То очищаеся просто список. Данные не трогаются);
*/
DataList* dataList_Free(DataList **dataList, DataFreeFunc func) {
	DataList *next = NULL;
	if (dataList && *dataList) {
		if (func) {
			func(&(*dataList)->data);
		}
		(*dataList)->data = NULL;
		next = (*dataList)->next;
		(*dataList)->next = NULL;

		free(*dataList);
		*dataList = NULL;
	}

	return next;
}

/**
* Функция очистки элемента списка
* @param {DataList**} root		- Указатель элемент списка
*/
DataList* dataList_Free(DataList **dataList) {
	DataList *next = NULL;
	if (dataList && *dataList) {
		(*dataList)->data = NULL;
		next = (*dataList)->next;
		(*dataList)->next = NULL;

		free(*dataList);
		*dataList = NULL;
	}

	return next;
}

/**
* Функция очистки списка
* @param {DataList**} root		- Указатель на начало списка
* @param {?DataFreeFunc*} func	- Указатель на callback функцию очистки данных списка (если NULL То очищаеся просто список. Данные не трогаются);
*/
void dataList_ListFree(DataList **root, DataFreeFunc func) {
	if (root) {
		while (*root) {
			*root = dataList_Free(root, func);
		}
	}
}

/**
* Функция очистки списка
* @param {DataList**} root		- Указатель на начало списка
*/
void dataList_ListFree(DataList **root) {
	if (root) {
		while (*root) {
			*root = dataList_Free(root);
		}
	}
}

/**
* Добавление данных в конец списка
* @param {DataList**} root	- Указатель на начало списка
* @param {void*} data		- Указатель на добавляемые данные
* @returns {DataList*}
*/
DataList *dataList_Put(DataList **root, void *data) {
	if (root) {
		if (*root) {
			DataList *node = NULL;
			node = *root;
			while (node->next) {
				node = node->next;
			}
			node->next = dataList_Create(data);
			return node->next;
		}
		else {
			*root = dataList_Create(data);
			return *root;
		}
	}
	else {
		return dataList_Create(data);
	}
}

/**
* Копирование списка
* @param {void*} root - Указатель на начало копируемого списка
* @returns {DataList*}
*/
DataList *dataList_Copy(DataList *root) {
	DataList *copy = NULL;
	DataList *node = root;
	while (node) {
		dataList_Put(&copy, node->data);
		node = node->next;
	}

	return copy;
}

/**
* Вставка данных в список
* @param {DataList**} root	- Указатель на начало списка
* @param {void*} data		- Указатель на добавляемые данные
* @param {DataList**} after	- Указатель на элемент списка, после которого добавляем данные
* @returns {DataList*}
*/
DataList *dataList_Put(DataList **root, void *data, DataList *after) {
	if (root) {
		if (*root) {
			DataList *node = NULL;
			if (after) {
				node = after->next;
				after->next = dataList_Create(data);
				after->next->next = node;
				node = after->next;
			}
			else {
				node = *root;
				while (node->next) {
					node = node->next;
				}
				node->next = dataList_Create(data);
				node = node->next;
			}

			return node;
		}
		else {
			*root = dataList_Create(data);
			return *root;
		}
	}
	else {
		return dataList_Create(data);
	}
}

/**
 * Удаление данных из списка
 * @param {DataList**} root - Указатель на наало списка
 * @param {void*} data		- Указатель на удаляемые данные
 * @returns {?DataList*}
 */
DataList *dataList_Remove(DataList **root, void *data) {
	if (root) {
		if (*root) {
			DataList *node = *root;
			DataList *prev = NULL;
			DataList *tmp;

			while (node) {
				if (node->data == data) {
					if (node == *root) {
						tmp = *root;
						*root = node->next;
						node = *root;
						prev = NULL;

						tmp->data = NULL;
						free(tmp);
					}
					else {
						prev->next = node->next;

					}
				}

				if (node->data == data) {
					if (prev) {
						prev->next = node->next;

						node->next = NULL;
						node->data = NULL;
						free(node);

						node = prev->next;
					}
					else if (node->next) {
						*root = node->next;

						node->next = NULL;
						node->data = NULL;
						free(node);

						node = *root;
					}
					else {
						prev = node;
						node = node->next;
					}
				}
				else {
					prev = node;
					node = node->next;
				}
			}
		}
	}

	return *root;
}

/**
 * Функция поиска данных в списке по совпадению указателя на данные
 * @param {DataList*} root - Указатель на начало списка
 * @param {void*} data - Указатель на данные
 * @returns {?DataList*}
 */
DataList *dataList_Find(DataList *root, void *data) {
	DataList *node = root;
	while (node && node->data != data) {
		node = node->next;
	}

	return node;
}

/**
* Функция поиска данных в списке с вызовом функции критерия отбора
* @param {DataList*} root - Указатель на начало списка
* @param {DataFindFunc*} func - Указатель на callback функцибю поиска
* @param {void*} data - Указатель на данные поиска
* @returns {?DataList*}
*/
DataList *dataList_Find(DataList *root, DataFindFunc func, void *data) {
	DataList *node = root;
	while (node && !func(node->data, data)) {
		node = node->next;
	}

	return node;
}

int dataList_Length(DataList *root) {
	int i = 0;
	DataList *node = root;
	while (node) {
		node = node->next;
		i++;
	}

	return i;
}

//**********************************************************************

/**
 * Функция объединения двух списков
 * @param {DataList**} dest - Указатель на первый список. Куда быдыт добавляться данные
 * @param {DataList} source - Указатель на второй список, откуда будут добавляться данные
 * @returns {DataList*}
 */
DataList *dataList_Union(DataList **dest, DataList *source) {
	if (dest && *dest) {
		DataList *node = source;
		while (node) {
			if (!dataList_Find(*dest, node->data)) {
				dataList_Put(dest, node->data);
			}
			node = node->next;
		}
		return *dest;
	}
	else {
		return source;
	}
}

/**
* Функция вычитания из первого списка содержимого второго
* @param {DataList**} dest - Указатель на первый список.
* @param {DataList} source - Указатель на второй список
* @returns {DataList*}
*/
DataList *dataList_Subtraction(DataList **dest, DataList *source) {
	if (dest && *dest) {
		if (source) {
			DataList *node = source;
			while (node && *dest) {
				if (dataList_Find(*dest, node->data)) {
					dataList_Remove(dest, node->data);
				}
				node = node->next;
			}
			return *dest;
		}
		else {
			return *dest;
		}
	}
	else {
		return NULL;
	}
}

/**
* Функция пересечения первого и второго списка (внутреннее пересечение)
* @param {DataList**} dest - Указатель на первый список.
* @param {DataList} source - Указатель на второй список
* @returns {DataList*}
*/
DataList *dataList_Merge(DataList **dest, DataList *source) {
	if (dest && *dest && source) {
		DataList *node = source;
		while (node && *dest) {
			if (!dataList_Find(*dest, node->data)) {
				dataList_Remove(dest, node->data);
			}
			node = node->next;
		}
		return *dest;
	}
	else {
		return NULL;
	}
}