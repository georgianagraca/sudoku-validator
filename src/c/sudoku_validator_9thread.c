#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define N 9

int sudoku[N][N];
int resultado = 1;
pthread_mutex_t lock;


void *validar_linhas(void *param) {
    int row = *(int *)param; //seleção da linha do sudoku
    int seen[N] = {0}; //Array que define quais numeros ja foram vistos

    for (int i = 0; i < N; i++) {
        int val = sudoku[row][i]; //seleciona o numero da linha 

        // Se o valor for 0, passa para o próximo (jogo sudoku em andamento)
        if (val == 0)
            continue;

        // Verifica se o número está fora do intervalo ou repetido
        if (val < 1 || val > 9 || seen[val - 1]) {
            pthread_mutex_lock(&lock); //abre o lock para alterar a variavel resultado
            resultado = 0; //avisa que uma das linhas esta incorreta
            pthread_mutex_unlock(&lock); //destrava o lock
            pthread_exit(NULL); //encerra a operação
        }

        seen[val - 1] = 1; //marca o numero como ja visto
    }

    pthread_exit(NULL); //encerra a operação
}

void *validar_colunas(void *param) {
    int col = *(int *)param; //seleção da coluna do sudoku
    int seen[N] = {0}; //Array que define quais numeros ja foram vistos

    for (int i = 0; i < N; i++) {
        int val = sudoku[i][col]; //seleciona o numero da coluna

        // Se o valor for 0, passa para o próximo (jogo sudoku em andamento)
        if (val == 0)
            continue;

        // Verifica se o número está fora do intervalo ou repetido
        if (val < 1 || val > 9 || seen[val - 1]) {
            pthread_mutex_lock(&lock); //abre o lock para alterar a variavel resultado
            resultado = 0; //avisa que uma das colunas esta incorreta
            pthread_mutex_unlock(&lock); //destrava o lock
            pthread_exit(NULL); //encerra a operação
        }

        seen[val - 1] = 1; //marca o numero como ja visto
    }

    pthread_exit(NULL); //encerra a operação
}

void *validar_quadro(void *param) {
    int quadro = *(int *)param; //seleção da indice base do quadro
    int seen[N] = {0}; //Array que define quais numeros ja foram vistos

    int start_row = (quadro / 3) * 3; //calcula a linha inicial
    int start_col = (quadro % 3) * 3; //calcula a coluna inicial

    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) {
            //Define o elemento do quadro a ser checado
            int val = sudoku[start_row + i][start_col + j];

            // Se o valor for 0, passa para o próximo (jogo sudoku em andamento)
            if (val == 0)
                continue;

            // Verifica se o número está fora do intervalo ou repetido
            if (val < 1 || val > 9 || seen[val - 1]) {
                pthread_mutex_lock(&lock); //abre o lock para alterar a variavel resultado
                resultado = 0; //avisa que uma das colunas esta incorreta
                pthread_mutex_unlock(&lock); //destrava o lock
                pthread_exit(NULL); //encerra a operação
            }

            seen[val - 1] = 1; //marca o numero como ja visto
        }
    }

    pthread_exit(NULL); //encerra a operação
}

