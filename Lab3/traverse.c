#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 20

typedef int Status;

typedef enum { DG, DN, UDG, UDN } GraphKind;
typedef char VertexType;

typedef struct ArcNode {
	int adjvex;
	struct ArcNode *nextarc;
	int weight;
} ArcNode;

typedef struct VNode {
	VertexType data;
	ArcNode *firstarc;
} VNode, AdjList[MAX_VERTEX_NUM];

typedef struct {
	AdjList vertices;
	int vexnum, arcnum;
	GraphKind kind;
} ALGraph;

static void add_arc(ALGraph *G, int tail, int head, int weight) {
	ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
	if (!p) {
		return;
	}
	p->adjvex = head;
	p->weight = weight;
	p->nextarc = G->vertices[tail].firstarc;
	G->vertices[tail].firstarc = p;
}

void CreateALGraph(GraphKind GKind, ALGraph *G, int vexnum, int arcnum, char *vexs, int *arcs) {
	int i;
	G->kind = GKind;
	G->vexnum = vexnum;
	G->arcnum = arcnum;

	for (i = 0; i < vexnum && i < MAX_VERTEX_NUM; ++i) {
		G->vertices[i].data = vexs[i];
		G->vertices[i].firstarc = NULL;
	}

	for (i = 0; i < arcnum; ++i) {
		int tail = arcs[3 * i];
		int head = arcs[3 * i + 1];
		int w = arcs[3 * i + 2];
		int weight = (GKind == DG || GKind == UDG) ? 1 : w;

		if (tail < 0 || tail >= vexnum || head < 0 || head >= vexnum) {
			continue;
		}

		add_arc(G, tail, head, weight);
		if (GKind == UDG || GKind == UDN) {
			add_arc(G, head, tail, weight);
		}
	}
}

void OutALGraph(ALGraph G) {
	int i;
	int weighted = (G.kind == DN || G.kind == UDN);
	printf("Vexnum=%d Arcnum=%d\n", G.vexnum, G.arcnum);
	for (i = 0; i < G.vexnum; ++i) {
		ArcNode *p = G.vertices[i].firstarc;
		printf("%c:", G.vertices[i].data);
		while (p) {
			if (weighted) {
				printf(" -> %c(%d)", G.vertices[p->adjvex].data, p->weight);
			} else {
				printf(" -> %c", G.vertices[p->adjvex].data);
			}
			p = p->nextarc;
		}
		printf("\n");
	}
}

void VerDegree(ALGraph G) {
	int i;
	if (G.kind == DG || G.kind == DN) {
		for (i = 0; i < G.vexnum; ++i) {
			int indeg = 0, outdeg = 0;
			ArcNode *p = G.vertices[i].firstarc;
			while (p) {
				outdeg++;
				p = p->nextarc;
			}
			for (int k = 0; k < G.vexnum; ++k) {
				for (p = G.vertices[k].firstarc; p; p = p->nextarc) {
					if (p->adjvex == i) {
						indeg++;
					}
				}
			}
			printf("%c: indeg=%d outdeg=%d\n", G.vertices[i].data, indeg, outdeg);
		}
	} else {
		for (i = 0; i < G.vexnum; ++i) {
			int deg = 0;
			ArcNode *p = G.vertices[i].firstarc;
			while (p) {
				deg++;
				p = p->nextarc;
			}
			printf("%c: deg=%d\n", G.vertices[i].data, deg);
		}
	}
}

static void dfs_visit(const ALGraph *G, int v, int *visited) {
	visited[v] = 1;
	printf("%c ", G->vertices[v].data);
	for (ArcNode *p = G->vertices[v].firstarc; p; p = p->nextarc) {
		if (!visited[p->adjvex]) {
			dfs_visit(G, p->adjvex, visited);
		}
	}
}

void DFSTraverse(ALGraph G) {
	int visited[MAX_VERTEX_NUM] = {0};
	for (int i = 0; i < G.vexnum; ++i) {
		if (!visited[i]) {
			dfs_visit(&G, i, visited);
		}
	}
	printf("\n");
}

void BFSTraverse(ALGraph G) {
	int visited[MAX_VERTEX_NUM] = {0};
	int queue[MAX_VERTEX_NUM];
	int front = 0, rear = 0;
	for (int i = 0; i < G.vexnum; ++i) {
		if (!visited[i]) {
			visited[i] = 1;
			printf("%c ", G.vertices[i].data);
			queue[rear++] = i;
			while (front != rear) {
				int v = queue[front++];
				for (ArcNode *p = G.vertices[v].firstarc; p; p = p->nextarc) {
					if (!visited[p->adjvex]) {
						visited[p->adjvex] = 1;
						printf("%c ", G.vertices[p->adjvex].data);
						queue[rear++] = p->adjvex;
					}
				}
			}
		}
	}
	printf("\n");
}


int main(void) {
	char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    int arcs[] = {
        0, 1, 1,
        0, 2, 1,
        1, 3, 1,
        1, 4, 1,
        2, 5, 1,
        3, 4, 1,
        4, 5, 1
    };
	ALGraph G;
    CreateALGraph(UDG, &G, 6, 7, vexs, arcs);
    OutALGraph(G);
	printf("DFS: ");
	DFSTraverse(G);
	printf("BFS: ");
	BFSTraverse(G);
}
