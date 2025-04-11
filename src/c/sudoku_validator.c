#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 9

int sudoku[N][N];
int resultado = 1;
pthread_mutex_t lock;

void get_puzzle(int rows, int cols, int *puzzle) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            puzzle[i * cols + j] = i + 1; 
        }
    }
}

void *validar_linha(void *param) {
    int row = *(int *)param;
    int seen[N] = {0};

    for (int i = 0; i < N; i++) {
        int val = sudoku[row][i];

        // Verifica se o número está fora do intervalo ou repetido
        if (val < 1 || val > 9 || seen[val - 1]) {
            pthread_mutex_lock(&lock);
            resultado = 0;
            pthread_mutex_unlock(&lock);
            pthread_exit(NULL);
        }

        seen[val - 1] = 1;
    }

    pthread_exit(NULL);
}

/*int main() {
    FILE *file = fopen("sudoku.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo sudoku.txt");
        return 1;
    }

    // Lê a matriz do arquivo
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            fscanf(file, "%d", &sudoku[i][j]);

    fclose(file);

    pthread_t threads[N];
    int indices[N];

    pthread_mutex_init(&lock, NULL);

    // Cria as threads
    for (int i = 0; i < N; i++) {
        indices[i] = i;
        pthread_create(&threads[i], NULL, validar_linha, &indices[i]);
    }

    // Aguarda todas as threads finalizarem
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    return resultado ? 0 : 1;
}*/
