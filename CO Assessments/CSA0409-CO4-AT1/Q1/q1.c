#include <stdio.h>

int main()
{
    int n, f, m, i, start;

    printf("Enter total disk blocks: ");
    scanf("%d", &n);

    printf("Enter file size: ");
    scanf("%d", &f);

    printf("1. Contiguous\n2. Linked\n3. Indexed\n");
    printf("Enter method: ");
    scanf("%d", &m);

    if (f > n)
    {
        printf("Allocation not possible\n");
        return 0;
    }

    if (m == 1)
    {
        start = 0;

        if (start + f > n)
            printf("Contiguous allocation failed\n");
        else
        {
            printf("Allocated blocks: ");
            for (i = start; i < start + f; i++)
                printf("%d ", i);
        }
    }
    else if (m == 2)
    {
        printf("Allocated blocks: ");
        for (i = 0; i < f; i++)
            printf("%d ", i);
    }
    else if (m == 3)
    {
        if (f + 1 > n)
            printf("Indexed allocation not possible\n");
        else
        {
            printf("Index block: 0\n");
            printf("Allocated blocks: ");
            for (i = 1; i <= f; i++)
                printf("%d ", i);
        }
    }
    else
        printf("Invalid method\n");

    return 0;
}