#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
typedef struct Node {
    int vertex;
    int cost; // Weight or cost of the edge
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    struct Node** adjLists;
    int* visited;
} Graph;

struct Node* createNode(int v, int cost) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->cost = cost;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int vertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->adjLists = (struct Node**)malloc(vertices * sizeof(struct Node*));
    graph->visited = (int*)malloc(vertices * sizeof(int));
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest, int cost) {
    struct Node* newNode = createNode(dest, cost);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    newNode = createNode(src, cost);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void DFS(struct Graph* graph, int vertex) {
    struct Node* adjList = graph->adjLists[vertex];
    struct Node* temp = adjList;
    graph->visited[vertex] = 1;
    while (temp != NULL) {
        int connectedVertex = temp->vertex;

        if (graph->visited[connectedVertex] == 0) {
            DFS(graph, connectedVertex);
        }
        temp = temp->next;
    }
}

int countComponents(struct Graph* graph, int *arrayForStartVertex) {
    int numComponents = 0;
	int idx = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->visited[i] == 0) {
			arrayForStartVertex[idx++] = i;
            DFS(graph, i);
            numComponents++;
        }
    }
    return numComponents;
}

void calculateMinimumCost(struct Graph* graph, int* arrayForStartVertex, int numComponents) {
    for (int i = 0; i < numComponents; i++) {
        int startVertex = arrayForStartVertex[i];
        // Mark all vertices as unvisited
        for (int j = 0; j < graph->numVertices; j++) {
            graph->visited[j] = 0;
        }
		int cost = INT_MAX;
		int vertex = startVertex;
		graph->visited[vertex] = 1;
		int ok = 0;
		int pathCost = 0;
		while (1) {
			cost = INT_MAX;
			ok = 0;
			struct Node* adjList = graph->adjLists[vertex];
			adjList = adjList->next;
			while (adjList != NULL) {
				printf("%d ", adjList->vertex);
				if (graph->visited[adjList->vertex] == 0) {
					ok = 1;
					if (adjList->cost < cost) {
						cost = adjList->cost;
						vertex = adjList->vertex;
					}
				}
				adjList = adjList->next;
			}
			printf("\n");
			if (ok == 0) {
				break;
			}
			graph->visited[vertex] = 1;
			pathCost += cost;
		}
        printf("Minimum cost for Component %d (Start vertex: %d): %d\n", i, startVertex, pathCost);
    }
}

int main(int argv, char *argc[]) {
    if(atoi(argc[1]) == 1) {
    FILE* inputFile = fopen("./input/cerinta1/test0.in", "r");
    if (inputFile == NULL) {
        printf("Failed to open the input file.\n");
        return 1;
    }
    int vertices, edges;
    fscanf(inputFile, "%d %d", &vertices, &edges);
    struct Graph* graph = createGraph(vertices);
    int src;
	int dest;
	int index = 0;
	int ok = 0;
	char *srcString = malloc(100);
	char *destString = malloc(100);
	char **array;
	array = malloc(10000);
    int i, cost;
    for (i = 0; i < edges; i++) {
		ok = 0;
        fscanf(inputFile, "%s", srcString);
        fscanf(inputFile, "%s", destString);
		for (int j = 0; j < index; j++) {
			if (strcmp(array[j], srcString) == 0) {
				src = j;
				ok = 1;
				break;
			}
		}
		if (ok == 0) {
			src = index;
			array[index] = malloc(100);
			strcpy(array[index], srcString);
			index++;
		}
		ok = 0;
		for (int j = 0; j < index; j++) {
			if (strcmp(array[j], destString) == 0) {
				dest = j;
				ok = 1;
				break;
			}
		}
		if (ok == 0) {
			dest = index;
			array[index] = malloc(100);
			strcpy(array[index], destString);
			index++;
		}

        fscanf(inputFile, "%d", &cost);
        addEdge(graph, src, dest, cost);
	    }

    fclose(inputFile);
	for (int j = 0; j < graph->numVertices; j++) {
		printf("%d %d\n", graph->adjLists[j]->vertex, j);
	}
	printf("\n\n");
	for (int j = 0; j < graph->numVertices; j++) {
		printf("%s %d\n", array[j], j);
	}

    int startVertex = 0;
    //DFS(graph, startVertex);
	int *arrayForStartVertex = malloc(vertices * sizeof(int));
    int numComponents = countComponents(graph, arrayForStartVertex);
	calculateMinimumCost(graph, arrayForStartVertex, numComponents);
	for (int j = 0; j < numComponents; j++) {
		printf("Start vertex for component %d is %d\n", j, arrayForStartVertex[j]);
	}
    FILE* outputFile = fopen("tema3.out", "w");
    if (outputFile == NULL) {
        printf("Failed to open the output file.\n");
        return 1;
    }
	printf("Numarul de componente = %d\n", numComponents);
    fprintf(outputFile, "%d\n", numComponents);
    fclose(outputFile);

    return 0;
}
}