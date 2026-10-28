#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <queue>

using namespace std;

bool show = false;

int** generate_adjacency_matrix(int size, int edge_percent) {
    int **G = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        G[i] = (int *)malloc(size * sizeof(int));
    }

    for (int i = 0; i < size; i++) {
        G[i][i] = 0;
        for (int j = i + 1; j < size; j++) {
            int val = (rand() % 100 < edge_percent) ? 1 : 0;
            G[i][j] = val;
            G[j][i] = val;
        }
    }
    return G;
}

void free_matrix(int **G, int size) {
    for (int i = 0; i < size; i++) {
        free(G[i]);
    }
    free(G);
}

void print_matrix(int **G, int size) {
    printf("   ");
    for (int j = 0; j < size; j++) {
        printf("v%-3d", j);
    }
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("v%-2d ", i);
        for (int j = 0; j < size; j++) {
            printf("%-4d", G[i][j]);
        }
        printf("\n");
    }
}

struct Node {
    int vertex;
    struct Node* next;
};

struct Node* createNode(int v) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

void push_node(struct Node** head, int v) {
    struct Node* newNode = createNode(v);
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void pop_node(struct Node** head) {
    if (*head == NULL) return;
    struct Node* temp = *head;
    *head = (*head)->next;
    free(temp);
}

void print_lists(struct Node** adjLists, int size) {
    for (int i = 0; i < size; i++) {
        printf("v%d: ", i);
        struct Node* temp = adjLists[i];
        while (temp != NULL) {
            printf("%d ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

queue<int>* build_lists_std(int size, int **G) {
    queue<int>* adjLists = new queue<int>[size];
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (G[i][j] == 1) {
                adjLists[i].push(j);
                adjLists[j].push(i);
            }
        }
    }
    return adjLists;
}

void print_lists_std(queue<int>* adjLists, int size) {
    for (int i = 0; i < size; i++) {
        printf("v%d: ", i);
        queue<int> temp = adjLists[i];
        while (!temp.empty()) {
            printf("%d ", temp.front());
            temp.pop();
        }
        printf("\n");
    }
}

struct Node** build_lists_custom(int size, int **G) {
    struct Node** adjLists = (struct Node**)malloc(size * sizeof(struct Node*));
    for (int i = 0; i < size; i++) {
        adjLists[i] = NULL;
    }
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (G[i][j] == 1) {
                push_node(&adjLists[i], j);
                push_node(&adjLists[j], i);
            }
        }
    }
    return adjLists;
}

void free_lists_custom(struct Node** adjLists, int size) {
    for (int i = 0; i < size; i++) {
        while (adjLists[i] != NULL) {
            pop_node(&adjLists[i]);
        }
    }
    free(adjLists);
}

// Задание 1

void bfs_matrix_queue(int v, int size, int **G, bool *NUM) {
    queue<int> Q;
    Q.push(v);
    NUM[v] = true;

    while (!Q.empty()) {
        v = Q.front();
        Q.pop();
        if (show) printf("%d ", v);

        for (int i = 0; i < size; i++) {
            if (G[v][i] == 1 && !NUM[i]) {
                Q.push(i);
                NUM[i] = true;
            }
        }
    }
}

void run_task_1(int size, int **G, int start) {
    bool *NUM = (bool *)calloc(size, sizeof(bool));

    printf("\nОбход в ширину (Матрица смежности)\n");
    printf("Стартовая вершина: %d\n", start);
    if (show) printf("Порядок обхода вершин: ");

    if (!NUM[start]) {
        bfs_matrix_queue(start, size, G, NUM);
    }

    for (int i = 0; i < size; i++) {
        if (!NUM[i]) {
            bfs_matrix_queue(i, size, G, NUM);
        }
    }

    if (show) printf("\n");
    free(NUM);
}

void bfs_list_queue(int v, queue<int>* adjLists, bool *NUM) {
    queue<int> Q;
    Q.push(v);
    NUM[v] = true;

    while (!Q.empty()) {
        v = Q.front();
        Q.pop();
        if (show) printf("%d ", v);

        queue<int> temp = adjLists[v];
        while (!temp.empty()) {
            int adjVertex = temp.front();
            temp.pop();
            if (!NUM[adjVertex]) {
                Q.push(adjVertex);
                NUM[adjVertex] = true;
            }
        }
    }
}

void run_task_1_lists(int size, queue<int>* adjLists, int start) {
    bool *NUM = (bool *)calloc(size, sizeof(bool));

    printf("\nОбход в ширину (Списки смежности)\n");
    printf("Стартовая вершина: %d\n", start);
    if (show) printf("Порядок обхода вершин: ");

    if (!NUM[start]) {
        bfs_list_queue(start, adjLists, NUM);
    }

    for (int i = 0; i < size; i++) {
        if (!NUM[i]) {
            bfs_list_queue(i, adjLists, NUM);
        }
    }

    if (show) printf("\n");
    free(NUM);
}

// Задание 2

void bfs_custom_queue(int v, int size, int **G, bool *NUM) {
    struct Node* Q = NULL;

    push_node(&Q, v);
    NUM[v] = true;

    while (Q != NULL) {
        v = Q->vertex;
        pop_node(&Q);
        if (show) printf("%d ", v);

        for (int i = 0; i < size; i++) {
            if (G[v][i] == 1 && !NUM[i]) {
                push_node(&Q, i);
                NUM[i] = true;
            }
        }
    }
}

void run_task_2(int size, int **G, int start) {
    bool *NUM = (bool *)calloc(size, sizeof(bool));

    printf("\nОбход в ширину (Матрица смежности)\n");
    printf("Стартовая вершина: %d\n", start);
    if (show) printf("Порядок обхода вершин: ");

    if (!NUM[start]) {
        bfs_custom_queue(start, size, G, NUM);
    }

    for (int i = 0; i < size; i++) {
        if (!NUM[i]) {
            bfs_custom_queue(i, size, G, NUM);
        }
    }

    if (show) printf("\n");
    free(NUM);
}

void bfs_list_custom_queue(int v, struct Node** adjLists, bool *NUM) {
    struct Node* Q = NULL;

    push_node(&Q, v);
    NUM[v] = true;

    while (Q != NULL) {
        v = Q->vertex;
        pop_node(&Q);
        if (show) printf("%d ", v);

        struct Node* temp = adjLists[v];
        while (temp != NULL) {
            int adjVertex = temp->vertex;
            if (!NUM[adjVertex]) {
                push_node(&Q, adjVertex);
                NUM[adjVertex] = true;
            }
            temp = temp->next;
        }
    }
}

void run_task_2_lists(int size, struct Node** adjLists, int start) {
    bool *NUM = (bool *)calloc(size, sizeof(bool));

    printf("\nОбход в ширину (Списки смежности)\n");
    printf("Стартовая вершина: %d\n", start);
    if (show) printf("Порядок обхода вершин: ");

    if (!NUM[start]) {
        bfs_list_custom_queue(start, adjLists, NUM);
    }

    for (int i = 0; i < size; i++) {
        if (!NUM[i]) {
            bfs_list_custom_queue(i, adjLists, NUM);
        }
    }

    if (show) printf("\n");
    free(NUM);
}

int main() {
    srand(time(NULL));
    int size;
    int edge_percent;
    int start;

    printf("Введите количество вершин графа: ");
    if (scanf("%d", &size) != 1 || size <= 0) {
        printf("Ошибка: некорректное количество вершин.\n");
        return 1;
    }

    printf("Введите вероятность появления ребра в процентах (от 0 до 100): ");
    if (scanf("%d", &edge_percent) != 1 || edge_percent < 0 || edge_percent > 100) {
        printf("Ошибка: некорректное значение вероятности.\n");
        return 1;
    }

    int answer;
    printf("Выводить матрицу, списки смежности и порядок обхода? (1 - да, 0 - нет): ");
    if (scanf("%d", &answer) != 1 || (answer != 0 && answer != 1)) {
        printf("Ошибка: нужно ввести 1 или 0.\n");
        return 1;
    }
    show = (answer == 1);

    int **G = generate_adjacency_matrix(size, edge_percent);

    if (show) {
        printf("\nСгенерированная матрица смежности G (%d вершин, вероятность ребра %d%%):\n",
               size, edge_percent);
        print_matrix(G, size);
    }

    printf("Введите начальную вершину (от 0 до %d): ", size - 1);
    if (scanf("%d", &start) != 1 || start < 0 || start >= size) {
        printf("Ошибка: некорректная начальная вершина.\n");
        free_matrix(G, size);
        return 1;
    }

    queue<int>* adjListsStd = build_lists_std(size, G);
    struct Node** adjListsCustom = build_lists_custom(size, G);

    if (show) {
        printf("\nСписки смежности графа:\n");
        print_lists(adjListsCustom, size);
    }

    clock_t t1, t2;
    double durations;

    printf("\nЗАДАНИЕ №1: Использование очереди из стандартной библиотеки queue\n");

    t1 = clock();
    run_task_1(size, G, start);
    t2 = clock();
    durations = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Время выполнения: %.6f сек.\n", durations);

    t1 = clock();
    run_task_1_lists(size, adjListsStd, start);
    t2 = clock();
    durations = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Время выполнения: %.6f сек.\n", durations);

    printf("\nЗАДАНИЕ №2: Использование пользовательской очереди\n");

    t1 = clock();
    run_task_2(size, G, start);
    t2 = clock();
    durations = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Время выполнения: %.6f сек.\n", durations);

    t1 = clock();
    run_task_2_lists(size, adjListsCustom, start);
    t2 = clock();
    durations = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Время выполнения: %.6f сек.\n", durations);

    delete[] adjListsStd;
    free_lists_custom(adjListsCustom, size);
    free_matrix(G, size);

    return 0;
}