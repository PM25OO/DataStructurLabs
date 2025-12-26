#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 20
#define INF 0x3f3f3f3f

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

static int locate_vex(const ALGraph *G, VertexType v) {
	for (int i = 0; i < G->vexnum; ++i) {
		if (G->vertices[i].data == v) return i;
	}
	return -1;
}

void Dijkstra(const ALGraph *G, VertexType src, VertexType dst) {
	int s = locate_vex(G, src);
	int t = locate_vex(G, dst);
	if (s == -1 || t == -1) {
		printf("vertex not found\n");
		return;
	}
	int dist[MAX_VERTEX_NUM];
	int prev[MAX_VERTEX_NUM];
	int visited[MAX_VERTEX_NUM] = {0};
	for (int i = 0; i < G->vexnum; ++i) {
		dist[i] = INF;
		prev[i] = -1;
	}
	dist[s] = 0;

	for (int iter = 0; iter < G->vexnum; ++iter) {
		int u = -1;
		int mind = INF;
		for (int i = 0; i < G->vexnum; ++i) {
			if (!visited[i] && dist[i] < mind) {
				mind = dist[i];
				u = i;
			}
		}
		if (u == -1) break;
		visited[u] = 1;
		for (ArcNode *p = G->vertices[u].firstarc; p; p = p->nextarc) {
			int v = p->adjvex;
			if (!visited[v] && dist[u] + p->weight < dist[v]) {
				dist[v] = dist[u] + p->weight;
				prev[v] = u;
			}
		}
	}

	if (dist[t] == INF) {
		printf("no path from %c to %c\n", src, dst);
		return;
	}

	int path[MAX_VERTEX_NUM];
	int cnt = 0;
	for (int v = t; v != -1; v = prev[v]) {
		path[cnt++] = v;
	}
	printf("shortest path %c->%c (len=%d): ", src, dst, dist[t]);
	for (int i = cnt - 1; i >= 0; --i) {
		printf("%c", G->vertices[path[i]].data);
		if (i) printf(" -> ");
	}
	printf("\n");
}


int main(void) {
	char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
	int arcs[] = {
		0, 1, 8,
		0, 2, 6,
		0, 4, 7,
		1, 3, 3,
		2, 3, 10,
		2, 6, 9,
		4, 6, 9,
		4, 7, 13,
		3, 5, 4,
		3, 8, 19,
        6, 7, 2,
        6, 8, 8,
		7, 8, 6,
		5, 9, 14,
		8, 9, 10
	};
	ALGraph G;
	CreateALGraph(DN, &G, 10, 16, vexs, arcs);
	printf("Graph (figure network):\n");
	OutALGraph(G);
	Dijkstra(&G, 'A', 'J');
}
