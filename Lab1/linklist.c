#include <stdio.h>
#include <stdlib.h>
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int ElemType;
typedef int Status;
typedef struct LNode { 
    ElemType data;  
    struct LNode *next;
}LNode, *LinkList;

Status InitList(LinkList *L) {
    *L = (LinkList)malloc(sizeof(LNode));
    if (!(*L)) exit(OVERFLOW);
    (*L)->next = NULL;
    return OK;
}

Status ClearList(LinkList L) {
    LinkList p, q;
    p = L->next;
    while (p) {
        q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;
    return OK;
}

int ListLength(LinkList L) {
    int length = 0;
    LinkList p = L->next;
    while (p) {
        length++;
        p = p->next;
    }
    return length;
}

Status GetElem(LinkList L, int i, ElemType *e) {
    LinkList p = L->next;
    int j = 1;
    while (p && j < i) {
        p = p->next;
        j++;
    }
    if (!p || j > i) return ERROR;
    *e = p->data;
    return OK;
}

int LocateList(LinkList L, ElemType e) {
    LinkList p = L->next;
    int i = 1;
    while (p) {
        if (p->data == e) return i;
        p = p->next;
        i++;
    }
    return 0;
}

Status ListInsert(LinkList L, int i, ElemType e) {
    LinkList p = L;
    int j = 0;
    while (p && j < i - 1) {
        p = p->next;
        j++;
    }
    if (!p || j > i - 1) return ERROR;
    LinkList s = (LinkList)malloc(sizeof(LNode));
    if (!s) exit(OVERFLOW);
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

Status ListDelete(LinkList L, int i, ElemType *e) {
    LinkList p = L;
    int j = 0;
    while (p->next && j < i - 1) {
        p = p->next;
        j++;
    }
    if (!(p->next) || j > i - 1) return ERROR;
    LinkList q = p->next;
    *e = q->data;
    p->next = q->next;
    free(q);
    return OK;
}

Status ListTraverse(LinkList L) {
    LinkList p = L->next;
    while (p) {
        printf("%d -> ", p->data);
        p = p->next;
    }
    printf("^\n");
    return OK;
}

LinkList Union(LinkList La, LinkList Lb) {
    LinkList Lc;
    InitList(&Lc);
    ElemType e;
    int len = ListLength(La);
    for (int i = 1; i <= len; i++) {
        GetElem(La, i, &e);
        ListInsert(Lc, ListLength(Lc) + 1, e);
    }
    len = ListLength(Lb);
    for (int i = 1; i <= len; i++) {
        GetElem(Lb, i, &e);
        if (!LocateList(La, e)) {
            ListInsert(Lc, ListLength(Lc) + 1, e);
        }
    }
    return Lc;
}

LinkList Intersection(LinkList La, LinkList Lb) {
    LinkList Lc;
    InitList(&Lc);
    ElemType e;
    int len = ListLength(Lb);
    for (int i = 1; i <= len; i++) {
        GetElem(Lb, i, &e);
        if (LocateList(La, e)) {
            ListInsert(Lc, ListLength(Lc) + 1, e);
        }
    }
    return Lc;
}

LinkList Difference(LinkList La, LinkList Lb) {
    LinkList Lc;
    InitList(&Lc);
    ElemType e;
    int len = ListLength(La);
    for (int i = 1; i <= len; i++) {
        GetElem(La, i, &e);
        if (!LocateList(Lb, e)) {
            ListInsert(Lc, ListLength(Lc) + 1, e);
        }
    }
    return Lc;
}

void MergeList(LinkList La, LinkList Lb, LinkList *Lc) {
    InitList(Lc);
    int i = 1, j = 1;
    int lenA = ListLength(La), lenB = ListLength(Lb);
    ElemType a, b;
    while (i <= lenA && j <= lenB) {
        GetElem(La, i, &a);
        GetElem(Lb, j, &b);
        if (a <= b) {
            ListInsert(*Lc, ListLength(*Lc) + 1, a);
            i++;
        } else {
            ListInsert(*Lc, ListLength(*Lc) + 1, b);
            j++;
        }
    }
    while (i <= lenA) {
        GetElem(La, i, &a);
        ListInsert(*Lc, ListLength(*Lc) + 1, a);
        i++;
    }
    while (j <= lenB) {
        GetElem(Lb, j, &b);
        ListInsert(*Lc, ListLength(*Lc) + 1, b);
        j++;
    }
}

void Purge(LinkList L) {
    LinkList p = L->next;
    while (p && p->next) {
        if (p->data == p->next->data) {
            LinkList q = p->next;
            p->next = q->next;
            free(q);
        } else {
            p = p->next;
        }
    }
}

int main() {
    // LA=2 → 8 → 27 → 39 → 66 → 77 → 89 → ∧
    // LB=6 → 18 → 27 → 59 → 65 → 77 → 89 → 120 → 140 → ∧
    LinkList LA, LB;
    InitList(&LA);
    ListInsert(LA, 1, 2);
    ListInsert(LA, 2, 8);
    ListInsert(LA, 3, 27);
    ListInsert(LA, 4, 39);
    ListInsert(LA, 5, 66);
    ListInsert(LA, 6, 77);
    ListInsert(LA, 7, 89);
    InitList(&LB);
    ListInsert(LB, 1, 6);
    ListInsert(LB, 2, 18);
    ListInsert(LB, 3, 27);
    ListInsert(LB, 4, 59);
    ListInsert(LB, 5, 65);
    ListInsert(LB, 6, 77);
    ListInsert(LB, 7, 89);
    ListInsert(LB, 8, 120);
    ListInsert(LB, 9, 140);
    printf("LA: ");
    ListTraverse(LA);
    printf("LB: ");
    ListTraverse(LB);

    // 测试并集、交集、差集函数
    printf("Union of LA and LB: ");
    LinkList Lc = Union(LA, LB);
    ListTraverse(Lc);
    printf("Intersection of LA and LB: ");
    Lc = Intersection(LA, LB);
    ListTraverse(Lc);
    printf("Difference of LA and LB: ");
    Lc = Difference(LA, LB);
    ListTraverse(Lc);

    // 测试合并函数
    printf("Merge of LA and LB: ");
    LinkList Lc1;
    MergeList(LA, LB, &Lc1);
    ListTraverse(Lc1);

    // 测试去重函数
    printf("Purge duplicates in merged list: ");
    Purge(Lc1);
    ListTraverse(Lc1);
}
