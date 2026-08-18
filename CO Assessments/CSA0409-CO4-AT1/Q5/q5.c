#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, f, head, alloc, sched;
    int req[50], block[50], used[100] = {0};
    int i, j, k, current, seek, min, index, temp;

    printf("Enter total disk blocks: ");
    scanf("%d", &n);

    printf("Enter file size: ");
    scanf("%d", &f);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter number of disk requests: ");
    scanf("%d", &k);

    printf("Enter request queue: ");
    for (i = 0; i < k; i++)
        scanf("%d", &req[i]);

    printf("1. Contiguous\n2. Indexed\n");
    printf("Enter allocation method: ");
    scanf("%d", &alloc);

    printf("1. FCFS\n2. SSTF\n");
    printf("Enter scheduling method: ");
    scanf("%d", &sched);

    /* Allocation */
    if (alloc == 1)
    {
        for (i = 0; i <= n - f; i++)
        {
            int free = 1;

            for (j = i; j < i + f; j++)
                if (used[j])
                    free = 0;

            if (free)
            {
                for (j = 0; j < f; j++)
                {
                    block[j] = i + j;
                    used[i + j] = 1;
                }
                break;
            }
        }

        if (i > n - f)
        {
            printf("Contiguous allocation failed\n");
            return 0;
        }
    }
    else if (alloc == 2)
    {
        if (f + 1 > n)
        {
            printf("Indexed allocation failed\n");
            return 0;
        }

        block[0] = 0;
        used[0] = 1;

        j = 1;
        for (i = 1; i < n && j < f + 1; i++)
        {
            if (!used[i])
            {
                block[j] = i;
                used[i] = 1;
                j++;
            }
        }

        if (j < f + 1)
        {
            printf("Indexed allocation failed\n");
            return 0;
        }
    }
    else
    {
        printf("Invalid allocation method\n");
        return 0;
    }

    printf("\nAllocated blocks: ");
    for (i = 0; i < f; i++)
        printf("%d ", block[i]);

    /* Scheduling allocated blocks */
    current = head;
    seek = 0;

    printf("\nSeek sequence: %d", current);

    if (sched == 1)
    {
        /* FCFS */
        for (i = 0; i < f; i++)
        {
            seek += abs(current - block[i]);
            current = block[i];
            printf(" -> %d", current);
        }
    }
    else if (sched == 2)
    {
        /* SSTF */
        for (i = 0; i < f; i++)
        {
            min = 9999;
            index = -1;

            for (j = 0; j < f; j++)
            {
                if (used[j] != 2 &&
                    abs(current - block[j]) < min)
                {
                    min = abs(current - block[j]);
                    index = j;
                }
            }

            seek += min;
            current = block[index];
            used[index] = 2;

            printf(" -> %d", current);
        }
    }
    else
    {
        printf("\nInvalid scheduling method\n");
        return 0;
    }

    printf("\nTotal Seek Time: %d\n", seek);

    printf("\nCombination: ");

    if (alloc == 1)
        printf("Contiguous + ");
    else
        printf("Indexed + ");

    if (sched == 1)
        printf("FCFS\n");
    else
        printf("SSTF\n");

    printf("Performance: %d seek operations\n", seek);

    return 0;
}