#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <queue> 
using namespace std;

typedef struct node {
    int data;
    struct node* next;
} node;

typedef struct stack {
    int* data;
    int top;
    int capacity;
} stack;

stack* createStack(int capacity) {
    stack* s = (stack*)malloc(sizeof(stack));
    s->data = (int*)malloc(capacity * sizeof(int));
    s->top = -1;
    s->capacity = capacity;
    return s;
}

void push(stack* s, int value) {
    if (s->top == s->capacity - 1) {
        printf("Stack overflow\n");
        return;
    }
    s->data[++s->top] = value;
}

int pop(stack* s) {
    if (s->top == -1) {
        printf("Stack underflow\n");
        return -1;
    }
    return s->data[s->top--];
}

int isEmpty(stack* s) {
    return s->top == -1;
}

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

node** createAdj(int** G, int size) {
    node** A = (node**)malloc(size * sizeof(node*));
    for (int i = 0; i < size; ++i) {
        A[i] = NULL;
        node* tail = NULL;
        for (int j = 0; j < size; ++j) {
            if (G[i][j] == 1) {
                node* newNode = (node*)malloc(sizeof(node));
                newNode->data = j;
                newNode->next = NULL;
                if (tail == NULL) {
                    A[i] = newNode;
                }
                else {
                    tail->next = newNode;
                }
                tail = newNode;
            }
        }
    }
    return A;
}

void printAdj(node** A, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d: ", i);
        node* current = A[i];
        while (current != NULL) {
            printf("%d ", current->data);
            current = current->next;
        }
        printf("\n");
    }
    printf("\n");
}

void DFSNR(int** G, int n, int* vis, int s) {
    stack* st = createStack(n);
    push(st, s);
    vis[s] = 1;
    //printf("%d ", s);

    while (!isEmpty(st)) {
        int v = st->data[st->top];
        int found = 0;

        for (int i = 0; i < n; ++i) {
            if (G[v][i] == 1 && vis[i] == 0) {
                push(st, i);
                vis[i] = 1;
                //printf("%d ", i);
                found = 1;
                break;
            }
        }

        if (!found) {
            pop(st);
        }
    }
}

void DFS(int** G, int size, int* vis, int s) {
    vis[s] = 1;
    //printf("%d ", s);
    for (int i = 0; i < size; ++i) {
        if (G[s][i] == 1 && vis[i] == 0) {
            DFS(G, size, vis, i);
        }
    }
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

void DiFSD(int** G, int size, int* dist, int s, int d = 0) {
    dist[s] = d;
    for (int i = 0; i < size; ++i) {
        if (G[s][i] == 1 && (dist[i] == -1 || dist[i] > d)) {
            d++;
            DiFSD(G, size, dist, i, d);
            d--;
        }
    }
}

void DiFSA(node** A, int size, int* dist, int s) {
    queue <int> q;
    q.push(s);
    dist[s] = 0;
    node* current = NULL;
    while (!q.empty()) {
        current = A[q.front()];
        //printf("%d ", q.front());
        s = q.front();
        q.pop();

        while (current) {
            if (dist[current->data] == -1) {
                q.push(current->data);
                dist[current->data] = dist[s] + 1;
            }
            current = current->next;
        }
    }
}

void DiFSDA(node** A, int* dist, int s, int d = 0) {
    dist[s] = d;
    node* current = A[s];
    while (current != NULL) {
        if (dist[current->data] == -1 || dist[current->data] > d) {
            d++;
            DiFSDA(A, dist, current->data, d);
            d--;
        }
        current = current->next;
    }
}

int main() {
    setlocale(LC_ALL, "Rus");
    int n = 100;
    int** M1 = NULL;
    node** A1 = NULL;
    //printf("Введите количество вершин: ");
    srand(time(NULL));

    int* dist = (int*)malloc(sizeof(int) * n);
    
    M1 = createG(n);
    A1 = createAdj(M1, n);
    printf("1");
    //printG(M1, n);
    
    clock_t start, end;
    FILE* file = fopen("result.csv", "w");
    for (int i = 0; i < n; ++i) dist[i] = -1;
    start = clock();
    DiFS(M1, n, dist, 0);
    end = clock();

    //for (int i = 0; i < n; ++i) printf("%d %2d\n", i, dist[i]);

    float spent_time = (float)(end - start) / CLOCKS_PER_SEC;
    printf("DiFS;%.3f;\n", spent_time);
    fprintf(file, "DiFS;%.3f;\n", spent_time);


    for (int i = 0; i < n; ++i) dist[i] = -1;
    //printAdj(A1, n);
    start = clock();
    DiFSA(A1, n, dist, 0);
    end = clock();
    //for (int i = 0; i < n; ++i) printf("%d %2d\n", i, dist[i]);
    spent_time = (float)(end - start) / CLOCKS_PER_SEC;
    printf("DiFSA;%.3f;\n", spent_time);
    fprintf(file, "DiFSA;%.3f;\n", spent_time);

    for (int i = 0; i < n; ++i) dist[i] = -1;
    start = clock();
    DiFSD(M1, n, dist, 0);
    end = clock();
    //for (int i = 0; i < n; ++i) printf("%d %2d\n", i, dist[i]);
    spent_time = (float)(end - start) / CLOCKS_PER_SEC;
    printf("DiFSD;%.3f;\n", spent_time);
    fprintf(file, "DiFSD;%.3f;\n", spent_time);

    for (int i = 0; i < n; ++i) dist[i] = -1;
    start = clock();
    DiFSDA(A1, dist, 0);
    end = clock();
    //for (int i = 0; i < n; ++i) printf("%d %2d\n", i, dist[i]);
    spent_time = (float)(end - start) / CLOCKS_PER_SEC;
    printf("DiFSDA;%.3f;\n", spent_time);
    fprintf(file, "DiFSDA;%.3f;\n", spent_time);

    fclose(file);

    for (int i = 0; i < n; ++i) dist[i] = 0;
    start = clock();
    DFSNR(M1, n, dist, 0);
    end = clock();
    spent_time = (float)(end - start) / CLOCKS_PER_SEC;
    printf("\n\nDFSNR;%.3f;\n", spent_time);

    for (int i = 0; i < n; ++i) dist[i] = 0;
    start = clock();
    DFS(M1, n, dist, 0);
    end = clock();
    spent_time = (float)(end - start) / CLOCKS_PER_SEC;
    printf("DFS;%.3f;\n", spent_time);


    free(M1);
    free(A1);
    free(dist);
}