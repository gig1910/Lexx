#pragma once

#include <stdlib.h>
#include <string.h>

#include "dataList.h"
#include "strTree.h"
#include "AVLTree.h"
#include "db.h"

typedef struct Query {
	char *field = NULL;
	char *query = NULL;
	unsigned char condition = 0;
	unsigned char conditionQuery = 0;
} Query;

Query *query_Create(char *field, char *query, unsigned char condition, unsigned char conditionQuery) {
	Query *q = (Query*)calloc(1, sizeof(Query));

	q->field = (char*)calloc(strlen(field) + 1, sizeof(char));
	strcpy(q->field, field);

	q->query = (char*)calloc(strlen(query) + 1, sizeof(char));
	strcpy(q->query, query);

	q->condition = condition;
	q->condition = conditionQuery;

	return q;
}

DataList *query_Put(DataList **queryList, Query *query) {
	return dataList_Put(queryList, query);
}

DataList *query_query(Query *query, DataList *res) {
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
			return AVLTree_FindData(ageRoot, *(int*)query->query);
		}
		else if (strcmp(query->field, "weight") == 0) {
			return AVLTree_FindData(weightRoot, *(int*)query->query);
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

DataList *query_Query(DataList *queryList) {
	DataList *res = NULL;
	DataList *ql = queryList;
	while (ql) {
		if (!res) {
			res = query_query((Query*)ql->data, NULL);
		}
		else {
			DataList *_dl = query_query((Query*)ql->data, res);

			switch (((Query*)ql->data)->condition) {
			case 0:		//È
				dataList_Merge(&res, _dl);
				break;

			case 1:		//ÈËÈ
				dataList_Union(&res, _dl);
				break;

			case 2:	//ÍÅ
				dataList_Subtraction(&res, _dl);
				break;
			}

			dataList_ListFree(&_dl);
			_dl = NULL;
		}

		ql = ql->next;
	}

	return res;
}

void query_Free(void **query) {
	if (query && *query) {
		free(((Query*)*query)->field);
		((Query*)*query)->field = NULL;
		free(((Query*)*query)->query);
		((Query*)*query)->query = NULL;

		free(*query);
		*query = NULL;
	}
}

void query_ListFree(DataList **queryList) {
	if (queryList) {
		dataList_ListFree(queryList, query_Free);
	}
}

DataList *strQuery(char *query) {
	return NULL;
}