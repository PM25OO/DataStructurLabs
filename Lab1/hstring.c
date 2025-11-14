#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int Status;

typedef struct
{

    char *ch; // 若是非空串，则按串长分配存储区，否则ch为NULL。

    int length; // 串长度

} HString; // 堆分配串的类型定义

Status StrAssign(HString *S, char *chars)
{
    int len = strlen(chars);
    S->ch = (char *)malloc((len + 1) * sizeof(char));
    if (!S->ch)
        exit(OVERFLOW);
    strcpy(S->ch, chars);
    S->length = len;
    return OK;
}

int StrCompare(HString S, HString T)
{
    if (S.length != T.length)
        return S.length < T.length ? -1 : 1;
    else
    {
        for (int i = 0; i < S.length; i++)
        {
            if (S.ch[i] != T.ch[i])
                return -1;
        }
        return 0;
    }
}

int StrLength(HString S)
{
    return S.length;
}

Status ConCat(HString *T, HString S1, HString S2)
{
    T->ch = (char *)malloc((S1.length + S2.length + 1) * sizeof(char));
    if (!T->ch)
        exit(OVERFLOW);
    strcpy(T->ch, S1.ch);
    strcat(T->ch, S2.ch);
    T->length = S1.length + S2.length;
    return OK;
}

Status SubString(HString *Sub, HString S, int pos, int len)
{
    if (pos < 1 || pos > S.length || len < 0 || pos + len - 1 > S.length)
        return ERROR;
    Sub->ch = (char *)malloc((len + 1) * sizeof(char));
    if (!Sub->ch)
        exit(OVERFLOW);
    strncpy(Sub->ch, S.ch + pos - 1, len);
    Sub->ch[len] = '\0';
    Sub->length = len;
    return OK;
}

Status StrTraverse(HString S)
{
    for (int i = 0; i < S.length; i++)
    {
        printf("%c", S.ch[i]);
    }
    printf("\n");
    return OK;
}

Status StrCopy(HString *T, HString S)
{
    T->ch = (char *)malloc((S.length + 1) * sizeof(char));
    if (!T->ch)
        exit(OVERFLOW);
    strcpy(T->ch, S.ch);
    T->length = S.length;
    return OK;
}

int Index(HString S, HString T, int pos)
{
    if (pos < 1 || pos > S.length - T.length + 1)
        return 0;
    for (int i = pos - 1; i <= S.length - T.length; i++)
    {
        int j;
        for (j = 0; j < T.length; j++)
        {
            if (S.ch[i + j] != T.ch[j])
                break;
        }
        if (j == T.length)
            return i + 1;
    }
    return 0;
}

Status Replace(HString *S, HString T1, HString T2)
{
    int pos = 1;
    while (pos <= S->length)
    {
        pos = Index(*S, T1, pos);
        if (pos == 0)
            break;

        // 提取pos之前的部分
        HString before, after, temp;
        if (pos > 1)
        {
            SubString(&before, *S, 1, pos - 1);
        }

        // 提取pos+T1.length之后的部分
        if (pos + T1.length - 1 < S->length)
        {
            SubString(&after, *S, pos + T1.length, S->length - pos - T1.length + 1);
        }

        // 拼接：before + T2 + after
        free(S->ch);
        if (pos > 1)
        {
            ConCat(&temp, before, T2);
            if (pos + T1.length - 1 < S->length)
            {
                ConCat(S, temp, after);
                free(temp.ch);
                free(after.ch);
            }
            else
            {
                StrCopy(S, temp);
                free(temp.ch);
            }
            free(before.ch);
        }
        else
        {
            if (pos + T1.length - 1 < S->length)
            {
                ConCat(S, T2, after);
                free(after.ch);
            }
            else
            {
                StrCopy(S, T2);
            }
        }

        pos += T2.length;
    }
    return OK;
}

Status StrInsert(HString *S, int pos, HString T)
{
    if (pos < 1 || pos > S->length + 1)
        return ERROR;

    HString before, after;

    if (pos == 1)
    {
        // 在开头插入
        if (S->length > 0)
        {
            SubString(&after, *S, 1, S->length);
            free(S->ch);
            ConCat(S, T, after);
            free(after.ch);
        }
        else
        {
            free(S->ch);
            StrCopy(S, T);
        }
    }
    else if (pos == S->length + 1)
    {
        // 在末尾插入
        SubString(&before, *S, 1, S->length);
        free(S->ch);
        ConCat(S, before, T);
        free(before.ch);
    }
    else
    {
        // 在中间插入
        SubString(&before, *S, 1, pos - 1);
        SubString(&after, *S, pos, S->length - pos + 1);
        HString temp;
        ConCat(&temp, before, T);
        free(S->ch);
        ConCat(S, temp, after);
        free(before.ch);
        free(after.ch);
        free(temp.ch);
    }

    return OK;
}

Status StrDelete(HString *S, int pos, int len)
{
    if (pos < 1 || len < 0 || pos + len - 1 > S->length)
        return ERROR;

    if (len == 0)
        return OK;

    HString before, after;

    if (pos == 1 && pos + len - 1 == S->length)
    {
        // 删除整个串
        free(S->ch);
        S->ch = NULL;
        S->length = 0;
    }
    else if (pos == 1)
    {
        // 删除开头部分
        SubString(&after, *S, pos + len, S->length - pos - len + 1);
        free(S->ch);
        StrCopy(S, after);
        free(after.ch);
    }
    else if (pos + len - 1 == S->length)
    {
        // 删除末尾部分
        SubString(&before, *S, 1, pos - 1);
        free(S->ch);
        StrCopy(S, before);
        free(before.ch);
    }
    else
    {
        // 删除中间部分
        SubString(&before, *S, 1, pos - 1);
        SubString(&after, *S, pos + len, S->length - pos - len + 1);
        free(S->ch);
        ConCat(S, before, after);
        free(before.ch);
        free(after.ch);
    }

    return OK;
}

int main()
{
    HString T1, T2, T3, S;
    char* input = (char*)malloc(100 * sizeof(char));
    int position;

    printf("=== 数据准备 ===\n");

    printf("T1: ");
    scanf("%s", input);
    StrAssign(&T1, input);
    printf("\nT2: ");
    scanf("%s", input);
    StrAssign(&T2, input);
    printf("\nT3: ");
    scanf("%s", input);
    StrAssign(&T3, input);
    printf("\n");

    printf("=== 将T1赋给S ===\n");
    StrCopy(&S, T1);
    printf("S: ");
    StrTraverse(S);
    printf("\n");

    printf("=== 将T2插入到S的尾部 ===\n");
    StrInsert(&S, S.length + 1, T2);
    printf("S: ");
    StrTraverse(S);
    printf("\n");

    printf("=== 将S中的T1替换成T3 ===\n");
    Replace(&S, T1, T3);
    printf("S: ");
    StrTraverse(S);
    printf("\n");

    printf("=== 查找T2在S中的位置 ===\n");
    position = Index(S, T2, 1);
    printf("T2在S中的位置: %d\n", position);
    printf("\n");

    printf("=== 删除S中的子串T2 ===\n");
    if (position > 0)
    {
        StrDelete(&S, position, T2.length);
    }
    printf("删除后的S: ");
    StrTraverse(S);

    // 释放内存
    free(T1.ch);
    free(T2.ch);
    free(T3.ch);
    free(S.ch);

    return 0;
}