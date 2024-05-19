/**
 * Trabalho 02 - Estrutura de Dados III
 *
 * Integrantes do grupo:
 *
 * Leonardo Hannas de Carvalho Santos  NUSP: 11800480
 * Lucas Carvalho Freiberger Stapf     NUSP: 11800559
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/linkedlist.h"
#include "../headers/constants.h"

/**
 * @brief Inicializa com os valores padrao uma lista encadeada
 *
 * @param l Lista encadeada a ser inicializada.
 * 
 * @author Leonardo Hannas de Carvalho Santos
 * @author Lucas Carvalho Freiberger Stapf
 */
void createLinkedList(linkedlist *l) {
  l->size = 0;
  l->head = NULL;
}

/**
 * @brief Libera todos os nodos alocados para a lista encadeada.
 * 
 * @param l Lista encadeada a ser apagada.
 * 
 * @author Leonardo Hannas de Carvalho Santos
 * @author Lucas Carvalho Freiberger Stapf
 */
void deleteLinkedList(linkedlist *l) {

  node *n = l->head;

  while (n != NULL) {
    l->head = l->head->next;
    free(n->data);
    free(n);
    n = l->head;
  }
}

/**
 * @brief Adiciona um elemento generico na lista encadeada.
 * 
 * @param l Lista onde o elemento sera adicionado.
 * @param data Elemento generico a ser adicionado.
 * @return TRUE caso o elemento tenha sido adicionado com sucesso e FALSE caso contrario. 
 * 
 * @author Leonardo Hannas de Carvalho Santos
 * @author Lucas Carvalho Freiberger Stapf
 */
node* addElementLinkedList(linkedlist *l, void *data) {

  if(l == NULL || data == NULL) return NULL;

  node *n = malloc(sizeof(node));
  if(n == NULL) return NULL;

  n->data = data;
  n->next = NULL;

  if(l->head == NULL) { // Lista vazia

    l->head = n;
    l->size++;
    return n;
  
  } else {

    node *aux = l->head;
    while (aux->next != NULL) aux = aux->next;
    aux->next = n;
    l->size++;
    return n;
  }
}

/**
 * Retorna o ponteiro para o nodo cujo indice na lista encadeada é o passado como argumento na função.
 * @param l lista encadeada.
 * @param index indice do nodo desejado.
 * @return Retorna o ponteiro para o nodo do indíce desejado. Retorna NULL caso o indíce for inválido.
 */
node* getNodeLinkedList(linkedlist l, int index) {
    if (index < 0 || index >= l.size) return NULL;
    node *aux = l.head;
    for(int i = 0; i < index; i++) aux = aux->next;
    return aux;
}

/**
 * Verifica se a lista encadeada possui um determinado dado armazenado.
 * @param l lista encadeada.
 * @param data dado usado na verificação.
 * @param comparator função utilizada na comparação dos dados armazenados.
 * @return Retorna o indíce do primeiro nodo que armazena o dado passado da lista. Retorna -1 caso o dado não
 * esteja presente na lista.
 */
int hasDataLinkedList(linkedlist l, void *data, int (*comparator)()) {
    node *aux = l.head;
    for (int i = 0; i < l.size; i++) {
        if (comparator(aux->data, data) == 0) return i;
        else aux = aux->next;
    }
    return -1; // Elemento não encontrado.
}

/**
 * Verifica se um determinado nodo está presente na lista encadeada.
 * @param l lista encadeada.
 * @param n nodo a ser verificado.
 * @param comparator função comparadora.
 * @return Retorna o indíce do nodo na lista caso seja encontrado, retorna -1 caso o 
 * nodo não esteja presente na lista.
 */
int hasNodeLinkedList(linkedlist l, node n, int (*comparator)()) {
    node *aux = l.head;
    for (int i = 0; i < l.size; i++) {
        if (comparator(*aux, n) == 0) return i;
        else aux = aux->next;
    }
    return -1;
}


/**
 * @brief Funcao que adiciona apenas strings em uma lista encadeada. Strings iguais nao serao
 * adicionadas.
 *
 * @param l Lista onde a string sera adicionada.
 * @param str String a ser adicionada.
 *
 * @author Leonardo Hannas de Carvalho Santos
 * @author Lucas Carvalho Freiberger Stapf
 */
