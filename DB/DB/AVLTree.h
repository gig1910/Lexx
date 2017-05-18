#pragma once

#include <stdlib.h>
#include <string.h>

#include "dataList.h"

typedef struct AVLTreeNode {
	int key;
	unsigned char height = 1;
	struct AVLTreeNode *left = NULL;
	struct AVLTreeNode *right = NULL;
	struct DataList *data = NULL;
	struct DataList *dataLast = NULL;
} AVLTreeNode;

/**
 * Создание ноды AVL дерева
 * @param {int} k - Ключ ноды
 * @param {void*} data - Указатель на данные
 */
AVLTreeNode *AVLTree_Create(int k, void *data) {
	AVLTreeNode *p = (AVLTreeNode*)calloc(1, sizeof(AVLTreeNode));
	p->height = 1;
	p->left = NULL;
	p->right = NULL;
	p->key = k;
	p->data = dataList_Create(data);
	p->dataLast = p->data;

	return p;
}

unsigned char AVLTree_Height(AVLTreeNode *p)
{
	return p ? p->height : 0;
}

int AVLTree_bFactor(AVLTreeNode *p)
{
	return AVLTree_Height(p->right) - AVLTree_Height(p->left);
}

void AVLTree_FixHeight(AVLTreeNode *p)
{
	unsigned char hl = AVLTree_Height(p->left);
	unsigned char hr = AVLTree_Height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

AVLTreeNode *AVLTree_RotateRight(AVLTreeNode *p) // правый поворот вокруг p
{
	AVLTreeNode *q = p->left;
	p->left = q->right;
	q->right = p;
	AVLTree_FixHeight(p);
	AVLTree_FixHeight(q);

	return q;
}

AVLTreeNode *AVLTree_RotateLeft(AVLTreeNode *q) // левый поворот вокруг q
{
	AVLTreeNode *p = q->right;
	q->right = p->left;
	p->left = q;
	AVLTree_FixHeight(q);
	AVLTree_FixHeight(p);

	return p;
}

AVLTreeNode *AVLTree_Balance(AVLTreeNode *p) // балансировка узла p
{
	AVLTree_FixHeight(p);

	if (AVLTree_bFactor(p) == 2)
	{
		if (AVLTree_bFactor(p->right) < 0)
			p->right = AVLTree_RotateRight(p->right);
		return AVLTree_RotateLeft(p);
	}
	if (AVLTree_bFactor(p) == -2)
	{
		if (AVLTree_bFactor(p->left) > 0)
			p->left = AVLTree_RotateRight(p->left);
		return AVLTree_RotateLeft(p);
	}
	return p; // балансировка не нужна
}

AVLTreeNode *AVLTree_Put(AVLTreeNode *p, int k, void *data) // вставка ключа k в дерево с корнем p
{
	if (p) {
		if (k < p->key)
			p->left = AVLTree_Put(p->left, k, data);
		else
			p->right = AVLTree_Put(p->right, k, data);

		return AVLTree_Balance(p);
	}
	else {	//Ноды ещё нет. Создаём первую и возврашаем её
		return AVLTree_Create(k, data);;
	}
}

AVLTreeNode *AVLTree_Find(AVLTreeNode *p, int k) {
	AVLTreeNode *node = p;
	while (p != NULL && p->key != k) {
		p = p->key < k ? p->left : p->right;
	}

	return p;
}

DataList *AVLTree_FindData(AVLTreeNode *p, int k) {
	AVLTreeNode *node = p;
	while (p != NULL && p->key != k) {
		p = p->key < k ? p->left : p->right;
	}

	return p ? dataList_Copy(p->data) : NULL;
}

AVLTreeNode *AVLTree_findMin(AVLTreeNode *p) // поиск узла с минимальным ключом в дереве p 
{
	return p->left ? AVLTree_findMin(p->left) : p;
}

AVLTreeNode *AVLTree_removeMin(AVLTreeNode *p) // удаление узла с минимальным ключом из дерева p
{
	if (p->left == 0)
		return p->right;
	p->left = AVLTree_removeMin(p->left);

	return AVLTree_Balance(p);
}

AVLTreeNode *AVLTree_Remove(AVLTreeNode *p, int k, void *data) // удаление ключа k из дерева p
{
	if (!p) return 0;
	if (k < p->key)
		p->left = AVLTree_Remove(p->left, k, data);
	else if (k > p->key)
		p->right = AVLTree_Remove(p->right, k, data);
	else //  k == p->key 
	{
		AVLTreeNode *q = p->left;
		AVLTreeNode *r = p->right;
		delete p;
		if (!r) return q;
		AVLTreeNode *min = AVLTree_findMin(r);
		min->right = AVLTree_removeMin(r);
		min->left = q;
		return AVLTree_Balance(min);
	}

	return AVLTree_Balance(p);
}

void AVLTree_Free(AVLTreeNode **root) {
	if (root && *root) {
		AVLTree_Free(&(*root)->left);
		(*root)->left = NULL;
		AVLTree_Free(&(*root)->right);
		(*root)->right = NULL;

		dataList_ListFree(&(*root)->data, NULL);
		(*root)->data = NULL;
		(*root)->dataLast = NULL;

		free(*root);
		*root = NULL;
	}
}