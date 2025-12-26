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

typedef struct {
	int adjvex;
	int lowcost;
} CloseEdge;

typedef struct {
	int tail;
	int head;
	int weight;
} Edge;

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

static int locate_vex(const MGraph *G, VertexType v) {
	for (int i = 0; i < G->vexnum; ++i) {
		if (G->vexs[i] == v) {
			return i;
		}
	}
	return -1;
}

static void print_closedge(CloseEdge *closedge, int count, const MGraph *G) {
	printf("closedge: ");
	for (int i = 0; i < count; ++i) {
		if (closedge[i].adjvex == -1) {
			printf("[%c, -, -] ", G->vexs[i]);
		} else {
			printf("[%c,%c,%d] ", G->vexs[i], G->vexs[closedge[i].adjvex], closedge[i].lowcost);
		}
	}
	printf("\n");
}

void MiniSpanTree_Prim(MGraph G, VertexType u) {
	int start = locate_vex(&G, u);
	if (start == -1) {
		printf("start vertex not found\n");
		return;
	}
	CloseEdge closedge[MAX_VERTEX_NUM];
	int visited[MAX_VERTEX_NUM] = {0};
	visited[start] = 1;

	for (int i = 0; i < G.vexnum; ++i) {
		if (i != start) {
			closedge[i].adjvex = start;
			closedge[i].lowcost = G.arcs[start][i].adj;
		} else {
			closedge[i].adjvex = -1;
			closedge[i].lowcost = 0;
		}
	}
	print_closedge(closedge, G.vexnum, &G);

	for (int k = 1; k < G.vexnum; ++k) {
		int min = INFINITY;
		int mv = -1;
		for (int j = 0; j < G.vexnum; ++j) {
			if (!visited[j] && closedge[j].lowcost < min) {
				min = closedge[j].lowcost;
				mv = j;
			}
		}
		if (mv == -1) {
			break;
		}
		visited[mv] = 1;
		printf("(%c,%c,%d)\n", G.vexs[closedge[mv].adjvex], G.vexs[mv], closedge[mv].lowcost);
		for (int j = 0; j < G.vexnum; ++j) {
			if (!visited[j] && G.arcs[mv][j].adj < closedge[j].lowcost) {
				closedge[j].lowcost = G.arcs[mv][j].adj;
				closedge[j].adjvex = mv;
			}
		}
		print_closedge(closedge, G.vexnum, &G);
	}
}

static int cmp_edge(const void *a, const void *b) {
	const Edge *ea = (const Edge *)a;
	const Edge *eb = (const Edge *)b;
	return ea->weight - eb->weight;
}

void MiniSpanTree_Kruskal(MGraph G) {
	Edge edges[MAX_VERTEX_NUM * MAX_VERTEX_NUM];
	int ecount = 0;
	for (int i = 0; i < G.vexnum; ++i) {
		for (int j = i + 1; j < G.vexnum; ++j) {
			if (G.arcs[i][j].adj != INFINITY) {
				edges[ecount].tail = i;
				edges[ecount].head = j;
				edges[ecount].weight = G.arcs[i][j].adj;
				ecount++;
			}
		}
	}
	qsort(edges, ecount, sizeof(Edge), cmp_edge);
	printf("sorted edges: ");
	for (int i = 0; i < ecount; ++i) {
		printf("(%c,%c,%d) ", G.vexs[edges[i].tail], G.vexs[edges[i].head], edges[i].weight);
	}
	printf("\n");

	int cnvx[MAX_VERTEX_NUM];
	for (int i = 0; i < G.vexnum; ++i) {
		cnvx[i] = i;
	}

	for (int i = 0; i < ecount; ++i) {
		int x = edges[i].tail;
		int y = edges[i].head;
		if (cnvx[x] != cnvx[y]) {
			printf("(%c,%c,%d)\n", G.vexs[x], G.vexs[y], edges[i].weight);
			int old = cnvx[y];
			int newc = cnvx[x];
			for (int k = 0; k < G.vexnum; ++k) {
				if (cnvx[k] == old) {
					cnvx[k] = newc;
				}
			}
			printf("cnvx: ");
			for (int k = 0; k < G.vexnum; ++k) {
				printf("[%c,%d] ", G.vexs[k], cnvx[k]);
			}
			printf("\n");
		}
	}
}


int main(void) {
	char vexs1[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    int arcs1[] = {
        0, 1, 10,
        0, 2, 12,
        0, 4, 15,
        1, 2, 7,
        1, 3, 5,
        1, 5, 6,
        2, 4, 12,
        2, 5, 8,
        3, 5, 6,
        4, 5, 10
    };
    MGraph G1;
    CreateMGraph(UDN, &G1, 6, 10, vexs1, arcs1);
    OutMGraph(G1);

	printf("\nPrim MST from A:\n");
	MiniSpanTree_Prim(G1, 'A');
	printf("\nKruskal MST:\n");
	MiniSpanTree_Kruskal(G1);
    
}

