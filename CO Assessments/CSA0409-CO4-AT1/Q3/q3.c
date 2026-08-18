#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, disk, head, i, j, temp;
    int req[50], a[50], b[50], c[50];
    int fcfs = 0, sstf = 0, scan = 0;
    int pos, min, index, current;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    printf("Enter disk size: ");
    scanf("%d", &disk);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter request queue: ");
    for (i = 0; i < n; i++)
        scanf("%d", &req[i]);

    current = head;
    printf("\nFCFS Sequence: %d", current);

    for (i = 0; i < n; i++)
    {
        fcfs += abs(current - req[i]);
        current = req[i];
        printf(" -> %d", current);
    }

    printf("\nFCFS Total Seek Time: %d\n", fcfs);

    /* SSTF */
    for (i = 0; i < n; i++)
        b[i] = req[i];

    current = head;
    printf("\nSSTF Sequence: %d", current);

    for (i = 0; i < n; i++)
    {
        min = 9999;
        index = -1;

        for (j = 0; j < n; j++)
        {
            if (b[j] != -1 && abs(current - b[j]) < min)
            {
                min = abs(current - b[j]);
                index = j;
            }
        }

        sstf += min;
        current = b[index];
        b[index] = -1;

        printf(" -> %d", current);
    }

    printf("\nSSTF Total Seek Time: %d\n", sstf);

    for (i = 0; i < n; i++)
        a[i] = req[i];

    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (a[i] > a[j])
            {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }

    printf("\nSCAN Sequence: %d", head);

    pos = 0;

    while (pos < n && a[pos] < head)
        pos++;

    for (i = pos - 1; i >= 0; i--)
    {
        scan += abs(head - a[i]);
        head = a[i];
        printf(" -> %d", head);
    }

    if (head != 0)
    {
        scan += head;
        head = 0;
        printf(" -> 0");
    }

    for (i = pos; i < n; i++)
    {
        scan += abs(head - a[i]);
        head = a[i];
        printf(" -> %d", head);
    }

    printf("\nSCAN Total Seek Time: %d\n", scan);

    printf("\n--- Performance Comparison ---\n");
    printf("FCFS : %d\n", fcfs);
    printf("SSTF : %d\n", sstf);
    printf("SCAN : %d\n", scan);

    if (fcfs <= sstf && fcfs <= scan)
        printf("Best Algorithm: FCFS\n");
    else if (sstf <= fcfs && sstf <= scan)
        printf("Best Algorithm: SSTF\n");
    else
        printf("Best Algorithm: SCAN\n");

    return 0;
}