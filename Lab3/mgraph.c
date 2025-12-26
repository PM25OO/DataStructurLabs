#include <stdio.h>
#include <stdlib.h>

#define INFINITY 32767
#define MAX_VERTEX_NUM 20

typedef int Status;

typedef enum { DG, DN, UDG, UDN } GraphKind;
typedef int VRType;
typedef char InfoType;

typedef struct ArcCell {
	VRType adj;    /* 1/0 for unweighted, weight for weighted graphs */
	InfoType *info;
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef char VertexType;

typedef struct {
	VertexType vexs[MAX_VERTEX_NUM];
	AdjMatrix arcs;
	int vexnum, arcnum;
	GraphKind kind;
} MGraph;

static int is_edge_present(const MGraph *G, int i, int j) {
	if (G->kind == DG || G->kind == UDG) {
		return G->arcs[i][j].adj != 0;
	}
	return G->arcs[i][j].adj != INFINITY;
}

void CreateMGraph(GraphKind GKind, MGraph *G, int vexnum, int arcnum, char *vexs, int *arcs) {
	int i, j;
	G->kind = GKind;
	G->vexnum = vexnum;
	G->arcnum = arcnum;

	for (i = 0; i < vexnum && i < MAX_VERTEX_NUM; ++i) {
		G->vexs[i] = vexs[i];
	}

	for (i = 0; i < vexnum; ++i) {
		for (j = 0; j < vexnum; ++j) {
			if (GKind == DG || GKind == UDG) {
				G->arcs[i][j].adj = 0;
			} else {
				G->arcs[i][j].adj = INFINITY;
			}
			G->arcs[i][j].info = NULL;
		}
	}

	for (i = 0; i < arcnum; ++i) {
		int tail = arcs[3 * i];
		int head = arcs[3 * i + 1];
		int weight = arcs[3 * i + 2];

		if (tail < 0 || tail >= vexnum || head < 0 || head >= vexnum) {
			continue;
		}

		if (GKind == DG) {
			G->arcs[tail][head].adj = 1;
		} else if (GKind == UDG) {
			G->arcs[tail][head].adj = 1;
			G->arcs[head][tail].adj = 1;
		} else if (GKind == DN) {
			G->arcs[tail][head].adj = weight;
		} else if (GKind == UDN) {
			G->arcs[tail][head].adj = weight;
			G->arcs[head][tail].adj = weight;
		}
	}
}

void OutMGraph(MGraph G) {
	int i, j;
	printf("Vexnum=%d Arcnum=%d\n", G.vexnum, G.arcnum);
	printf("Vexs: ");
	for (i = 0; i < G.vexnum; ++i) {
		printf("%c ", G.vexs[i]);
	}
	printf("\nAdjacency Matrix:\n");

	for (i = 0; i < G.vexnum; ++i) {
		for (j = 0; j < G.vexnum; ++j) {
			if (G.kind == DG || G.kind == UDG) {
				printf("%d ", G.arcs[i][j].adj);
			} else {
				if (G.arcs[i][j].adj == INFINITY) {
					printf("INF ");
				} else {
					printf("%d ", G.arcs[i][j].adj);
				}
			}
		}
		printf("\n");
	}
}

void VerDegree(MGraph G) {
	int i, j;
	if (G.kind == DG || G.kind == DN) {
		for (i = 0; i < G.vexnum; ++i) {
			int indeg = 0, outdeg = 0;
			for (j = 0; j < G.vexnum; ++j) {
				if (is_edge_present(&G, i, j)) {
					outdeg++;
				}
				if (is_edge_present(&G, j, i)) {
					indeg++;
				}
			}
			printf("%c: indeg=%d outdeg=%d\n", G.vexs[i], indeg, outdeg);
		}
	} else {
		for (i = 0; i < G.vexnum; ++i) {
			int deg = 0;
			for (j = 0; j < G.vexnum; ++j) {
				if (is_edge_present(&G, i, j)) {
					deg++;
				}
			}
			printf("%c: deg=%d\n", G.vexs[i], deg);
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

    MGraph G1, G2, G3, G4;
    CreateMGraph(UDG, &G1, 4, 5, vexs1, arcs1);
    CreateMGraph(DG, &G2, 4, 6, vexs2, arcs2);
    CreateMGraph(UDN, &G3, 4, 4, vexs3, arcs3);
    CreateMGraph(DN, &G4, 3, 3, vexs4, arcs4);

    printf("无向图G1：\n");
    OutMGraph(G1);
    VerDegree(G1);
    printf("\n有向图G2：\n");
    OutMGraph(G2);
    VerDegree(G2);
    printf("\n无向网G3：\n");
    OutMGraph(G3);
    VerDegree(G3);
    printf("\n有向网G4：\n");
    OutMGraph(G4);
    VerDegree(G4);
}

