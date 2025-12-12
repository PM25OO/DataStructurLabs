#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {  // 定义二叉链表结点类型 
    int weight;
    int parent, lchild, rchild; 
} HTNode, *HTree;           // 动态分配数组存储赫夫曼树

typedef char **HTCode;  // 动态分配字符串数组存储赫夫曼编码表

// 辅助函数：在HT[0..k-1]中选择两个双亲域为-1且权值最小的结点
void Select(HTree HT, int k, int *s1, int *s2) {
    int min1 = 0x7FFFFFFF, min2 = 0x7FFFFFFF;
    *s1 = -1;
    *s2 = -1;

    for (int i = 0; i < k; i++) {
        if (HT[i].parent == -1) {
            if (HT[i].weight < min1) {
                min2 = min1;
                *s2 = *s1;
                min1 = HT[i].weight;
                *s1 = i;
            } else if (HT[i].weight < min2) {
                min2 = HT[i].weight;
                *s2 = i;
            }
        }
    }
}

// (1) InitHTree
// 初始化赫夫曼树
void InitHTree(HTree *HT, int *w, int n) {
    int m = 2 * n - 1;
    *HT = (HTree)malloc(m * sizeof(HTNode));
    
    // 初始化前n个元素（叶子结点）
    for (int i = 0; i < n; i++) {
        (*HT)[i].weight = w[i];
        (*HT)[i].parent = -1;
        (*HT)[i].lchild = -1;
        (*HT)[i].rchild = -1;
    }
    
    // 初始化后m-n个元素
    for (int i = n; i < m; i++) {
        (*HT)[i].weight = 0;
        (*HT)[i].parent = -1;
        (*HT)[i].lchild = -1;
        (*HT)[i].rchild = -1;
    }
}

// (2) CreateHTree
// 构造赫夫曼树HT
void CreateHTree(HTree *HT, int n) {
    int m = 2 * n - 1;
    int s1, s2;
    
    // 依次构造n-1个非叶子结点
    for (int i = n; i < m; i++) {
        Select(*HT, i, &s1, &s2);
        
        (*HT)[s1].parent = i;
        (*HT)[s2].parent = i;
        
        (*HT)[i].lchild = s1;
        (*HT)[i].rchild = s2;
        (*HT)[i].weight = (*HT)[s1].weight + (*HT)[s2].weight;
    }
}

// (3) HTCoding
// 生成赫夫曼编码HC
void HTCoding(HTree HT, HTCode *HC, int n) {
    *HC = (HTCode)malloc(n * sizeof(char *));
    char *cd = (char *)malloc(n * sizeof(char)); // 临时存放编码
    cd[n - 1] = '\0';
    
    for (int i = 0; i < n; i++) {
        int start = n - 1;
        int c = i;
        int f = HT[i].parent;
        
        // 从叶子到根逆向求编码
        while (f != -1) {
            if (HT[f].lchild == c)
                cd[--start] = '0';
            else
                cd[--start] = '1';
            c = f;
            f = HT[f].parent;
        }
        
        (*HC)[i] = (char *)malloc((n - start) * sizeof(char));
        strcpy((*HC)[i], &cd[start]);
        printf("Node %d (weight %d): %s\n", i, HT[i].weight, (*HC)[i]);
    }
    free(cd);
}

// 辅助函数：打印赫夫曼树内容
void PrintHT(HTree HT, int n) {
    int m = 2 * n - 1;
    printf("Index\tWeight\tParent\tLChild\tRChild\n");
    for (int i = 0; i < m; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", i, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild);
    }
}

int main() {
    int w[] = {5, 29, 7, 8, 14, 23, 3, 11};
    int n = 8;
    HTree HT;
    HTCode HC;

    printf("\n(1) 权重w={5, 29, 7, 8, 14, 23, 3, 11}， 初始化HTree，并打印：\n\n");
    InitHTree(&HT, w, n);
    PrintHT(HT, n);

    printf("\n(2) 构造赫夫曼树：\n\n");
    CreateHTree(&HT, n);
    PrintHT(HT, n);

    printf("\n(3) 输出赫夫曼树HT中各叶子结点的赫夫曼编码：\n\n");
    HTCoding(HT, &HC, n);

    // 释放内存
    free(HT);
    for(int i=0; i<n; i++) free(HC[i]);
    free(HC);

    return 0;
}
