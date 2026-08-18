#include <stdio.h>

int main()
{
    int n, i, req, block;

    printf("Enter number of disk blocks: ");
    scanf("%d", &n);

    int bitmap[n];

    printf("Enter bitmap (0=Free, 1=Allocated):\n");
    for (i = 0; i < n; i++)
        scanf("%d", &bitmap[i]);

    printf("Enter number of blocks to allocate: ");
    scanf("%d", &req);

    int count = 0;

    for (i = 0; i < n && count < req; i++)
    {
        if (bitmap[i] == 0)
        {
            bitmap[i] = 1;
            count++;
        }
    }

    if (count < req)
        printf("Insufficient free space\n");
    else
    {
        printf("Blocks allocated successfully\n");
        printf("Updated bitmap: ");
        for (i = 0; i < n; i++)
            printf("%d ", bitmap[i]);
    }

    printf("\nEnter block to deallocate (-1 to skip): ");
    scanf("%d", &block);

    if (block >= 0 && block < n)
    {
        bitmap[block] = 0;
        printf("Block %d deallocated\n", block);
    }

    printf("Final bitmap: ");
    for (i = 0; i < n; i++)
        printf("%d ", bitmap[i]);

    return 0;
}