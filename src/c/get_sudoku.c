#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINHA 165 
#define MAX_SUDOKU 82

void get_puzzle(int *puzzle, int *solution) {
    FILE *arq;
    char linha[MAX_LINHA] = {0}, *virg;
    char puzzle_aux[82] = {0};
    char solution_aux[82] = {0};

    arq = fopen("../c/sudoku.csv", "r"); 
    if (!arq) {
        printf("Erro ao abrir arquivo");
        exit(1);
    }

    // pula a primeira linha do arquivo (cabeçalho)
    if (fgets(linha, MAX_LINHA, arq)) 
        if (linha[strlen(linha) - 1] == '\n') linha[strlen(linha) - 1] = '\0';

    // sorteia índice (representa a linha do arquivo que sera usado como base para gerar o jogo)
    srand(time(NULL));
    int indice = rand() % 1000;

    // ler as linhas do arquivo, até encontrar a linha correspondente ao indice sorteado
    for (int i = 0; i <= indice; i++) {
        if (!fgets(linha, MAX_LINHA, arq)) {
            printf("Erro ao ler linha %d\n", i);
            fclose(arq);
            return;
        }
    }

    // remove quebra de linha
    if (linha[strlen(linha) - 1] == '\n') linha[strlen(linha) - 1] = '\0';

    //separa a linha com base na virgula
    virg = strchr(linha, ',');
    if (virg) {
        *virg = '\0';
        virg++;

        // copia o puzzle
        strncpy(puzzle_aux, linha, MAX_SUDOKU - 1);
        puzzle_aux[MAX_SUDOKU - 1] = '\0';

        //copia a solução
        strncpy(solution_aux, virg, MAX_SUDOKU - 1);
        solution_aux[MAX_SUDOKU - 1] = '\0';
    } 

    fclose(arq);

    // converte para matriz 9x9 de inteiros
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            puzzle[i * 9 + j] = puzzle_aux[i * 9 + j] - '0';
            solution[i * 9 + j] = solution_aux[i * 9 + j] - '0';
        }
    }

    //printf("puzzle_aux: %s\n", puzzle_aux);
    //printf("solucao: %s\n", solution_aux);
}