#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define N 9
int sudoku[N][N];
int resultado = 1;
pthread_mutex_t lock;
clock_t tempo_execucao_linha = 0, tempo_execucao_coluna = 0, tempo_execucao_quadro = 0; //calcula o tempo de execução total para as threads de linha, coluna e quadros


void *validar_linhas(void *param) {


    clock_t tempo = clock();

    for (int linha = 0; linha < N; linha++){
        int seen[N] = {0}; //Array que define quais numeros ja foram vistos
        for (int coluna = 0; coluna < N; coluna++){

            // Verifica se alguma thread já identificou um erro, se tiver a operação é encerrada
            pthread_mutex_lock(&lock);
            if (resultado == 0) {
                tempo = clock() - tempo;
                tempo_execucao_linha = tempo; //define o tempo de execução da thread de linha
                pthread_mutex_unlock(&lock);
                pthread_exit(NULL); //encerra a operação
            }
            pthread_mutex_unlock(&lock);

            int val = sudoku[linha][coluna];

            if (val == 0) continue;

            // Verifica se o número está fora do intervalo ou repetido
            if (val < 1 || val > 9 || seen[val - 1]) {
                pthread_mutex_lock(&lock); //abre o lock para alterar a variavel resultado e tempo_execucao_linha
                resultado = 0; //avisa que uma das colunas esta incorreta
                tempo = clock() - tempo;
                tempo_execucao_linha = tempo; //define o tempo de execução da thread de linha
                pthread_mutex_unlock(&lock); //destrava o lock
                pthread_exit(NULL); //encerra a operação
            }
        }
    }
    tempo = clock() - tempo;
    tempo_execucao_linha = tempo; //define o tempo de execução da thread de linha
    pthread_exit(NULL); //encerra a operação
}

void* validar_colunas(void *param) {

    clock_t tempo = clock();

    for (int linha = 0; linha < N; linha++){

        int seen[N] = {0}; //Array que define quais numeros ja foram vistos

        for (int coluna = 0; coluna < N; coluna++){

            // Verifica se alguma thread já identificou um erro, se tiver a operação é encerrada
            pthread_mutex_lock(&lock);
            if (resultado == 0) {
                tempo = clock() - tempo;
                tempo_execucao_coluna = tempo; //define o tempo de execução da thread de coluna
                pthread_mutex_unlock(&lock);
                pthread_exit(NULL); //encerra a operação
            }

            pthread_mutex_unlock(&lock);

            int val = sudoku[coluna][linha];

            if (val == 0) continue;

            // Verifica se o número está fora do intervalo ou repetido
            if (val < 1 || val > 9 || seen[val - 1]) {
                pthread_mutex_lock(&lock); //abre o lock para alterar a variavel resultado e tempo_execucao_coluna
                resultado = 0; //avisa que uma das colunas esta incorreta
                tempo = clock() - tempo;
                tempo_execucao_coluna = tempo; //define o tempo de execução da thread de coluna
                pthread_mutex_unlock(&lock);
                pthread_exit(NULL); //encerra a operação
            }
        }
    }

    tempo = clock() - tempo;
    tempo_execucao_coluna = tempo; //define o tempo de execução da thread de coluna
    pthread_exit(NULL); //encerra a operação
}

void *validar_quadros(void *param) {
    clock_t tempo = clock();

    for (int quadro =0; quadro < N; quadro++){
        int seen[N] = {0}; //Array que define quais numeros ja foram vistos
        int start_row = (quadro / 3) * 3; //calcula a linha inicial
        int start_col = (quadro % 3) * 3; //calcula a coluna inicial

        for (int i=0; i < 3; i++){
            for (int j = 0; j < 3; j++){

                // Verifica se alguma thread já identificou um erro, se tiver a operação é encerrada
                pthread_mutex_lock(&lock);
                if (resultado == 0) {
                    tempo = clock() - tempo;
                    tempo_execucao_quadro = tempo; //define o tempo de execução da thread de quadros
                    pthread_mutex_unlock(&lock);
                    pthread_exit(NULL); //encerra a operação
                }
                pthread_mutex_unlock(&lock);

                //Define o elemento do quadro a ser checado
                int val = sudoku[start_row + i][start_col + j];
                // Se o valor for 0, passa para o próximo (jogo sudoku em andamento)
                if (val == 0) continue;

                // Verifica se o número está fora do intervalo ou repetido
                if (val < 1 || val > 9 || seen[val - 1]) {
                    pthread_mutex_lock(&lock); //abre o lock para alterar a variavel resultado
                    resultado = 0; //avisa que uma das colunas esta incorreta
                    tempo = clock() - tempo;
                    tempo_execucao_quadro = tempo; //define o tempo de execução da thread de quadros
                    pthread_mutex_unlock(&lock);
                    pthread_exit(NULL); //encerra a operação
                }
            seen[val - 1] = 1; //marca o numero como ja visto
            }
        }
    }
    tempo = clock() - tempo;
    tempo_execucao_quadro = tempo; //define o tempo de execução da thread de quadros
    pthread_exit(NULL); //encerra a operação
}


int verificar_puzzle(int *puzzle){
    resultado = 1;
    pthread_t threads[3];
    clock_t tempo_criacao, tempo_execucao;
    FILE *arq;

    // Copia o puzzle para a matriz global sudoku
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sudoku[i][j] = puzzle[i * 9 + j];
        }
    }

    arq = fopen("/app/resultados/resultados2.txt", "w"); //cria arquivo para registrar o tempo de execução das verificações
    if (arq== NULL) {
        printf("Erro ao criar o arquivo!\n");
        exit(1);
    }

    fprintf(arq, "Executando verificação....\n");

    tempo_execucao = clock();

    pthread_mutex_init(&lock, NULL); //inicia o lock

    tempo_criacao = clock();

    pthread_create(&threads[0], NULL, validar_linhas, NULL);
    pthread_create(&threads[1], NULL, validar_colunas, NULL);
    pthread_create(&threads[2], NULL, validar_quadros, NULL);

    tempo_criacao = clock() - tempo_criacao;

    //espera a finalização das threads
    for (int i=0; i < 3; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock); //encerra o lock

    tempo_execucao = clock() - tempo_execucao;

    // escreve no arquivo resultados.txt o tempo de execução para análise
    fprintf(arq, "Tempo de execução das threads de linha: %.3f ms\n", ((double)tempo_execucao_linha)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de execução das threads de coluna: %.3f ms\n", ((double)tempo_execucao_coluna)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de execução das threads de quadros: %.3f ms\n\n", ((double)tempo_execucao_quadro)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de criação das threads: %.3f ms\n", ((double)tempo_criacao)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de execução total das threads: %.3f ms\n", ((double)tempo_execucao)/((CLOCKS_PER_SEC/1000)));
    fclose(arq);
    return resultado;
}

