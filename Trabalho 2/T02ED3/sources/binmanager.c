/**
 * Trabalho 02 - Estrutura de Dados III
 *
 * Integrantes do grupo:
 *
 * Leonardo Hannas de Carvalho Santos  NUSP: 11800480
 * Lucas Carvalho Freiberger Stapf     NUSP: 11800559
 */

#include <stdio.h>
#include "../headers/binmanager.h"

/**
 * @brief Esta funcao le o registro de cabecalho no inicio do arquivo binario e salva
 * os dados em um HeaderRegister.
 * OBS: O arquivo ja deve estar aberto para a leitura no modo binario.
 * 
 * @param f Arquivo onde serao salvos os dados.
 * @param hr Registro que tera os dados lidos do arquivo.
 *
 * @author Leonardo Hannas de Carvalho Santos
 * @author Lucas Carvalho Freiberger Stapf
 */
void readHeaderRegisterBIN(FILE *f, HeaderRegister *hr) {

  fseek(f, SEEK_STATUS, SEEK_SET);

  fread(&hr->status, sizeof(hr->status), 1, f);
  fread(&hr->topoDaLista, sizeof(hr->topoDaLista), 1, f);
  fread(&hr->nroEstacoes, sizeof(hr->nroEstacoes), 1, f);
  fread(&hr->nroParesEstacao, sizeof(hr->nroParesEstacao), 1, f);
}

/**
 * @brief Esta função realiza a leitura de 1 registro do arquivo binario a partir da
 * posicao corrente de leitura do arquivo.
 * OBS: O arquivo ja deve estar aberto para leitura no modo binario.
 * 
 * @param f FILE do arquivo binario de leitura.
 * @param dr Variável onde serao salvos os dados do registro (se este nao estiver removido).
 * @return END_OF_FILE_BIN caso o final do arquivo seja alcancado, REMOVED caso o registro 
 * lido esteja logicamente removido ou NOT_REMOVED caso tenha ocorrido uma leitura com sucesso.
 *
 * @author Leonardo Hannas de Carvalho Santos
 * @author Lucas Carvalho Freiberger Stapf
 */
int readDataRegisterBIN(FILE *f, DataRegister *dr) {

  int ret = fread(&dr->removido, sizeof(dr->removido), 1, f);
  if (ret == 0) return END_OF_FILE_BIN;

  fread(&dr->tamanhoRegistro, sizeof(dr->tamanhoRegistro), 1, f);

  LONG_8 pi = ftell(f);
  LONG_8 pf;

  fread(&dr->proxLista, sizeof(dr->proxLista), 1, f);

  if(dr->removido == '1') { // logicamente removido
    fseek(f, dr->tamanhoRegistro - sizeof(dr->proxLista), SEEK_CUR);
    return REMOVED;
  }

  fread(&dr->codEstacao, sizeof(dr->codEstacao), 1, f);
  fread(&dr->codLinha, sizeof(dr->codLinha), 1, f);
  fread(&dr->codProxEstacao, sizeof(dr->codProxEstacao), 1, f);
  fread(&dr->distProxEstacao, sizeof(dr->distProxEstacao), 1, f);
  fread(&dr->codLinhaIntegra, sizeof(dr->codLinhaIntegra), 1, f);
  fread(&dr->codEstIntegra, sizeof(dr->codEstIntegra), 1, f);

  int i = 0;
  do { // Leitura do campo nomeEstacao feita char a char

    fread(&dr->nomeEstacao[i], sizeof(char), 1, f);

    if (dr->nomeEstacao[i] == FIELD_DELIMITER) break;
    else i++;

  } while (dr->nomeEstacao[i] != FIELD_DELIMITER);
  dr->nomeEstacao[i] = '\0'; // Substitui o FIELD_DELIMITER pelo '\0'

  i = 0;
  do { // Leitura do campo nomeLinha feita char a char

    fread(&dr->nomeLinha[i], sizeof(char), 1, f);

    if (dr->nomeLinha[i] == FIELD_DELIMITER) break;
    else i++;

  } while (dr->nomeLinha[i] != FIELD_DELIMITER);
  dr->nomeLinha[i] = '\0'; // Substitui o FIELD_DELIMITER pelo '\0'

  pf = ftell(f);
  LONG_8 bytesLidos = pf - pi;

  char c;
  while (dr->tamanhoRegistro > bytesLidos) { // pular o lixo de memoria.
    fread(&c, sizeof(char), 1, f);
    bytesLidos++;
  } 

  return NOT_REMOVED;
}

