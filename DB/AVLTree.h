#pragma once

#include "dataList.h"

typedef struct AVLTreeNode {
	int key;
	unsigned char height = 1;
	AVLTreeNode *left = NULL;
	AVLTreeNode *right = NULL;
	DataList *data;
} AVLTreeNode;

unsigned char height(AVLTreeNode *p)
{
	return p ? p->height : 0;
}

int bfactor(AVLTreeNode *p)
{
	return height(p->right) - height(p->left);
}

void fixheight(AVLTreeNode *p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

AVLTreeNode *rotateRight(AVLTreeNode *p) // правый поворот вокруг p
{
	AVLTreeNode *q = p->left;
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

AVLTreeNode *rotateLeft(AVLTreeNode *q) // левый поворот вокруг q
{
	AVLTreeNode *p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

AVLTreeNode *balance(AVLTreeNode *p) // балансировка узла p
{
	fixheight(p);
	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0)
			p->right = rotateRight(p->right);
		return rotateLeft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotateLeft(p->left);
		return rotateRight(p);
	}
	return p; // балансировка не нужна
}

AVLTreeNode *createAVLTreeNode(int k, void *data) {
	AVLTreeNode *p = (AVLTreeNode*)calloc(1, sizeof(AVLTreeNode));
	p->height = 1;
	p->left = NULL;
	p->right = NULL;
	p->key = k;
	p->data = createDataList(data);
	return p;
}

AVLTreeNode *insert(AVLTreeNode *p, int k, void *data) // вставка ключа k в дерево с корнем p
{
	if (p == NULL) {
		return createAVLTreeNode(k, data);
	}

	if (k < p->key)
		p->left = insert(p->left, k, data);
	else
		p->right = insert(p->right, k, data);

	return balance(p);
}

AVLTreeNode *insertDataToAVLTree(AVLTreeNode *root, int k, void *data) {
	AVLTreeNode *p = find(root, k);
	if (p == NULL) {
		return insert(root, k, data);
	}
	else {
		putInDataList(p->data, data);
		return root;
	}
}

AVLTreeNode *find(AVLTreeNode *p, int k) {
	AVLTreeNode *node = p;
	while (p != NULL && p->key != k) {
		p = p->key < k ? p->left : p->right;
	}
	return p;
}

AVLTreeNode *findMin(AVLTreeNode *p) // поиск узла с минимальным ключом в дереве p 
{
	return p->left ? findMin(p->left) : p;
}

AVLTreeNode *removeMin(AVLTreeNode *p) // удаление узла с минимальным ключом из дерева p
{
	if (p->left == 0)
		return p->right;
	p->left = removeMin(p->left);
	return balance(p);
}

void removeDataFromAVLTree(AVLTreeNode *root, int k, void *data) {
	AVLTreeNode *node = find(root, k);
	if (node != NULL) {
		node->data = removeDataFromDataList(node->data, data);
		if (node->data == NULL) {
			root = remove(root, k);
		}
	}
}

AVLTreeNode *remove(AVLTreeNode *p, int k) // удаление ключа k из дерева p
{
	if (!p) return 0;
	if (k < p->key)
		p->left = remove(p->left, k);
	else if (k > p->key)
		p->right = remove(p->right, k);
	else //  k == p->key 
	{
		AVLTreeNode *q = p->left;
		AVLTreeNode *r = p->right;
		delete p;
		if (!r) return q;
		AVLTreeNode *min = findMin(r);
		min->right = removeMin(r);
		min->left = q;
		return balance(min);
	}

	return balance(p);
}

void freeAVLTreeNode(AVLTreeNode *root) {
	if (root != NULL) {
		freeAVLTreeNode(root->left);
		freeAVLTreeNode(root->right);
		free(root);
	}
}