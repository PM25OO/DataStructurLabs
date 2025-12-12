#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int Status;
typedef char TElemType;

typedef struct BiThrNode {
    TElemType data;
    struct BiThrNode *lchild, *rchild;
    int LTag, RTag;
} BiThrNode, *BiThrTree;

// Stack for non-recursive traversal
BiThrTree stack[100];
int top = -1;

void Push(BiThrTree p) {
    if (top < 99) stack[++top] = p;
}

BiThrTree Pop() {
    if (top >= 0) return stack[top--];
    return NULL;
}

int StackEmpty() {
    return top == -1;
}

// (1) InitBiThrTree
// 根据先序遍历的字符序列，创建一棵按线索二叉链表结构存储的尚未线索化的二叉树
Status InitBiThrTree(BiThrTree *T) {
    char ch;
    // Read character, skipping whitespace
    if (scanf(" %c", &ch) == EOF) return ERROR;
    
    if (ch == '#') {
        *T = NULL;
    } else {
        *T = (BiThrNode *)malloc(sizeof(BiThrNode));
        if (!*T) exit(OVERFLOW);
        (*T)->data = ch;
        
        InitBiThrTree(&(*T)->lchild);
        if ((*T)->lchild) (*T)->LTag = 0; else (*T)->LTag = 1;
        
        InitBiThrTree(&(*T)->rchild);
        if ((*T)->rchild) (*T)->RTag = 0; else (*T)->RTag = 1;
    }
    return OK;
}

// Helper to print node info
void Visit(BiThrTree p) {
    char ldata = '^', rdata = '^';
    
    if (p->lchild) {
        ldata = p->lchild->data;
    }
    
    if (p->rchild) {
        rdata = p->rchild->data;
    }

    printf("| %d | %c | %c | %c | %d |\n", p->LTag, ldata, p->data, rdata, p->RTag);
}

// (2) InOrderThreading
// 对InitBiThrTree函数创建的二叉树T按中序遍历进行线索化
Status InOrderThreading(BiThrTree *Thrt, BiThrTree T) {
    *Thrt = (BiThrNode *)malloc(sizeof(BiThrNode));
    if (!*Thrt) return ERROR;
    (*Thrt)->LTag = 0;
    (*Thrt)->RTag = 1;
    (*Thrt)->rchild = *Thrt;
    
    if (!T) {
        (*Thrt)->lchild = *Thrt;
        (*Thrt)->LTag = 0; 
    } else {
        (*Thrt)->lchild = T;
        (*Thrt)->data = '@'; 
        BiThrTree pre = *Thrt;
        BiThrTree p = T;
        
        // Non-recursive Inorder Traversal for Threading
        top = -1; 
        while (p || !StackEmpty()) {
            if (p) {
                Push(p);
                if (p->LTag == 0) p = p->lchild;
                else p = NULL;
            } else {
                p = Pop();
                
                if (!p->lchild) { 
                    p->LTag = 1; 
                    p->lchild = pre;
                }
                
                if (!pre->rchild) { 
                    pre->RTag = 1;
                    pre->rchild = p;
                }
                
                pre = p;
                
                if (p->RTag == 0) p = p->rchild;
                else p = NULL;
            }
        }
        
        pre->rchild = *Thrt;
        pre->RTag = 1;
        (*Thrt)->rchild = pre;
    }
    return OK;
}

// (3) InOrderTraverse (Recursive)
// 按孩子中序遍历线索二叉树（递归）
void InOrderTraverse(BiThrTree T) {
    if (T) {
        if (T->LTag == 0) InOrderTraverse(T->lchild);
        Visit(T);
        if (T->RTag == 0) InOrderTraverse(T->rchild);
    }
}

// (4) InOrderTraverse_Thr (Non-recursive)
// 按线索和孩子中序遍历线索二叉树（非递归）
void InOrderTraverse_Thr(BiThrTree T) {
    BiThrTree p = T->lchild; 
    while (p != T) {
        while (p->LTag == 0) p = p->lchild;
        Visit(p);
        while (p->RTag == 1 && p->rchild != T) {
            p = p->rchild;
            Visit(p);
        }
        p = p->rchild;
    }
}

int main() {
    BiThrTree T, Thrt;
    printf("输入线索二叉树：\n"); // ABD#F###CE###
    InitBiThrTree(&T);
    
    printf("\n中序遍历结果:\n| LTag | 左指针所指元素 | 本结点的值 | 右指针所指元素 | RTag |\n");
    InOrderTraverse(T);

    printf("\n正在进行线索化...\n");
    InOrderThreading(&Thrt, T);

    printf("\n线索化结果:\n| LTag | 左指针所指元素 | 本结点的值 | 右指针所指元素 | RTag |\n");
    InOrderTraverse_Thr(Thrt);
    
    return 0;
}