void addStringLinkedList(linkedlist *l, char *str) {

  if(l == NULL || str == NULL) return;

  node *n = (node*) malloc(sizeof(node));
  if(n == NULL) return;

  n->data = malloc(sizeof(char) * MAX_SIZE_STR);

  if (n->data == NULL) {
    free(n);
    return;
  }

  n->next = NULL;
  strcpy(n->data, str);

  if(l->head == NULL) { // Lista vazia

    l->head = n;
    l->size++;
    return;

  } else {

    node *aux = l->head;

    while(aux != NULL) {

      if(strcmp(aux->data, str) == 0) {
        free(n->data);
        free(n);
        return;

      } else if(aux->next == NULL) {
        aux->next = n;
        l->size++;
        return;

      } else aux = aux->next;
    }
  }
}

/** 
 * @brief Verifica se uma determina string está presente na lista encadeada. 
 * 
 * @param l lista encadeada.
 * @param str string analisada.
 * @return Retorna o indíce do nodo onde está presente a string ou então retorna -1 caso não seja encontrada
 * a string passada.
 */
int hasStringElementLinkedList(linkedlist *l, char *str) {

    int i = 0; // Elemento não encontrado.
    node *node = l->head;

    while (node != NULL) {

        char *s = (char*) node->data;
        if (strcmp(s, str) == 0) return i;
        else node = node->next;
        i++;
    }

    return -1; // Elemento não encontrado.
}

/**
 * @brief Imprime na tela uma lista encadeada de strings.
 * 
 * @param l lista encadeada.
 */
void printStringLinkedList(linkedlist l) {

    node *aux = l.head;
    while (aux != NULL) {
        if (aux->next != NULL) printf("%s ", (char *)aux->data);
        else printf("%s", (char *)aux->data);
        aux = aux->next;
    }
}

/**
 * @brief Inicializa um iterador para a lista encadeada.
 * 
 * @param l lista encadeada.
 * @param i iterador.
 */
void createIterator(linkedlist l, iterator *i) {
    i->list = l;
    i->current = l.head;
}

/**
 * @brief Retorna o próximo elemento apontado pelo iterador.
 * 
 * @param i iterador.
 * @return Retorna o próximo elemento apontado pelo iterador.
 */
node* getNextNode(iterator *i) {
    node *n = i->current;
    if (i->current != NULL) i->current = i->current->next;
    return n;
}

/**
 * @brief Verifica se há um próximo nodo não nula apontado pelo iterador.
 * 
 * @param i iterador.
 * @return Retorna TRUE caso possua um próximo nodo válido, FALSE caso contrário.
 */
int hasNextNode(iterator *i) {
    if (i->current != NULL) return TRUE;
    else return FALSE;
}

/**
 * @brief Função que adiciona um elemento na primeira posição da lista encadeada.
 * 
 * @param l lista encadeada.
 * @param data dado a ser armazenado.
 * @return Retorna SUCCESS caso a adição do dado ocorra com sucesso, ERROR caso contrário.
 */
int addElementFirstLinkedList(linkedlist *l, void *data) {

    if (l == NULL) return ERROR;

    if (l->size == 0) addElementLinkedList(l, data);
    else {
        node *n = malloc(sizeof(node));
        n->data = data;
        n->next = l->head;
        l->head = n;
        l->size++;
    }

    return SUCCESS;
}

/**
 * @brief Função que remove um determinado nodo da lista encadeada.
 * 
 * @param l lista encadeada.
 * @param n nodo a ser removido.
 */
void removeNodeLinkedList(linkedlist *l, node *n) {

    if (l == NULL || n == NULL) return;

    if (l->head == n) { // É o primeiro elemento
        l->head = n->next;
        l->size--;
    }

    node *aux = l->head;

    while (aux != NULL) {
        if (aux->next == n) {
            aux->next = n->next;
            l->size--;
            return;
        } else aux = aux->next;
    }
}

/**
 * @brief Função que imprime a lista encadeada na tela com a formatação desejada.
 * 
 * @param l lista encadeada
 * @param print função com a formatação da impressão do dado.
 */
void printLinkedList(linkedlist l, void (*print)()) {
    node *aux = l.head;
    while (aux != NULL) {
        print(aux->data);
        aux = aux->next;
    }
}

/**
 * @brief Função que copia uma lista encadeada em outra. Os dados passam a ser compartilhados pelas duas listas.
 * 
 * @param l_destination lista de destino.
 * @param l_origin lista de origem.
 */
void copyLinkedList(linkedlist *l_destination, linkedlist *l_origin) {

    iterator iter;
    createIterator(*l_origin, &iter);

    createLinkedList(l_destination);

    while (hasNextNode(&iter) == TRUE) {
        node *n = getNextNode(&iter);
        addElementLinkedList(l_destination, n->data);
    }
}
