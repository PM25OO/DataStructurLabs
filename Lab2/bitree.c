# include <stdio.h>
# include <stdlib.h>

typedef int Status;            //声明函数类型名
typedef  char TElemType;       //声明结点元素值得类型

typedef  struct BiTNode {      //定义二叉链表结点类型 

    TElemType  		data;

    struct BiTNode  *lchild, *rchild; 	//指向左右孩子结点的指针

} BiTNode, *BiTree;

BiTree CreatBiTree(BiTNode *T) {
    TElemType ch;
    if (scanf(" %c", &ch) != 1) {
        return NULL;
    }
    if (ch == '#') {
        T = NULL;
    } else {
        T = (BiTNode *) malloc(sizeof(BiTNode));
        if (!T) {
            exit(-1);
        }
        T->data = ch;
        T->lchild = CreatBiTree(T->lchild);
        T->rchild = CreatBiTree(T->rchild);
    }
    return T;
}

void PreOrderTraverse(BiTree T) {
    if (T) {
        printf("%c ", T->data);
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
}

void InOrderTraverse(BiTree T) {
    if (T) {
        InOrderTraverse(T->lchild);
        printf("%c ", T->data);
        InOrderTraverse(T->rchild);
    }
}

void PostOrderTraverse(BiTree T) {
    if (T) {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        printf("%c ", T->data);
    }
}

void traverseHelper(BiTree T, int *zero, int *one, int *two) {
    if (T) {
        int i = 0;
        if (T->lchild != NULL) i += 1;
        if (T->rchild != NULL) i += 1;
        *one += (i == 1 ? 1 : 0);
        *two += (i == 2 ? 1 : 0);
        *zero += (i == 0 ? 1 : 0);
        traverseHelper(T->lchild, zero, one, two);
        traverseHelper(T->rchild, zero, one, two);
    }
}

int TNodes(BiTree T, int d) {
    int zero = 0, one = 0, two = 0;
    traverseHelper(T, &zero, &one, &two);
    if (d == 0) return zero;
    if (d == 1) return one;
    if (d == 2) return two;
}

int High(BiTree T) {
    if (!T) {
        return 0;
    } else {
        int leftHeight = High(T->lchild);
        int rightHeight = High(T->rchild);
        return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    }
}

BiTree CreateBST(BiTree *T, const char *chars) {
    if (*chars == '\0') {
        *T = NULL;
        return;
    }
    *T = (BiTNode *) malloc(sizeof(BiTNode));
    if (!*T) {
        exit(-1);
    }
    (*T)->data = *chars;
    CreateBST(&((*T)->lchild), chars + 1);
    CreateBST(&((*T)->rchild), chars + 2);
}



int main() {
    BiTree T = NULL;
    printf("请输入二叉树的先序遍历序列（#表示空树）: \n");
    T = CreatBiTree(T); //测试数据：ABD#F###CE###
    printf("先序遍历结果: \n");
    PreOrderTraverse(T);
    printf("\n中序遍历结果: \n");
    InOrderTraverse(T);
    printf("\n后序遍历结果: \n");
    PostOrderTraverse(T);
    printf("\n");
    printf("度为0的结点个数: %d\n", TNodes(T, 0));
    printf("度为1的结点个数: %d\n", TNodes(T, 1));
    printf("度为2的结点个数: %d\n", TNodes(T, 2));

    BiTree T1 = CreateBST(T1, "DBFCAEG");
    
    return 0;
}

// 按前序遍历创建几个二叉树输入示例，至少有5个节点，并画出树形图
// 示例1: A B D # # E # # C # #
// 示例2: F G # H # # I J # # K # ##
// 示例3: M N O # # P # # Q R # # S # ##
// 示例4: X Y # Z # # W # # ##
// 示例5: 1 2 4 # # 5 # # 3 # ##
