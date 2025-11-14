#include <stdio.h>
#include <stdlib.h>
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAXSIZE 100

typedef int ElemType;    //声明ElemType的类型
typedef int Status;      //声明函数类型名
typedef struct SqList { 

        ElemType *elem;  //顺序表数据存储空间基址 

        int length;

}SqList;                 // LIST为用户定义的线性表类型

Status InitList(SqList *L) {
    L->elem = (ElemType *)malloc(MAXSIZE * sizeof(ElemType));
    if (!L->elem) exit(OVERFLOW);
    L->length = 0;
    return OK;
}

Status ClearList(SqList *L) {
    L->length = 0;
    return OK;
}

int ListLength(SqList L) {
    return L.length;
}

Status GetElem(SqList L, int i, ElemType *e) {
    if (i < 1 || i > L.length) return ERROR;
    *e = L.elem[i - 1];
    return OK;
}

int LocateList(SqList L, ElemType e) {
    for (int i = 0; i < L.length; i++) {
        if (L.elem[i] == e) return i + 1;
    }
    return 0;
}

Status ListInsert(SqList *L, int i, ElemType e) {
    if (i < 1 || i > L->length + 1) return ERROR;
    if (L->length >= MAXSIZE) return ERROR;
    for (int j = L->length - 1; j >= i - 1; j--) {
        L->elem[j + 1] = L->elem[j];
    }
    L->elem[i - 1] = e;
    L->length++;
    return OK;
}

Status ListDelete(SqList *L, int i, ElemType *e) {
    if (i < 1 || i > L->length) return ERROR;
    *e = L->elem[i - 1];
    for (int j = i; j < L->length; j++) {
        L->elem[j - 1] = L->elem[j];
    }
    L->length--;
    return OK;
}

Status ListTraverse(SqList L) {
    for (int i = 0; i < L.length; i++) {
        printf("%d ", L.elem[i]);
    }
    printf("\n");
    return OK;
}

SqList Union(SqList La, SqList Lb) {
    SqList Lc;
    InitList(&Lc);
    ElemType e;
    for (int i = 1; i <= La.length; i++) {
        GetElem(La, i, &e);
        ListInsert(&Lc, Lc.length + 1, e);
    }
    for (int i = 1; i <= Lb.length; i++) {
        GetElem(Lb, i, &e);
        if (!LocateList(La, e)) {
            ListInsert(&Lc, Lc.length + 1, e);
        }
    }
    return Lc;
}

SqList Intersection(SqList La, SqList Lb) {
    SqList Lc;
    InitList(&Lc);
    ElemType e;
    for (int i = 1; i <= Lb.length; i++) {
        GetElem(Lb, i, &e);
        if (LocateList(La, e)) {
            ListInsert(&Lc, Lc.length + 1, e);
        }
    }
    return Lc;
}

SqList Difference(SqList La, SqList Lb) {
    SqList Lc;
    InitList(&Lc);
    ElemType e;
    for (int i = 1; i <= La.length; i++) {
        GetElem(La, i, &e);
        if (!LocateList(Lb, e)) {
            ListInsert(&Lc, Lc.length + 1, e);
        }
    }
    return Lc;
}

void MergeList(SqList La, SqList Lb, SqList *Lc) {
    InitList(Lc);
    int i = 1, j = 1;
    ElemType a, b;
    while (i <= La.length && j <= Lb.length) {
        GetElem(La, i, &a);
        GetElem(Lb, j, &b);
        if (a <= b) {
            ListInsert(Lc, Lc->length + 1, a);
            i++;
        } else {
            ListInsert(Lc, Lc->length + 1, b);
            j++;
        }
    }
    while (i <= La.length) {
        GetElem(La, i, &a);
        ListInsert(Lc, Lc->length + 1, a);
        i++;
    }
    while (j <= Lb.length) {
        GetElem(Lb, j, &b);
        ListInsert(Lc, Lc->length + 1, b);
        j++;
    }
}

void Purge(SqList *L) {
    int i = 1;
    ElemType e1, e2;
    while (i < L->length) {
        GetElem(*L, i, &e1);
        GetElem(*L, i + 1, &e2);
        if (e1 == e2) {
            ListDelete(L, i + 1, &e2);
        } else {
            i++;
        }
    }
}

int main() {
    
    // 数据准备，LA=（2,8,27,39,66,77,89）, LB=（6,18,27,59,65,77,89,120,140）
    SqList LA, LB;
    InitList(&LA);
    ListInsert(&LA, 1, 2);
    ListInsert(&LA, 2, 8);
    ListInsert(&LA, 3, 27);
    ListInsert(&LA, 4, 39);
    ListInsert(&LA, 5, 66);
    ListInsert(&LA, 6, 77);
    ListInsert(&LA, 7, 89);
    InitList(&LB);
    ListInsert(&LB, 1, 6);
    ListInsert(&LB, 2, 18);
    ListInsert(&LB, 3, 27);
    ListInsert(&LB, 4, 59);
    ListInsert(&LB, 5, 65);
    ListInsert(&LB, 6, 77);
    ListInsert(&LB, 7, 89);
    ListInsert(&LB, 8, 120);
    ListInsert(&LB, 9, 140);
    printf("LA: ");
    ListTraverse(LA);
    printf("LB: ");
    ListTraverse(LB);

    // 测试并集、交集、差集函数
    printf("Union of LA and LB: ");
    SqList Lc = Union(LA, LB);
    ListTraverse(Lc);
    printf("Intersection of LA and LB: ");
    Lc = Intersection(LA, LB);
    ListTraverse(Lc);
    printf("Difference of LA and LB: ");
    Lc = Difference(LA, LB);
    ListTraverse(Lc);

    // 测试合并函数
    printf("Merge of LA and LB: ");
    SqList Lc1;
    MergeList(LA, LB, &Lc1);
    ListTraverse(Lc1);

    // 测试去重函数
    printf("Purge duplicates in merged list: ");
    Purge(&Lc1);
    ListTraverse(Lc1);
}
