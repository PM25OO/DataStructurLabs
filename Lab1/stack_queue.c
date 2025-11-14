#include <stdio.h>
#include <stdlib.h>
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int Status;

#define STACK_INIT_SIZE 10
#define STACKINCREMENT 5

typedef int SElemType; // 声明SElemType的类型

typedef struct
{

    SElemType *base; // 顺序栈数据存储空间基址

    SElemType *top; // 顺序栈栈顶指针

    int stacksize;

} SqStack; // SqStack为用户定义的顺序栈类型

#define MAXQSIZE 10
#define QUEUE_MAXQSIZE (MAXQSIZE + 1) // 队列的最大长度

typedef int QElemType; // 声明SElemType的类型

typedef struct
{

    QElemType *base; // 初始化队列时动态分配存储空间的地址(数组名)

    int front; // 头元素的下标变量

    int rear; // 尾元素的下标变量

} SqQueue; // SqQueue为用户定义的循环队列类型

Status InitStack(SqStack *S)
{
    S->base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!S->base)
        exit(OVERFLOW);
    S->top = S->base;
    S->stacksize = STACK_INIT_SIZE;
    return OK;
}

Status ClearStack(SqStack *S)
{
    S->top = S->base;
    return OK;
}

int StackLength(SqStack S)
{
    return S.top - S.base;
}

Status GetTop(SqStack S, SElemType *e)
{
    if (S.top == S.base)
        return ERROR;
    *e = *(S.top - 1);
    return OK;
}

Status Push(SqStack *S, SElemType e)
{
    if (S->top - S->base >= S->stacksize)
    {
        S->base = (SElemType *)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!S->base)
            exit(OVERFLOW);
        S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *(S->top) = e;
    S->top++;
    return OK;
}

Status Pop(SqStack *S, SElemType *e)
{
    if (S->top == S->base)
        return ERROR;
    S->top--;
    *e = *(S->top);
    return OK;
}

Status InitQueue(SqQueue *Q)
{
    Q->base = (QElemType *)malloc(QUEUE_MAXQSIZE * sizeof(QElemType));
    if (!Q->base)
        exit(OVERFLOW);
    Q->front = 0;
    Q->rear = 0;
    return OK;
}

Status ClearQueue(SqQueue *Q)
{
    Q->front = 0;
    Q->rear = 0;
    return OK;
}

int QueueLength(SqQueue Q)
{
    return (Q.rear - Q.front + QUEUE_MAXQSIZE) % QUEUE_MAXQSIZE;
}

Status EnQueue(SqQueue *Q, QElemType e)
{
    if ((Q->rear + 1) % QUEUE_MAXQSIZE == Q->front)
        return ERROR;
    Q->base[Q->rear] = e;
    Q->rear = (Q->rear + 1) % QUEUE_MAXQSIZE;
    return OK;
}

Status DeQueue(SqQueue *Q, QElemType *e)
{
    if (Q->front == Q->rear)
        return ERROR;
    *e = Q->base[Q->front];
    Q->front = (Q->front + 1) % QUEUE_MAXQSIZE;
    return OK;
}

Status QueueEmpty(SqQueue Q)
{
    if (Q.front == Q.rear)
        return TRUE;
    else
        return FALSE;
}

Status QueueFull(SqQueue Q)
{
    if ((Q.rear + 1) % QUEUE_MAXQSIZE == Q.front)
        return TRUE;
    else
        return FALSE;
}

Status Conversion(int m, int n)
{
    if (m == 0)
    {
        printf("0\n");
        return OK;
    }
    SqStack S;
    InitStack(&S);
    int remainder;
    if (n != 2 && n != 8)
        return ERROR;
    while (m)
    {
        remainder = m % n;
        Push(&S, remainder);
        m = m / n;
    }
    printf("The converted number is: ");
    while (StackLength(S) > 0)
    {
        Pop(&S, &remainder);
        printf("%d", remainder);
    }
    printf("\n");
    return OK;
}

Status QueueReverse(SqQueue *Q)
{
    SqStack S;
    InitStack(&S);
    QElemType e;

    while (!QueueEmpty(*Q))
    {
        DeQueue(Q, &e);
        Push(&S, e);
    }

    while (StackLength(S) > 0)
    {
        Pop(&S, &e);
        EnQueue(Q, e);
    }

    return OK;
}

int main()
{

    // 数制转换
    int number, base;
    printf("Enter a decimal number and the base to convert to (2 or 8): ");
    scanf("%d %d", &number, &base);
    if (Conversion(number, base) == ERROR)
    {
        printf("Error: Base must be 2 or 8.\n");
    }

    // 循环队列操作
    // 1>连续入队9个整数后，显示队列Q的front和rear的值
    SqQueue Q;
    InitQueue(&Q);
    printf("连续入队9个整数:\n");
    for (int i = 1; i <= 9; i++)
    {
        QElemType e;
        scanf("%d", &e);
        EnQueue(&Q, e);
    }
    printf("Queue front: %d, rear: %d\n", Q.front, Q.rear);

    // 2>若再入队1个整数，显示此时队列的状态信息
    printf("再入队1个整数: ");
    QElemType e;
    scanf("%d", &e);
    if (EnQueue(&Q, e) == ERROR)
    {
        printf("Error: Queue is full.\n");
    }
    else
    {
        printf("Enqueued: %d\n", e);
        printf("Queue front: %d, rear: %d\n", Q.front, Q.rear);
    }

    // 3>连续出队5个整数，再入队2个整数，显示此时的队列Q的长度与front和rear的值
    printf("连续出队5个整数:\n");
    for (int i = 0; i < 5; i++)
    {
        QElemType e;
        DeQueue(&Q, &e);
        printf("Dequeued: %d\n", e);
    }
    printf("Queue front: %d, rear: %d\n", Q.front, Q.rear);
    printf("再入队2个整数:\n");
    for (int i = 11; i <= 12; i++)
    {
        QElemType e;
        scanf("%d", &e);
        EnQueue(&Q, e);
    }
    printf("Queue length: %d\n", QueueLength(Q));
    printf("Queue front: %d, rear: %d\n", Q.front, Q.rear);

    // 4>调用函数QueueReverse(Q)，将队列元素逆序重排，然后将队列元素出队并输出
    printf("Queue before reverse:\n");
    SqQueue tempQ = Q; // 复制队列以显示内容
    while (!QueueEmpty(tempQ))
    {
        QElemType e;
        DeQueue(&tempQ, &e);
        printf("%d ", e);
    }
    QueueReverse(&Q);
    printf("\nQueue after reverse:\n");
    while (!QueueEmpty(Q))
    {
        QElemType e;
        DeQueue(&Q, &e);
        printf("%d ", e);
    }
}