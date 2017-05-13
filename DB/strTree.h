#pragma once

#include "dataList.h"
#include "ctype.h"

/**
* Структура ноду строкового дерева поиска
*/
typedef struct StrTreeNode {
	struct LetterList *letters = NULL;		//Список довтупный букв для перехода
	struct DataList *data = NULL;			//Указатель на связанный список данных
	struct DataList *lastData = NULL;		//Указатель на последнюю запись списка данных (для ускорения вставки)
} StrTreeNode;

/**
 * Структура хранения буквы для строкового дерева поиска
 * @struct
 */
typedef struct LetterList {
	char letter = NULL;				//Буква
	struct StrTreeNode *node = NULL;		//Указатель на ноду дерева (вниз)
	struct LetterList *next = NULL;		//Переход к следующей букве (вправо)
} LetterList;

/**
 * Функция добавления данных к дереву
 * @param {*StrTreeNode} root	- указатель на вершину дерева строкового поиска
 * @param {*char}	     word	- Слово, которое надо сохранить
 * @param {*void}		 data	- Указатель на сохраняемую структура (обезличенный, т.к. дереву без разницы что ханить. Тип определяе потребитель информации)
 */
void putStrDataInTree(StrTreeNode *root, char *word, void *data) {
	StrTreeNode *node = root;
	if (node != NULL) {
		for (int i = 0; i < strlen(word); i++) {
			LetterList *l = node->letters;
			LetterList *lRoot = node->letters;
			LetterList *lLast = NULL;
			char letter = toupper(word[i]);
			while (l != NULL && l->letter != letter) {
				lLast = l;
				l = l->next;
			}

			if (l != NULL) {
				node = l->node;
			}
			else {
				LetterList *newL = (LetterList*)calloc(1, sizeof(LetterList));

				if (lLast == NULL) {
					node->letters = newL;
				}
				else {
					lLast->next = newL;
				}

				newL->letter = letter;
				newL->node = (StrTreeNode*)calloc(1, sizeof(StrTreeNode));
				node = newL->node;
			}
		}

		if (node->lastData == NULL) {
			node->lastData = createDataList(data);
			node->data = node->lastData;
		}
		else {
			node->lastData->next = createDataList(data);
			node->lastData = node->lastData->next;
		}
	}
}

/**
 * Функция поиска данных в дереве
 * @param {*StrTreeNode} root	- указатель на вершину дерева строкового поиска
 * @param {*char}	     word	- Слово, которое надо найти
 *
 * @returns {*DataList}- Найденные данные в виде однонаправленного списка
 */
DataList *findInStrTree(StrTreeNode *root, char *word) {
	if (root != NULL && word != NULL && strlen(word) > 0) {
		StrTreeNode *node = root;
		int i = 0;
		while (node != NULL && i < strlen(word)) {
			LetterList *l = node->letters;
			char letter = toupper(word[i]);
			while (l != NULL && l->letter != letter) {
				l = l->next;
			}
			if (l != NULL) {
				node = l->node;
				i++;
			}
			else {
				return NULL;
			}
		}

		return node->data;
	}
	else {
		return NULL;
	}
}

/**
 * Функция удаления ветки дерева (без очистки данный по указателям в списке данных)
 * @param {*StrTreeNode} root	- указатель на вершину дерева строкового поиска
 */
void freeStrTree(StrTreeNode * root) {
	if (root != NULL) {
		clearDataList(root->data, NULL);
		root->data = NULL;
		root->lastData = NULL;

		while (root->letters != NULL) {
			freeStrTree(root->letters->node);

			LetterList *tmpL = root->letters;
			root->letters = root->letters->next;

			tmpL->next = NULL;
			tmpL->node = NULL;
			free(tmpL);
		}

		free(root);
	}
}

/**
 * Функция рекурсивного поиска и удаления данных в дереве
 * @param {*StrTreeNode} root	- указатель на вершину дерева строкового поиска
 * @param {*char}	     word	- Слово, которое надо найти
 *
 * @returns {bool}- Найденные данные в виде однонаправленного списка
 * @private
 */
bool _recurFindAndDel(StrTreeNode *root, char *word, int i, void *data) {
	if (root != NULL) {
		if (i == strlen(word)) {
			DataList *d = removeDataFromDataList(root->data, data);
			if (d == NULL) {
				return false;
			}
		}
		else {
			LetterList *l = root->letters;
			char letter = toupper(word[i]);

			LetterList *prev = NULL;
			while (l != NULL && l->letter != letter) {
				prev = l;
				l = l->next;
			}

			if (l != NULL) {
				if (!_recurFindAndDel(l->node, word, ++i, data)) {
					if (prev != NULL) {
						prev->next = l->next;
					}
					else {
						root->letters = l->next;
					}
					l->next = NULL;
					l->node = NULL;
					free(l);

					if (root->letters == NULL) {
						freeStrTree(root);
						return false;
					}
				};
			}
		}
	}

	return true;
}

/**
 * Удаление данных в дереве
 */
void removeDataInStrTree(StrTreeNode *root, char *word, void *data) {
	_recurFindAndDel(root, word, 0, data);
}