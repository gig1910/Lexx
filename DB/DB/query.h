#pragma once

#include <stdlib.h>
#include <string.h>

#include "dataList.h"
#include "strTree.h"
#include "AVLTree.h"
#include "db.h"

typedef struct Query {
	char *field = NULL;
	void *query = NULL;
	unsigned char condition = 0;
} Query;

typedef struct QueryList {
	Query *query;
	byte condition = 0;
	QueryList *next;
} QueryList;

DataList *_query(Query *query, DataList *res) {
	if (query != NULL) {
		if (strcmp(query->field, "firstName") == 0) {
			return strTree_DataFind(firstNameRoot, (char*)query->query);
		}
		else if (strcmp(query->field, "middleName") == 0) {
			return strTree_DataFind(middleNameRoot, (char*)query->query);
		}
		else if (strcmp(query->field, "lastName") == 0) {
			return strTree_DataFind(lastNameRoot, (char*)query->query);
		}
		else if (strcmp(query->field, "sex") == 0) {
			DataList *_node = res;
			while (_node != NULL) {
				if (_node->data == NULL || ((Data*)_node->data)->sex != *((bool*)query->query)) {
					dataList_Remove(&res, _node->data);
				}
				_node = _node->next;
			}
			return res;
		}
		else if (strcmp(query->field, "age") == 0) {
			return findDataList(ageRoot, *(int*)query->query);
		}
		else if (strcmp(query->field, "weight") == 0) {
			return findDataList(weightRoot, *(int*)query->query);
		}
		else if (strcmp(query->field, "group") == 0) {
			DataList *_node = res;
			while (_node != NULL) {
				if (_node->data == NULL || ((Data*)_node->data)->group != *((int*)query->query)) {
					dataList_Remove(&res, _node->data);
				}
				_node = _node->next;
			}
			return res;
		}
	}

	return NULL;
}

DataList *query(QueryList *query) {
	DataList *res = NULL;
	QueryList *q = query;
	while (q != NULL) {
		if (res == NULL) {
			res = _query(q->query, NULL);
		}
		else {
			switch (q->condition) {
			case 0:		//È
				dataList_Merge(&res, _query(q->query, res));
				break;

			case 1:		//ÈËÈ
				dataList_Union(&res, _query(q->query, res));
				break;

			case 2:	//ÍÅ
				dataList_Subtraction(&res, _query(q->query, res));
				break;
			}
		}

		q = q->next;
	}

	return res;
}

void clearQueryList(QueryList *queryList) {
	while (queryList != NULL) {
		QueryList *q = queryList;
		queryList = queryList->next;

		free(q->query);
		free(q);
	}
}

DataList *strQuery(char *query) {
	return NULL;
}