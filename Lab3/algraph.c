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


int main(void) {
	char vexs1[] = {'A', 'B', 'C', 'D'};
	int arcs1[] = {
		0, 1, 1,
		0, 2, 1,
		0, 3, 1,
		2, 1, 1,
		2, 3, 1
	};

	char vexs2[] = {'A', 'B', 'C', 'D'};
	int arcs2[] = {
		1, 0, 1,
		0, 1, 1,
		2, 1, 1,
		1, 2, 1,
		0, 2, 1,
		1, 3, 1
	};

	char vexs3[] = {'A', 'B', 'C', 'D'};
	int arcs3[] = {
		0, 1, 56,
		0, 2, 34,
		0, 3, 78,
		2, 3, 25
	};

	char vexs4[] = {'A', 'B', 'C'};
	int arcs4[] = {
		2, 0, 50,
		0, 2, 64,
		2, 1, 45
	};

	ALGraph G1, G2, G3, G4;
	CreateALGraph(UDG, &G1, 4, 5, vexs1, arcs1);
	CreateALGraph(DG, &G2, 4, 6, vexs2, arcs2);
	CreateALGraph(UDN, &G3, 4, 4, vexs3, arcs3);
	CreateALGraph(DN, &G4, 3, 3, vexs4, arcs4);

	printf("无向图G1：\n");
	OutALGraph(G1);
	VerDegree(G1);
	printf("\n有向图G2：\n");
	OutALGraph(G2);
	VerDegree(G2);
	printf("\n无向网G3：\n");
	OutALGraph(G3);
	VerDegree(G3);
	printf("\n有向网G4：\n");
	OutALGraph(G4);
	VerDegree(G4);
	return 0;
}