void* verificar_linhas(void* arg){
    FILE* arq = (FILE*) arg; // Recebe o ponteiro para o arquivo de relatorio*

    // Verificação da linha usando 9 threads
    pthread_t threads_linhas[N]; // Definição de threads das linhas
    int indices[N]; //vetor para indices das linhas
    clock_t tempo_criacao, tempo_execucao;

    tempo_execucao = clock();
    tempo_criacao = clock();

    // Cria as threads da verificação das linhas
    for (int i = 0; i < N; i++) {
        indices[i] = i;
        pthread_create(&threads_linhas[i], NULL, validar_linhas, &indices[i]);
    }

    tempo_criacao = clock() - tempo_criacao;

    // Aguarda todas as threads finalizarem
    for (int i = 0; i < N; i++) {
        pthread_join(threads_linhas[i], NULL);
    }

    tempo_execucao = clock() - tempo_execucao;
    
    pthread_mutex_lock(&lock); //abre o lock para escrever no arquivo
    fprintf(arq, "\nVerificando linhas usando 9 threads...\n");
    fprintf(arq, "Tempo de criação das threads: %.3f ms\n", ((double)tempo_criacao)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de execução das threads: %.3f ms\n", ((double)tempo_execucao)/((CLOCKS_PER_SEC/1000)));
    pthread_mutex_unlock(&lock); //destrava o lock

    // Verificando usando uma única thread
}

void* verificar_colunas(void* arg){
    FILE* arq = (FILE*) arg; // Recebe o ponteiro para o arquivo de relatorio*

    //Verificação usando 9 threads
    pthread_t threads_coluna[N]; //Declara as threads da colunas
    int indices_coluna[N]; //armazena os indices das colunas
    clock_t tempo_criacao, tempo_execucao;

    tempo_execucao = clock();
    tempo_criacao = clock();

    // Cria as threads da verificação das colunas
    for (int i = 0; i < N; i++) {
        indices_coluna[i] = i; 
        pthread_create(&threads_coluna[i], NULL, validar_colunas, &indices_coluna[i]);
    }

    tempo_criacao = clock() - tempo_criacao;

    // Aguarda o termino das threads das colunas
    for (int i = 0; i < N; i++) {
        pthread_join(threads_coluna[i], NULL);
    }

    tempo_execucao = clock() - tempo_execucao;
    pthread_mutex_lock(&lock); //abre o lock para escrever no arquivo
    fprintf(arq, "\nVerificando colunas usando 9 threads...\n");
    fprintf(arq, "Tempo de criação das threads: %.3f ms\n", ((double)tempo_criacao)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de execução das threads: %.3f ms\n", ((double)tempo_execucao)/((CLOCKS_PER_SEC/1000)));
    pthread_mutex_unlock(&lock); //destrava o lock
}

void* verificar_quadros(void* arg){
    FILE* arq = (FILE*) arg; // Recebe o ponteiro para o arquivo de relatorio*

    // Verificação usando 9 threads
    pthread_t threads_quadro[N]; //Declara as threads dos quadros 3 x 3
    int indices_quadro[N]; //armazena os indices das quadros
    clock_t tempo_criacao, tempo_execucao;

    tempo_execucao = clock();
    tempo_criacao = clock();

    // Cria as threads da verificação das colunas
    for (int i = 0; i < N; i++) {
        indices_quadro[i] = i; 
        pthread_create(&threads_quadro[i], NULL, validar_quadro, &indices_quadro[i]);
    }

    tempo_criacao = clock() - tempo_criacao;

    // Aguarda o termino das threads das colunas
    for (int i = 0; i < N; i++) {
        pthread_join(threads_quadro[i], NULL);
    }

    tempo_execucao = clock() - tempo_execucao;
    pthread_mutex_lock(&lock); //abre o lock para escrever no arquivo
    fprintf(arq, "\nVerificando quadros usando 9 threads...\n");
    fprintf(arq, "Tempo de criação das threads: %.3f ms\n", ((double)tempo_criacao)/((CLOCKS_PER_SEC/1000)));
    fprintf(arq, "Tempo de execução das threads: %.3f ms\n", ((double)tempo_execucao)/((CLOCKS_PER_SEC/1000)));
    pthread_mutex_unlock(&lock); //destrava o lock
}

int verificar_puzzle(int *puzzle){
    resultado = 1;
    FILE *arq;

    // Copia o puzzle para a matriz global sudoku
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sudoku[i][j] = puzzle[i * 9 + j];
        }
    }

    arq = fopen("resultados.txt", "w"); //cria arquivo para registrar o tempo de execução das verificações
    if (arq== NULL) {
        printf("Erro ao criar o arquivo!\n");
        exit(1);
    }

    pthread_mutex_init(&lock, NULL); //inicia o lock

    // Declaração das threads que rodam a verificação em paralelo
    pthread_t thread_linhas, thread_colunas, thread_quadros;
    fprintf(arq, "Executando verificação....\n");

    // Criar as 3 threads para fazer a verificação em paralelo
    pthread_create(&thread_linhas, NULL, verificar_linhas, (void*) arq);
    pthread_create(&thread_colunas, NULL, verificar_colunas, (void*) arq);
    pthread_create(&thread_quadros, NULL, verificar_quadros, (void*) arq);

    // Esperar as 3 threads terminarem
    pthread_join(thread_linhas, NULL);
    pthread_join(thread_colunas, NULL);
    pthread_join(thread_quadros, NULL);

    pthread_mutex_destroy(&lock); //encerra o lock
    fclose(arq);
    return resultado;
}

int main(){
    int sudoku[82] = {
        5,3,4,6,7,8,9,1,2,
        6,7,2,1,9,5,3,4,8,
        1,9,8,3,4,2,5,6,7,
        8,5,9,7,6,1,4,2,3,
        4,2,6,8,5,3,7,9,1,
        7,1,3,9,2,4,8,5,6,
        9,6,1,5,3,7,2,8,4,
        2,8,7,4,1,9,6,3,5,
        3,4,5,2,8,6,1,7,9
    };

    if(verificar_puzzle(sudoku)){
        printf("Deu certo o puzzle\n");
    }
    else{
        printf("Existe um erro no puzzle");
    }
}