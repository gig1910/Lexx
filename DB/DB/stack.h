#pragma once

#include <stdlib.h>
#include "dataList.h"

/**
 * Структура СТЕК
 */
typedef struct Stack {
	void *data = NULL;	// Данные в стеке
	Stack *prev = NULL;	//Указатель на предыдущий элемент стека
} Stack;


/**
 * Функция помещения данных в вершину стека ()
 */
void stack_Push(Stack **stack, void *data) {
	if (stack) {
		Stack *el = (Stack*)calloc(1, sizeof(Stack));
		el->prev = *stack;
		el->data = data;
		*stack = el;
	}
}

void *stack_Pop(Stack **stack) {
	if (stack) {
		if (*stack) {
			Stack *tmp = (*stack);
			(*stack) = (*stack)->prev;
			void *data = tmp->data;
			tmp->data = NULL;
			tmp->prev = NULL;
			free(tmp);
			return data;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}

void *shift(Stack **stack) {
	if (stack) {
		if (*stack) {
			Stack *node = *stack;
			Stack *prev = NULL;
			while (node->prev) {
				if (node->prev->prev) {
					prev = node;
				}
				node = node->prev;
			}
			void *data = node->data;
			if (prev) {
				prev->prev = NULL;
			}
			else {
				(*stack)->prev - NULL;
			}
			free(node);

			return data;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}