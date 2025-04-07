/*Código C para validação do Sudoku*/

#include <stdio.h>
#include <pthread.h>

#define N 9  // Tamanho do Sudoku

int sudoku[N][N] = {
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

void *validar_linha(void *param) {
    int row = *(int *)param;
    int seen[N] = {0};

    for (int i = 0; i < N; i++) {
        if (seen[sudoku[row][i] - 1]) {
            printf("Linha %d inválida!\n", row + 1);
            return NULL;
        }
        seen[sudoku[row][i] - 1] = 1;
    }
    return NULL;
}

int main() {
    pthread_t threads[N];
    int indices[N];

    for (int i = 0; i < N; i++) {
        indices[i] = i;
        pthread_create(&threads[i], NULL, validar_linha, &indices[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Validação concluída!\n");
    return 0;
}
