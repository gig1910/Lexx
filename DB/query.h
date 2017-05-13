#pragma once

#include "dataList.h"
#include "strTree.h"
#include "AVLTree.h"
#include "db.h"

typedef struct QueryList {
	Data *filter = NULL;
	byte condition = 0;
	QueryList *next;
} QueryList;

DataList *query(QueryList *query) {
	return NULL;
}