#include <stdio.h>
#include <stdlib.h>

void printPageTable(int pageNumber, int frameNumber, char presentBit)
{
    printf("%5d\t|\t%5d\t|\t%c\n", pageNumber, frameNumber, presentBit);
}

int main()
{
    int pageSize, fileSize;
    int numPages, numFrames;
    int *pageTable;

    printf("Enter the size of the file (in bytes): ");
    scanf("%d", &fileSize);

    printf("Enter the page size (in bytes): ");
    scanf("%d", &pageSize);

    numPages = (fileSize + pageSize - 1) / pageSize;
    numFrames = numPages;

    pageTable = (int*)malloc(numPages * sizeof(int));

    printf("\nPage Table:\n");
    printf("Page Numbert|\tFrame Numbert|\tPresent Bit\n");
    printf("---------------------------------------------\n");

    for (int i = 0; i < numPages; i++) {
        pageTable[i] = i;
        printPageTable(i, i, 'Y');
    }

    free(pageTable);

    return 0;
}