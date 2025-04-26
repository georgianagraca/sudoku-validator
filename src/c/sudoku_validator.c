#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define N 9

#define MAX_LINHA 165 
#define MAX_SUDOKU 82

int sudoku[N][N];
int resultado = 1;
pthread_mutex_t lock;

void get_puzzle(int *puzzle, int *solution) {
    FILE *arq;
    char linha[MAX_LINHA] = {0}, *virg;
    char puzzle_aux[82] = {0};
    char solution_aux[82] = {0};

    //arq = fopen("c/sudoku.csv", "r"); //rodando pelo docker
    arq = fopen("../c/sudoku.csv", "r"); //rodando localmente
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


void *validar_linhas(void *param) {
    int row = *(int *)param; //seleção da linha do sudoku
    int seen[N] = {0}; //Array que define quais numeros ja foram vistos

    for (int i = 0; i < N; i++) {
        int val = sudoku[row][i]; //seleciona o numero da linha 

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

void *validar_quadros(void *param) {
    int quadro = *(int *)param; //seleção da indice base do quadro
    int seen[N] = {0}; //Array que define quais numeros ja foram vistos

    int start_row = (quadro / 3) * 3; //calcula a linha inicial
    int start_col = (quadro % 3) * 3; //calcula a coluna inicial

    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) {
            //Define o elemento do quadro a ser checado
            int val = sudoku[start_row + i][start_col + j];

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

int verificar_puzzle(int *puzzle){
    resultado = 1;
    pthread_t threads[N]; //Definição de Threads das linhas
    int indices[N]; //vetor para indices das linhas

    // Copia o puzzle para a matriz global sudoku
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sudoku[i][j] = puzzle[i * 9 + j];
        }
    }

    pthread_mutex_init(&lock, NULL); //inicia o lock

    // Cria as threads da verificação das linhas
    for (int i = 0; i < N; i++) {
        indices[i] = i;
        pthread_create(&threads[i], NULL, validar_linhas, &indices[i]);
    }

    // Aguarda todas as threads finalizarem
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_t threads_coluna[N]; //Declara as threads da colunas
    int indices_coluna[N]; //armazena os indices das colunas

    // Cria as threads da verificação das colunas
    for (int i = 0; i < N; i++) {
        indices_coluna[i] = i; 
        pthread_create(&threads_coluna[i], NULL, validar_colunas, &indices_coluna[i]);
    }

    // Aguarda o termino das threads das colunas
    for (int i = 0; i < N; i++) {
        pthread_join(threads_coluna[i], NULL);
    }

    pthread_t threads_quadro[N]; //Declara as threads dos quadros 3 x 3
    int indices_quadro[N]; //armazena os indices das quadros

    // Cria as threads da verificação das colunas
    for (int i = 0; i < N; i++) {
        indices_quadro[i] = i; 
        pthread_create(&threads_quadro[i], NULL, validar_quadros, &indices_quadro[i]);
    }

    // Aguarda o termino das threads das colunas
    for (int i = 0; i < N; i++) {
        pthread_join(threads_quadro[i], NULL);
    }

    pthread_mutex_destroy(&lock); //encerra o lock

    return resultado; //retorna 1 se foi validado, e 0 se não foi
}


int main(){
    int puzzle[81], solution[81];
    int retorno;

    // Gera um novo puzzle e sua solução
    //lembre de muda a linha de pegar o puzzle para:
    //arq = fopen("../c/sudoku.csv", "r");
    get_puzzle(puzzle, solution);

    // Copia a solução ou o puzzle para a matriz global 'sudoku'
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            //sudoku[i][j] = puzzle[i * 9 + j];
            sudoku[i][j] = solution[i * 9 + j];
        }
    }

    printf("Sudoku:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", sudoku[i][j]);
            //printf("%d ", puzzle[i][j]);
        }
        printf("\n");
    }

    retorno = verificar_puzzle(puzzle); //checando o sudoku

    if (retorno){
        printf("Todas as linhas são válidas.\n");
    }
    else{
        printf("Alguma linha é inválida.\n");
    }
        
    return 0;
}
