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

DataList *copyDataList(DataList *dataList) {
	DataList *copy = NULL;
	DataList *root = NULL;
	DataList *node = dataList;
	while (node != NULL) {
		if (copy == NULL) {
			copy = (DataList*)calloc(1, sizeof(DataList));
			root = copy;
		}
		else {
			copy->next = (DataList*)calloc(1, sizeof(DataList));
			copy = copy->next;
		}
		copy->data = node->data;

		node = node->next;
	}

	return root;
}

DataList *putInDataList(DataList *root, void *data) {
	if (root != NULL) {
		DataList *last = root;
		DataList *node = root;
		while (node != NULL) {
			last = node;
			node = node->next;
		}
		last->next = createDataList(data);

		return root;
	}
	else {
		root = createDataList(data);
	}

	return root;
}

DataList *removeDataFromDataList(DataList *root, void *data) {
	if (root != NULL) {
		DataList *node = root;
		DataList *prev = NULL;
		DataList *tmp;
		while (node != NULL) {
			if (node->data == data) {
				if (node == root) {
					tmp = root;
					root = node->next;
					node = root;
					prev = NULL;
					
					tmp->data = NULL;
					free(tmp);
				}
				else {
					prev->next = node->next;

				}
			}

			if (node->data == data) {
				if (prev != NULL) {
					prev->next = node->next;

					node->next = NULL;
					node->data = NULL;
					free(node);

					node = prev->next;
				}
				else if (node->next != NULL) {
					root = node->next;

					node->next = NULL;
					node->data = NULL;
					free(node);

					node = root;
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

		DataList *t = tmp->next;
		tmp->next = NULL;
		free(tmp);

		tmp = t;
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
				dest = putInDataList(dest, node->data);
			}
			node = node->next;
		}
	}

	return dest;
}

DataList *subtractionDataList(DataList *dest, DataList *source) {
	if (dest != NULL) {
		DataList *node = dest;
		while (node != NULL) {
			void *d = node->data;
			node = node->next;

			if (findDataInDataList(source, d) != NULL) {
				dest = removeDataFromDataList(dest, d);
			}
		}
	}

	return dest;
}

DataList *mergeDataList(DataList *dest, DataList *source) {
	if (dest != NULL) {
		if (source == NULL) {
			return NULL;
		}
		else {
			DataList *node = dest;
			while (node != NULL) {
				void *d = node->data;
				node = node->next;

				if (findDataInDataList(source, d) == NULL) {
					dest = removeDataFromDataList(dest, d);
				}
			}
		}
	}

	return dest;
}