#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define N 9

int sudoku[N][N]; //matriz representando o jogo sudoku
int resultado = 1; //variavel que representa se há erro (0) ou não (1) no sudoku
pthread_mutex_t lock;
clock_t tempo_execucao_linha = 0, tempo_execucao_coluna = 0, tempo_execucao_quadro = 0; //calcula o tempo de execução total para as threads de linha, coluna e quadros


void *validar_linhas(void *param) {
    int row = *(int *)param; //seleção da linha do sudoku
    int seen[N] = {0}; //Array que define quais numeros ja foram vistos
    clock_t tempo = clock();

    for (int i = 0; i < N; i++) {

        // Verifica se alguma thread já identificou um erro, se tiver a operação é encerrada
        pthread_mutex_lock(&lock);
        if (resultado == 0) {
            tempo = clock() - tempo;
            tempo_execucao_linha += tempo;
            pthread_mutex_unlock(&lock);
            pthread_exit(NULL); //encerra a operação
        }
        pthread_mutex_unlock(&lock);
        int val = sudoku[row][i]; //seleciona o numero da linha 

        // Se o valor for 0, passa para o próximo (jogo sudoku em andamento)
        if (val == 0)
            continue;

        // Verifica se o número está fora do intervalo ou repetido
        if (val < 1 || val > 9 || seen[val - 1]) {
            pthread_mutex_lock(&lock); //abre o lock para alterar a variavel resultado
            resultado = 0; //avisa que uma das linhas esta incorreta
            tempo = clock() - tempo;
            tempo_execucao_linha += tempo;
            pthread_mutex_unlock(&lock); //destrava o lock
            pthread_exit(NULL); //encerra a operação
        }

        seen[val - 1] = 1; //marca o numero como ja visto
    }

    pthread_mutex_lock(&lock); //abre lock para alterar a variavel tempo_execucao_linha
    tempo = clock() - tempo;
    tempo_execucao_linha += tempo; // calcula o tempo de execução da thread
    pthread_mutex_unlock(&lock); //destrava o lock
    pthread_exit(NULL); //encerra a operação
}

void *validar_colunas(void *param) {
    int col = *(int *)param; //seleção da coluna do sudoku
    int seen[N] = {0}; //Array que define quais numeros ja foram vistos
    clock_t tempo = clock();

    for (int i = 0; i < N; i++) {

        // Verifica se alguma thread já identificou um erro, se tiver a operação é encerrada
        pthread_mutex_lock(&lock);
        if (resultado == 0) {
            tempo = clock() - tempo;
            tempo_execucao_coluna += tempo; //adiciona tempo
            pthread_mutex_unlock(&lock);
            pthread_exit(NULL); //encerra a operação
        }
        pthread_mutex_unlock(&lock);

        int val = sudoku[i][col]; //seleciona o numero da coluna

        // Se o valor for 0, passa para o próximo (jogo sudoku em andamento)
        if (val == 0)
            continue;

        // Verifica se o número está fora do intervalo ou repetido
        if (val < 1 || val > 9 || seen[val - 1]) {
            pthread_mutex_lock(&lock); //abre o lock para alterar a variavel resultado e tempo_execucao_coluna
            resultado = 0; //avisa que uma das colunas esta incorreta
            tempo = clock() - tempo;
            tempo_execucao_coluna += tempo; //adiciona tempo
            pthread_mutex_unlock(&lock); //destrava o lock
            pthread_exit(NULL); //encerra a operação
        }

        seen[val - 1] = 1; //marca o numero como ja visto
    }

    pthread_mutex_lock(&lock); //abre lock para alterar a variavel tempo_execucao_coluna
    tempo = clock() - tempo;
    tempo_execucao_coluna += tempo;
    pthread_mutex_unlock(&lock); //destrava o lock

    pthread_exit(NULL); //encerra a operação
}