/**
 * @brief Esta funcao busca um registro no arquivo de dados BIN com base nos campos não vazios
 * do DataRegister passado. Quando um registro é encontrado, a função para e o registro é salvo no dr_return.
 * OBS: O arquivo já deve estar aberto para a leitura no modo binário.
 * 
 * @param f FILE do arquivo binário de leitura.
 * @param dr_search Variável que contem os campos de busca do registro.
 * @param dr_return Variável que contém o registro lido do arquivo.
 * @return REGISTER_NOT_FOUND caso nao tenha encontrado o registro ou REGISTER_FOUND caso o 
 * registro tenha sido encontrado. 
 *
 * @author Leonardo Hannas de Carvalho Santos
 * @author Lucas Carvalho Freiberger Stapf
 */
int findDataRegisterBIN(FILE *f, DataRegister *dr_search, DataRegister *dr_return) {

    long seek_cur = ftell(f);

    HeaderRegister hr;
    readHeaderRegisterBIN(f, &hr);


    if (statusFileBIN(hr) == FILE_NOT_CONSISTENT) return FILE_NOT_CONSISTENT;

    int found = REGISTER_NOT_FOUND;
    fseek(f, SEEK_FIRST_REGISTER, SEEK_SET); // Volta para primeiro registro do arquivo.

    DataRegister r;
    int ret;

    do {

        ret = readDataRegisterBIN(f, &r);

        if (ret == END_OF_FILE_BIN) break;
        if (ret == REMOVED) continue;
        else {

            if (compareRegister(*dr_search, r) == EQUIVALENT_REGISTERS) {
                found = REGISTER_FOUND;
                copyDataRegister(dr_return, &r);
            }
        }

    } while (ret != END_OF_FILE_BIN && found == REGISTER_NOT_FOUND);

    fseek(f, seek_cur, SEEK_SET);
    return found;
}


/**
 * @brief  Esta funcao printa todos os registros nao removidos do arquivo binario.
 * 
 * @param binName Nome do arquivo binario.
 * @return FILE_ERROR (caso o arquivo nao possa ser aberto),
 *         REGISTER_NOT_FOUND (caso nenhum registro tenha sido encontrado),
 *         REGISTER_FOUND (caso ao menos 1 registro tenha sido encontrado).
 *
 * @author Leonardo Hannas de Carvalho Santos
 * @author Lucas Carvalho Freiberger Stapf
 */
int printFileBIN(char *binName) {

  FILE *f = fopen(binName, "rb");
  if(f == NULL) return FILE_ERROR;

  int found = REGISTER_NOT_FOUND;

  HeaderRegister hr;
  readHeaderRegisterBIN(f, &hr);

  if(statusFileBIN(hr) == FILE_NOT_CONSISTENT) return FILE_NOT_CONSISTENT;

  DataRegister dr;
  fseek(f, SEEK_FIRST_REGISTER, SEEK_SET);

  int ret;

  while(!feof(f)) {

    ret = readDataRegisterBIN(f, &dr);

    if (ret == END_OF_FILE_BIN) break;
    else if(ret == NOT_REMOVED) {
      printRegister(dr);
      found = REGISTER_FOUND;
    } else continue;

    printf("\n");
  }

  fclose(f);

  return found;
}

/**
 * @brief Verifica se o status do arquivo binario eh consistente ou nao atraves do registro de
 * cabecalho.
 * 
 * @param hr Registro de cabecalho do arquivo binario.
 * @return CONSISTENT_FILE se o arquivo esta consiste e FILE_NOT_CONSISTENT caso contrario.
 * 
 * @author Leonardo Hannas de Carvalho Santos
 * @author Lucas Carvalho Freiberger Stapf
 */
int statusFileBIN(HeaderRegister hr) {

  if(hr.status == '1') return CONSISTENT_FILE;
  else return FILE_NOT_CONSISTENT;
}