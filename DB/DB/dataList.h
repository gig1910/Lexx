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
 * �������� �������� ������
 * @param {void*} data - ��������� �� ������, �������� � ������
 * @returns {DataList*}
 */
DataList *dataList_Create(void *data) {
	DataList *d = (DataList*)calloc(1, sizeof(DataList));
	d->next = NULL;
	d->data = data;
	return d;
}

/**
* ������� ������� �������� ������
* @param {DataList**} root		- ��������� ������� ������
* @param {?DataFreeFunc*} func	- ��������� �� callback ������� ������� ������ ������ (���� NULL �� �������� ������ ������. ������ �� ���������);
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
* ������� ������� �������� ������
* @param {DataList**} root		- ��������� ������� ������
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
* ������� ������� ������
* @param {DataList**} root		- ��������� �� ������ ������
* @param {?DataFreeFunc*} func	- ��������� �� callback ������� ������� ������ ������ (���� NULL �� �������� ������ ������. ������ �� ���������);
*/
void dataList_ListFree(DataList **root, DataFreeFunc func) {
	if (root) {
		while (*root) {
			*root = dataList_Free(root, func);
		}
	}
}

/**
* ������� ������� ������
* @param {DataList**} root		- ��������� �� ������ ������
*/
void dataList_ListFree(DataList **root) {
	if (root) {
		while (*root) {
			*root = dataList_Free(root);
		}
	}
}

/**
* ���������� ������ � ����� ������
* @param {DataList**} root	- ��������� �� ������ ������
* @param {void*} data		- ��������� �� ����������� ������
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
* ����������� ������
* @param {void*} root - ��������� �� ������ ����������� ������
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
* ������� ������ � ������
* @param {DataList**} root	- ��������� �� ������ ������
* @param {void*} data		- ��������� �� ����������� ������
* @param {DataList**} after	- ��������� �� ������� ������, ����� �������� ��������� ������
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
 * �������� ������ �� ������
 * @param {DataList**} root - ��������� �� ����� ������
 * @param {void*} data		- ��������� �� ��������� ������
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
 * ������� ������ ������ � ������ �� ���������� ��������� �� ������
 * @param {DataList*} root - ��������� �� ������ ������
 * @param {void*} data - ��������� �� ������
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
* ������� ������ ������ � ������ � ������� ������� �������� ������
* @param {DataList*} root - ��������� �� ������ ������
* @param {DataFindFunc*} func - ��������� �� callback �������� ������
* @param {void*} data - ��������� �� ������ ������
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
 * ������� ����������� ���� �������
 * @param {DataList**} dest - ��������� �� ������ ������. ���� ����� ����������� ������
 * @param {DataList} source - ��������� �� ������ ������, ������ ����� ����������� ������
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
* ������� ��������� �� ������� ������ ����������� �������
* @param {DataList**} dest - ��������� �� ������ ������.
* @param {DataList} source - ��������� �� ������ ������
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
* ������� ����������� ������� � ������� ������ (���������� �����������)
* @param {DataList**} dest - ��������� �� ������ ������.
* @param {DataList} source - ��������� �� ������ ������
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