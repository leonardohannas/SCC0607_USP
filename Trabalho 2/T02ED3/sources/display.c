/**
 * Trabalho 02 - Estrutura de Dados III
 *
 * Integrantes do grupo:
 *
 * Leonardo Hannas de Carvalho Santos  NUSP: 11800480
 * Lucas Carvalho Freiberger Stapf     NUSP: 11800559
 */

#include <stdio.h>
#include "../headers/display.h"
#include "../headers/constants.h"


/**
 * @brief Esta funcao printa na tela uma mensagem de acordo com o parametro passado.
 * 
 * @param cod Codigo da mensagem.
 * @return Retorna o proprio codigo passado.
 * 
 * @author Leonardo Hannas de Carvalho Santos
 * @author Lucas Carvalho Freiberger Stapf
 */
int showMessage(int cod) {

    switch (cod) {

        case FILE_ERROR:
            printf("Falha na execução da funcionalidade.");
            break;

        case PATH_NOT_FOUND:
            printf("Não existe caminho entre as estações solicitadas.");
            break;

        case CYCLE_NOT_FOUND:
            printf("Nao existe um ciclo a partir da estacao de origem.");
            break;

        default:
            printf("Falha na execucao da funcionalidade.");
            break;
    }

    return cod;
}