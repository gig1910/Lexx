#pragma once

typedef struct DataList {
	void *data;
	struct DataList *next;
} DataList;

typedef void(*DataFreeFunc)(void *data);

DataList *createDataList(void *data) {
	DataList *d = (DataList*)calloc(1, sizeof(DataList));
	d->next = NULL;
	d->data = data;
	return d;
}

DataList *putInDataList(DataList *node, void *data) {
	if (node != NULL) {
		node->next = createDataList(data);
		node = node->next;
	}
	return node;
}

DataList *removeDataFromDataList(DataList *root, void *data) {
	if (root != NULL) {
		DataList *node = root;
		DataList *prev = NULL;
		while (node != NULL) {
			if (node->data == data) {
				if (prev != NULL) {
					prev->next = node->next;

					node->next = NULL;
					node->data = NULL;
					free(node);
					node = NULL;

					node = prev->next;
				}
				else if (node->next != NULL) {
					root = node->next;

					node->next = NULL;
					node->data = NULL;
					free(node);
					node = NULL;

					node = root;
				}
			}
			else {
				prev = node;
				node = node->next;
			}
		}
	}

	return root;
}

DataList *findDataInDataList(DataList *root, void *data) {
	DataList *node = NULL;
	if (root != NULL) {
		node = root;
		while (node != NULL && node->data != data) {
			node = node->next;
		}
	}
	return node;
}

void clearDataList(DataList *dataList, DataFreeFunc func) {
	DataList *tmp = dataList;
	while (tmp != NULL) {
		if (func != NULL) {
			func(tmp->data);
		}
		tmp->data = NULL;

		DataList *t = tmp;
		tmp = tmp->next;

		t->next = NULL;
		try { free(t); }
		catch (_exception e) {}
		t = NULL;
	}
}

DataList *unionDataList(DataList *dest, DataList *source) {
	if (dest == NULL) {
		dest = source;
	}
	else if (source != NULL) {
		DataList *node = source;
		while (node != NULL) {
			if (findDataInDataList(dest, node->data) == NULL) {
				putInDataList(dest, node->data);
				node = node->next;
			}
		}
	}

	return dest;
}

DataList *removeDataListFromDataList(DataList *dest, DataList *source) {
	if (dest != NULL && source != NULL) {
		DataList *node = source;
		while (node != NULL) {
			if (findDataInDataList(dest, node->data) != NULL) {
				removeDataFromDataList(dest, node->data);
				node = node->next;
			}
		}
	}

	return dest;
}

DataList *mergeDataList(DataList *dest, DataList *source) {
	if (dest != NULL && source != NULL) {
		DataList *node = source;
		while (node != NULL) {
			if (findDataInDataList(dest, node->data) == NULL) {
				removeDataFromDataList(dest, node->data);
				node = node->next;
			}
		}
	}

	return dest;
}