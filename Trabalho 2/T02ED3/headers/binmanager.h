/**
* Trabalho 02 - Estrutura de Dados III
*
* Integrantes do grupo:
* 
* Leonardo Hannas de Carvalho Santos  NUSP: 11800480
* Lucas Carvalho Freiberger Stapf     NUSP: 11800559
*/

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdio.h>
#include "registers.h"

void readHeaderRegisterBIN(FILE *f, HeaderRegister *hr);

int readDataRegisterBIN(FILE *f, DataRegister *dr);
int findDataRegisterBIN(FILE *f, DataRegister *dr_search, DataRegister *dr_return);

int statusFileBIN(HeaderRegister hr);

#endif