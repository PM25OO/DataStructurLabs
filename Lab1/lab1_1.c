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

void Union(SqList *La, SqList Lb) {
    ElemType e;
    for (int i = 1; i <= Lb.length; i++) {
        GetElem(Lb, i, &e);
        if (!LocateList(*La, e)) {
            ListInsert(La, La->length + 1, e);
        }
    }
}

void Intersection(SqList *La, SqList Lb) {
    ElemType e;
    for (int i = 1; i <= La->length; ) {
        GetElem(*La, i, &e);
        if (!LocateList(Lb, e)) {
            ListDelete(La, i, &e);
        } else {
            i++;
        }
    }
}

void Difference(SqList *La, SqList Lb) {
    ElemType e;
    for (int i = 1; i <= Lb.length; i++) {
        GetElem(Lb, i, &e);
        int pos;
        while ((pos = LocateList(*La, e))) {
            ListDelete(La, pos, &e);
        }
    }
}
