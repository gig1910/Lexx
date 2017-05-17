#pragma once

#include <stdlib.h>
#include <string.h>

#include "dataList.h"
#include "ctype.h"

/**
* ��������� ���� ���������� ������ ������
*/
typedef struct StrTreeNode {
	int letter;								//�����
	struct StrTreeNode *parent = NULL;		//��������� �� ������ (��� ���������� �� �������� ��� �������� ��������)
	struct DataList *childs = NULL;			//������ ��������
	struct DataList *data = NULL;			//��������� �� ��������� ������ ������
	struct DataList *last = NULL;			//��������� �� ��������� ������� ������ (��� ��������� �������)
} StrTreeNode;


StrTreeNode *strTree_Create(int letter, void *data, StrTreeNode *parent) {
	StrTreeNode *node = (StrTreeNode*)calloc(1, sizeof(StrTreeNode));
	node->letter = letter;
	node->parent = parent;
	node->childs = NULL;
	node->data = data ? dataList_Create(data) : NULL;
	node->last = node->data;

	return node;
}

void strTree_Free(StrTreeNode **node) {
	if (node && *node) {

		//���������� �� ���� ������ � ������� ��
		while ((*node)->childs) {
			DataList *child = (*node)->childs;
			StrTreeNode *_node = (StrTreeNode*)(child->data);
			strTree_Free(&_node);
			(*node)->childs = dataList_Free(&child, NULL);
		}
		(*node)->letter = 0;
		(*node)->data = NULL;
		(*node)->last = NULL;
		(*node)->parent = NULL;
		free(*node);
		*node = NULL;
	}
}

/**
 * ������� ������
 */
bool findLetterInStrTreeNode(void *data, void *findData) {
	return data && ((StrTreeNode*)data)->letter == *(int*)findData;
};

/**
* ������� ���������� ������ � ������
* @param {*StrTreeNode} root	- ��������� �� ������� ������ ���������� ������
* @param {*char}	     word	- �����, ������� ���� ���������
* @param {*void}		 data	- ��������� �� ����������� ��������� (������������, �.�. ������ ��� ������� ��� ������. ��� ��������� ����������� ����������)
*/
void strTree_Put(StrTreeNode **root, char *word, void *data) {
	if (root) {
		if (!*root) {
			*root = strTree_Create('\0', NULL, NULL);
		}

		StrTreeNode *node = *root;
		for (size_t i = 0; i < strlen(word); i++) {
			int letter = toupper(word[i]);
			DataList *nextNode = dataList_Find(node->childs, findLetterInStrTreeNode, &letter);

			if (!nextNode) {
				node = (StrTreeNode*)(dataList_Put(&node->childs, strTree_Create(letter, NULL, node))->data);
			}
			else {
				node = (StrTreeNode*)nextNode->data;
			}
		}
		node->last = dataList_Put(&node->data, data, node->last);
	}
}

/**
* ������� ����� � ������
* @param {*StrTreeNode} root	- ��������� �� ������� ������ ���������� ������
* @param {*char}	     word	- �����, ������� ���� �����
*
* @returns {*DataList}- ��������� ������ � ���� ����������������� ������
*/
StrTreeNode *strTree_Find(StrTreeNode *root, char *word) {
	if (root && word && strlen(word) > 0) {
		StrTreeNode *node = root;
		size_t i = 0;
		while (node && i < strlen(word)) {
			int letter = toupper(word[i]);
			DataList *dl = dataList_Find(node->childs, findLetterInStrTreeNode, &letter);
			node = dl ? (StrTreeNode*)dl->data : NULL;
			i++;
		}

		return node;
	}
	else {
		return NULL;
	}
}

/**
* ������� ������ ������ � ������
* @param {*StrTreeNode} root	- ��������� �� ������� ������ ���������� ������
* @param {*char}	     word	- �����, ������� ���� �����
*
* @returns {*DataList}- ��������� ������ � ���� ����������������� ������
*/
DataList *strTree_DataFind(StrTreeNode *root, char *word) {
	StrTreeNode *node = strTree_Find(root, word);
	return node ? dataList_Copy(node->data) : NULL;
}

/**
 * �������� ������ � ������
 * @param {}
*/
void strTree_Remove(StrTreeNode **root, char *word, void *data) {
	if (root && *root) {
		StrTreeNode *node = strTree_Find(*root, word);
		if (node) {
			dataList_Remove(&node->data, data);

			while (node && !node->data && !node->childs) {
				if (node->parent) {
					dataList_Remove(&node->parent->childs, node);
				}

				StrTreeNode *tmp = node->parent;
				strTree_Free(&node);
				node = tmp->parent;
			}
		}
	}
}