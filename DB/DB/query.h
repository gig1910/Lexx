#pragma once

#include <stdlib.h>
#include <string.h>

#include "dataList.h"
#include "stack.h"
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
			case 0:		//И
				dataList_Merge(&res, _dl);
				break;

			case 1:		//ИЛИ
				dataList_Union(&res, _dl);
				break;

			case 2:	//НЕ
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
	if (query) {
		DataList *literals = NULL;

		//Первоначально проходимся по строке и делим её по разделителям на литералы (разделитель, это пробел, перенос строки и табиляция).
		//Обрабатываем с учетом кавычек (внутри кавычек пробел сохранятся)
		//Отдельно выделяются скобки
		size_t _len = strlen(query);
		for (size_t i = 0; i < _len; i++) {
			bool bSpace = false;	//Флаг пробела
			bool bQuotes = false;	//Флаг кавычки
			bool bEsc = false;		//Флаг символа escape-последовательности

			size_t _stInd = 0;

			switch (query[i]) {
			case 8: case 10: case 13: case 32:	//Табуляция, возврат картеки, перевод строки, пробел
				if (bQuotes) {	//Если была кавычка, то заносим в значение все пробелы как есть.

				}
				else {
					bSpace = true;
					_stInd = i;
				}
				break;

			case 34:			//Кавычка
				if (bQuotes && !bEsc) {	//Если закрывающая кавычка, то копируем строку без кавычек
					char *str = (char*)calloc(i - _stInd + 1, sizeof(char));
					strncpy(str, &query[_stInd], i - _stInd);
					dataList_Put(&literals, str);
				}
				else {
					bEsc = false;
					_stInd = i;
				}
				else {
					bQuotes = true;
				}
				break;
			case 40: case 41:
				dataList_Put()
					break;
			}
		}
	}
	return NULL;
}