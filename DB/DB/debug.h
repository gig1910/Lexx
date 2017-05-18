#pragma once

#include "stdio.h"
#include <stdarg.h>
#include <ctime>

#include "dataList.h"
#include "strTree.h"
#include "AVLTree.h"
#include "types.h"

bool _debug = false;

void debug_ON() {
	_debug = true;
}

void debug_OFF() {
	_debug = false;
}

void debug_Log(char *str, ...) {
	if (!_debug) return;

	FILE *f = fopen("degug.log", "a");
	if (f) {
		va_list args;
		va_start(args, str);
		float diff = clock();
		fprintf(f, "%8.4f ", diff / 1000);
		vfprintf(f, str, args);
		fprintf(f, "\n");
		fclose(f);
	}
}

void debug_Print(char *str, ...) {
	if (!_debug) return;

	FILE *f = fopen("degug.log", "a");
	if (f) {
		va_list args;
		va_start(args, str);
		vfprintf(f, str, args);
		fclose(f);
	}
}

void debug_PrintData(void *data) {
	if (!_debug) return;

	if (data != NULL) {
		Data *d = (Data*)data;
		debug_Print(
			"{%20s\t%20s\t%30s\t%1d\t%3d\t%3d\t%2d}\n",
			d->firstName, d->middleName, d->lastName, d->sex, d->age, d->weight, d->group
		);
	}
}

void debug_dataListPrint(DataList *root, DataPrintFunc func) {
	if (!_debug) return;

	debug_Print("printList\n");
	DataList *node = root;
	int i = 0;
	while (node) {
		debug_Print("\t%d: ", i++);
		if (func) {
			func(node->data);
		}
		else {
			debug_Print("%d\n", (int)(node->data));
		}
		node = node->next;
	}
}

void debug_StrTreePrint(StrTreeNode *root, int lev) {
	if (!_debug) return;

	if (root) {
		for (int i = 0; i < lev; i++) {
			debug_Print("\t");
		}
		debug_Print("\"%c\", dataCount: %d\n", (char)root->letter, dataList_Length(root->data));
		DataList *child = root->childs;
		while (child) {
			debug_StrTreePrint((StrTreeNode*)child->data, lev + 1);
			child = child->next;
		}
	}
}

void debug_AVLTreePrint(AVLTreeNode *root, int lev) {
	if (!_debug) return;

	if (root) {
		for (int i = 0; i < lev; i++) {
			debug_Print("\t");
		}
		debug_Print("key: %d, dataCount: %d\n", root->key, dataList_Length(root->data));

		if (root->left) {
			for (int i = 0; i < lev; i++) {
				debug_Print("\t");
			}
			debug_Print("<");
			debug_AVLTreePrint(root->left, lev + 1);
		}

		if (root->right) {
			for (int i = 0; i < lev; i++) {
				debug_Print("\t");
			}

			debug_Print(">");
			debug_AVLTreePrint(root->right, lev + 1);
		}
	}
}