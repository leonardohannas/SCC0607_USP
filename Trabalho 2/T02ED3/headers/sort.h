/**
* Trabalho 02 - Estrutura de Dados III
*
* Integrantes do grupo:
*
* Leonardo Hannas de Carvalho Santos  NUSP: 11800480
* Lucas Carvalho Freiberger Stapf     NUSP: 11800559
*/

#ifndef SORT_H
#define SORT_H

#include "linkedlist.h"

void sortLinkedList(linkedlist *l, int(*comparator)());
void sortArray(void **array, unsigned int size, int(*comparator)());

#endif