void *validar_quadro(void *param) {
    int quadro = *(int *)param; //seleção da indice base do quadro
    int seen[N] = {0}; //Array que define quais numeros ja foram vistos
    clock_t tempo = clock(); // calcula o tempo de execução da thread

    int start_row = (quadro / 3) * 3; //calcula a linha inicial
    int start_col = (quadro % 3) * 3; //calcula a coluna inicial

    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) {

            // Verifica se alguma thread já identificou um erro, se tiver a operação é encerrada
            pthread_mutex_lock(&lock);
            if (resultado == 0) {
                tempo = clock() - tempo;
                tempo_execucao_quadro += tempo; //adiciona o tempo 
                pthread_mutex_unlock(&lock);
                pthread_exit(NULL); //encerra a operação
            }
            pthread_mutex_unlock(&lock);
            //Define o elemento do quadro a ser checado
            int val = sudoku[start_row + i][start_col + j];

            // Se o valor for 0, passa para o próximo (jogo sudoku em andamento)
            if (val == 0)
                continue;

            // Verifica se o número está fora do intervalo ou repetido
            if (val < 1 || val > 9 || seen[val - 1]) {
                pthread_mutex_lock(&lock); //abre o lock para alterar a variavel resultado e de tempo_execucao_quadro
                resultado = 0; //avisa que uma das colunas esta incorreta
                tempo = clock() - tempo;
                tempo_execucao_quadro += tempo; //adiciona o tempo
                pthread_mutex_unlock(&lock); //destrava o lock
                pthread_exit(NULL); //encerra a operação
            }

            seen[val - 1] = 1; //marca o numero como ja visto
        }
    }

    pthread_mutex_lock(&lock); //abre lock para alterar a variavel tempo_execucao_quadro
    tempo = clock() - tempo;
    tempo_execucao_quadro += tempo; //adiciona o tempo
    pthread_mutex_unlock(&lock); //destrava o lock

    pthread_exit(NULL); //encerra a operação
}

int verificar_puzzle(int *puzzle){
    resultado = 1; //seta resultado para 1
    FILE *arq;
    pthread_t threads_coluna[N]; //Declara as threads da colunas
    pthread_t threads_quadro[N]; //Declara as threads dos quadros 3 x 3
    pthread_t threads_linhas[N]; // Definição de threads das linhas
    int indices[N]; //armazena os indices das colunas
    clock_t tempo_criacao, tempo_execucao;
    tempo_execucao_linha = 0;
    tempo_execucao_coluna = 0;
    tempo_execucao_quadro = 0;

    // Copia o puzzle para a matriz global sudoku
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sudoku[i][j] = puzzle[i * 9 + j];
        }
    }

    arq = fopen("/app/resultados/resultados.txt", "w"); //cria arquivo para registrar o tempo de execução das verificações
    if (arq== NULL) {
        printf("Erro ao criar o arquivo!\n");
        exit(1);
    }

    fprintf(arq, "Executando verificação....\n");

    tempo_execucao = clock();

    pthread_mutex_init(&lock, NULL); //inicia o lock

    tempo_criacao = clock();

    // Cria as threads da verificação das linhas
    for (int i = 0; i < N; i++) {
        indices[i] = i;
        pthread_create(&threads_linhas[i], NULL, validar_linhas, &indices[i]);
        pthread_create(&threads_coluna[i], NULL, validar_colunas, &indices[i]);
        pthread_create(&threads_quadro[i], NULL, validar_quadro, &indices[i]);
    }

    tempo_criacao = clock() - tempo_criacao; //calcula o tempo de criação das threads

    // Aguarda o termino das threads
    for (int i = 0; i < N; i++) {
        pthread_join(threads_linhas[i], NULL);
        pthread_join(threads_coluna[i], NULL);
        pthread_join(threads_quadro[i], NULL);
    }

    pthread_mutex_destroy(&lock); //encerra o lock

    tempo_execucao = clock() - tempo_execucao; // calcula o tempo de execução total

    // escreve no arquivo resultados.txt o tempo de execução para análise
    fprintf(arq, "Tempo de execução das threads de linha: %.3f ms\n", ((double)tempo_execucao_linha)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de execução das threads de coluna: %.3f ms\n", ((double)tempo_execucao_coluna)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de execução das threads de quadros: %.3f ms\n\n", ((double)tempo_execucao_quadro)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de criação das threads: %.3f ms\n", ((double)tempo_criacao)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de execução total das threads: %.3f ms\n", ((double)tempo_execucao)/((CLOCKS_PER_SEC/1000)));
    fclose(arq);
    return resultado;
}

