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

static int compute_indegree(const ALGraph *G, int indegree[]) {
	for (int i = 0; i < G->vexnum; ++i) {
		indegree[i] = 0;
	}
	for (int i = 0; i < G->vexnum; ++i) {
		for (ArcNode *p = G->vertices[i].firstarc; p; p = p->nextarc) {
			indegree[p->adjvex]++;
		}
	}
	return 0;
}

int CriticalPath(ALGraph *G) {
	if (G->kind != DG && G->kind != DN) {
		printf("Critical path only defined for directed AOE nets.\n");
		return 0;
	}

	int indegree[MAX_VERTEX_NUM];
	int ve[MAX_VERTEX_NUM];
	int vl[MAX_VERTEX_NUM];
	int topo[MAX_VERTEX_NUM];
	int stack[MAX_VERTEX_NUM];
	int top = -1;
	int count = 0;

	compute_indegree(G, indegree);
	for (int i = 0; i < G->vexnum; ++i) {
		ve[i] = 0;
		if (indegree[i] == 0) {
			stack[++top] = i;
		}
	}

	while (top >= 0) {
		int v = stack[top--];
		topo[count++] = v;
		for (ArcNode *p = G->vertices[v].firstarc; p; p = p->nextarc) {
			int k = p->adjvex;
			if (ve[v] + p->weight > ve[k]) {
				ve[k] = ve[v] + p->weight;
			}
			if (--indegree[k] == 0) {
				stack[++top] = k;
			}
		}
	}

	if (count < G->vexnum) {
		printf("Graph contains a cycle, no critical path.\n");
		return 0;
	}

	printf("Topological order: ");
	for (int i = 0; i < count; ++i) {
		printf("%c%s", G->vertices[topo[i]].data, (i + 1 == count) ? "\n" : " ");
	}

	printf("Earliest event times (ve):\n");
	for (int i = 0; i < G->vexnum; ++i) {
		printf("%c:%d%s", G->vertices[i].data, ve[i], (i + 1 == G->vexnum) ? "\n" : "  ");
	}

	int max_ve = ve[topo[count - 1]];
	for (int i = 0; i < G->vexnum; ++i) {
		vl[i] = max_ve;
	}

	for (int idx = count - 1; idx >= 0; --idx) {
		int v = topo[idx];
		for (ArcNode *p = G->vertices[v].firstarc; p; p = p->nextarc) {
			int k = p->adjvex;
			if (vl[k] - p->weight < vl[v]) {
				vl[v] = vl[k] - p->weight;
			}
		}
	}

	printf("Latest event times (vl):\n");
	for (int i = 0; i < G->vexnum; ++i) {
		printf("%c:%d%s", G->vertices[i].data, vl[i], (i + 1 == G->vexnum) ? "\n" : "  ");
	}

	printf("Activities detail (e=earliest start, l=latest start, slack=l-e):\n");

	printf("Critical activities:\n");
	for (int v = 0; v < G->vexnum; ++v) {
		for (ArcNode *p = G->vertices[v].firstarc; p; p = p->nextarc) {
			int k = p->adjvex;
			int e = ve[v];
			int l = vl[k] - p->weight;
			printf("%c -> %c  e=%d  l=%d  slack=%d%s\n",
			    G->vertices[v].data,
			    G->vertices[k].data,
			    e,
			    l,
			    l - e,
			    (e == l) ? "  *critical" : "");
			if (e == l) {
				printf("  (duration=%d)\n", p->weight);
			}
		}
	}

	return 1;
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
	CriticalPath(&G);

}
