#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, disk, head, i, j, temp, pos;
    char dir;
    int a[50], cscan = 0, look = 0, current;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    printf("Enter disk size: ");
    scanf("%d", &disk);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter direction (L/R): ");
    scanf(" %c", &dir);

    printf("Enter request queue: ");
    for (i = 0; i < n; i++)
        scanf("%d", &a[i]);

    /* Sort requests */
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (a[i] > a[j])
            {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }

    current = head;
    printf("\nC-SCAN Sequence: %d", current);

    if (dir == 'R' || dir == 'r')
    {
        pos = 0;
        while (pos < n && a[pos] < head)
            pos++;

        for (i = pos; i < n; i++)
        {
            cscan += abs(current - a[i]);
            current = a[i];
            printf(" -> %d", current);
        }

        cscan += abs(current - (disk - 1));
        current = disk - 1;
        printf(" -> %d", current);

        cscan += disk - 1;
        current = 0;
        printf(" -> 0");

        for (i = 0; i < pos; i++)
        {
            cscan += abs(current - a[i]);
            current = a[i];
            printf(" -> %d", current);
        }
    }
    else
    {
        pos = n - 1;
        while (pos >= 0 && a[pos] > head)
            pos--;

        for (i = pos; i >= 0; i--)
        {
            cscan += abs(current - a[i]);
            current = a[i];
            printf(" -> %d", current);
        }

        cscan += current;
        current = 0;
        printf(" -> 0");

        cscan += disk - 1;
        current = disk - 1;
        printf(" -> %d", current);

        for (i = n - 1; i > pos; i--)
        {
            cscan += abs(current - a[i]);
            current = a[i];
            printf(" -> %d", current);
        }
    }

    printf("\nC-SCAN Total Head Movement: %d\n", cscan);

    /* LOOK */
    current = head;
    printf("\nLOOK Sequence: %d", current);

    if (dir == 'R' || dir == 'r')
    {
        pos = 0;
        while (pos < n && a[pos] < head)
            pos++;

        for (i = pos; i < n; i++)
        {
            look += abs(current - a[i]);
            current = a[i];
            printf(" -> %d", current);
        }

        for (i = pos - 1; i >= 0; i--)
        {
            look += abs(current - a[i]);
            current = a[i];
            printf(" -> %d", current);
        }
    }
    else
    {
        pos = n - 1;
        while (pos >= 0 && a[pos] > head)
            pos--;

        for (i = pos; i >= 0; i--)
        {
            look += abs(current - a[i]);
            current = a[i];
            printf(" -> %d", current);
        }

        for (i = pos + 1; i < n; i++)
        {
            look += abs(current - a[i]);
            current = a[i];
            printf(" -> %d", current);
        }
    }

    printf("\nLOOK Total Head Movement: %d\n", look);

    printf("\n--- Comparison ---\n");
    printf("C-SCAN : %d\n", cscan);
    printf("LOOK   : %d\n", look);

    if (cscan < look)
        printf("C-SCAN performs better due to lower head movement.\n");
    else if (look < cscan)
        printf("LOOK performs better due to lower head movement.\n");
    else
        printf("Both algorithms have equal head movement.\n");

    return 0;
}