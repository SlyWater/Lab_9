#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <queue> 
using namespace std;


int** createG(int size) {
    int** G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; ++i) {
        G[i] = (int*)malloc(size * sizeof(int));
    }
    for (int i = 0; i < size; ++i) {
        for (int j = i; j < size; ++j) {
            G[i][j] = rand() % 2;
            G[j][i] = (i == j) ? 0 : G[i][j];
        }
    }
    return G;
}

void printG(int** G, int size) {
    printf("  ");
    for (int i = 0; i < size; ++i) printf("%d ", i);
    printf("\n");
    for (int i = 0; i < size; ++i) {
        printf("%d ", i);
        for (int j = 0; j < size; ++j) {
            printf("%d ", G[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}



void DiFS(int** G, int size, int* dist, int s) {
    queue <int> q;
    q.push(s);
    dist[s] = 0;

    while (!q.empty()) {
        s = q.front();
        q.pop();

        for (int i = 0; i < size; ++i) {
            if (G[s][i] && dist[i] == -1) {
                q.push(i);
                dist[i] = dist[s] + 1;

            }
        }
    }
}

int main() {

    clock_t start, end;
    setlocale(LC_ALL, "Rus");
    int n = 4;
    int** M1 = NULL;
    //printf("Введите количество вершин: ");
    srand(time(NULL));

    int* dist = (int*)malloc(sizeof(int) * n);

    M1 = createG(n);
    printG(M1, n);
    for (int i = 0; i < n; ++i) dist[i] = -1;
    DiFS(M1, n, dist, 0);
    for (int i = 0; i < n; ++i) printf("%d %d\n", i, dist[i]);

    free(M1);
    free(dist);
}