#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "proj4.h"

void initializeGrid(grid * g, char * fileName) {
    FILE * file = fopen(fileName, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    } // if

    unsigned int n = 0;

    while (fgetc(file) != '\n') {
        n++;
    } // while

    rewind(file);

    g -> n = n;
    g -> p = (unsigned char **) malloc(n * sizeof(unsigned char *));

    for (unsigned int i = 0; i < n; i++) {
        g -> p[i] = (unsigned char *) malloc(n * sizeof(unsigned char));

        for (unsigned int j = 0; j < n; j++) {
            fscanf(file, "%1hhu", &(g -> p[i][j]));
        } // for
    } // for

    fclose(file);
} // initialize

struct inputVal {
    int startingRow;
    int numberOfThreads;
    unsigned long sum;
    grid* inputGrid;
    grid* outputGrid;
};

bool scanLeftDiagonalDown(int iRow, int iCol, grid* input, unsigned long sum)
{
    //printf("Calling ScanLeftDiagonalDown\n");
    
    int runningSum = 0;
    int prevValue = 0;
    int ic = iCol;
    for (int ir = iRow; ir < input->n; ir++) {
                
        if (ic >= 0)
        {
            //printf("Comparing if consecutive values are equal\n");
            //printf("Checking [%d][%d] value in the cell is  %d\n",ir, ic, input->p[ir][ic]);
            //printf("prevalue is %d, prev + current cell is %d and target sum is %lu\n", prevValue, prevValue + input->p[ir][ic], sum);
            // check if two consecutive diagonal values is equal to the target sum
            if (prevValue + input->p[ir][ic] == sum) {

                return true;
            }

            prevValue = input->p[ir][ic];
            
            // check if the runningSum is equal to the target sum
            runningSum += input->p[ir][ic];
            
            //printf("Running total is %d\n", runningSum);
            
            if (runningSum == sum)
                return true;

            ic--;
        }
        else
            return false;
                
    }
    return false;
}

bool scanRightDiagonalDown(int iRow, int iCol,  grid* input, unsigned long sum)
{
    
    //printf("Calling ScanRightDiagonalDown\n");
    
    int runningSum = 0;
    int prevValue = 0;
    int ic = iCol;
    for (int ir = iRow; ir < input->n; ir++) {

        if (ic < input->n)
        {
            //printf("Comparing if consecutive values are equal\n");
            //printf("Checking [%d][%d] value in the cell is  %d\n",ir, ic, input->p[ir][ic]);
            //printf("prevalue is %d, prev + current cell is %d and target sum is %lu\n", prevValue, prevValue + input->p[ir][ic], sum);
            // check if two consecutive diagonal values is equal to the target sum
            if (prevValue + input->p[ir][ic] == sum)
                return true;
            prevValue = input->p[ir][ic];

            // check if the runningSum is equal to the target sum
            runningSum += input->p[ir][ic];
            
            //printf("Running total is %d\n", runningSum);
            
            if (runningSum == sum)
                return true;

            ic++;
        }
        else {
            return false;
        }     
    }
    return false;
}

bool scanLeftDiagonalUp(int iRow, int iCol,  grid* input, unsigned long sum)
{
    //printf("Calling scanLeftDiagonalUp\n");
    
    int runningSum = 0;
    int prevValue = 0;
    int ic = iCol;
    for (int ir = iRow; ir >= 0; ir--) {

        if (ic >= 0)
        {
            //printf("Comparing if consecutive values are equal\n");
            //printf("Checking [%d][%d] value in the cell is  %d\n",ir, ic, input->p[ir][ic]);
            //printf("prevalue is %d, prev + current cell is %d and target sum is %lu\n", prevValue, prevValue + input->p[ir][ic], sum);
            
            // check if two consecutive diagonal values is equal to the target sum
            if (prevValue + input->p[ir][ic] == sum)
                return true;
            prevValue = input->p[ir][ic];

            // check if the runningSum is equal to the target sum
            runningSum += input->p[ir][ic];
            
            //printf("Running total is %d\n", runningSum);
            
            if (runningSum == sum)
                return true;

            ic--;
        }
        else {
            return false;
        }   
    }
    return false;

}

bool scanRightDiagonalUp(int iRow, int iCol, grid* input, unsigned long sum)
{
    //printf("Calling scanRightDiagonalUp\n");
    
    int runningSum = 0;
    int prevValue = 0;
    int ic = iCol;
    for (int ir = iRow; ir >= 0; ir--) {

        if (ic < input->n)
        {
            //printf("Comparing if consecutive values are equal\n");
            //printf("Checking [%d][%d] value in the cell is  %d\n",ir, ic, input->p[ir][ic]);
            //printf("prevalue is %d, prev + current cell is %d and target sum is %lu\n", prevValue, prevValue + input->p[ir][ic], sum);
            
            // check if two consecutive diagonal values is equal to the target sum
            if (prevValue + input->p[ir][ic] == sum)
                return true;
            prevValue = input->p[ir][ic];

            // check if the runningSum is equal to the target sum
            runningSum += input->p[ir][ic];
            
             //printf("Running total is %d\n", runningSum);
            
            if (runningSum == sum)
                return true;

            ic++;
        }
        else {
            return false;
        }
    }
    return false;
}

void * computeDiagonalSums(void * arg) {
    struct inputVal *iparam = (struct inputVal *) arg;
    //inputVal inputParam = *iparam;
    
    int startRow = (*iparam).startingRow;
    int threadCount = (*iparam).numberOfThreads;

    int n = (*iparam).inputGrid->n;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    int* arr = (int*)malloc(n * sizeof(int));
    
    for (unsigned int iRow = startRow; iRow < n; )
    {    
    
        for (unsigned int iCol = 0; iCol < n; iCol++)
        {
            bool iRet = scanLeftDiagonalDown(iRow, iCol, (*iparam).inputGrid, (*iparam).sum);
            bool iRet2 = scanRightDiagonalDown(iRow, iCol, (*iparam).inputGrid, (*iparam).sum);
            bool iRet3 = scanLeftDiagonalUp(iRow, iCol, (*iparam).inputGrid, (*iparam).sum);
            bool iRet4 = scanRightDiagonalUp(iRow, iCol, (*iparam).inputGrid, (*iparam).sum);

            if (iRet == true || iRet2 == true || iRet3 == true || iRet4 == true)
            {
                arr[iCol] = (*iparam).inputGrid->p[iRow][iCol];
            }
            else
                arr[iCol] = 0;
            
            //printf("Result Value is %d\n", arr[iCol]);
            
        }

        

        pthread_mutex_lock(&mutex);
       for (unsigned int iCellVal = 0; iCellVal < n; iCellVal++)
       {
         if(iRow < n) 
     
            (*iparam).outputGrid->p[iRow][iCellVal] = arr[iCellVal];
       }
       pthread_mutex_unlock(&mutex);
        
       iRow += threadCount;
    }       
    
    free(arr);

    return arg;
} // computeDiagonalSums

void diagonalSums(grid * input, unsigned long s, grid * output, int t) {
    output -> n = input -> n;
    output -> p = (unsigned char **) malloc(output -> n * sizeof(unsigned char *));

    for (unsigned int i = 0; i < output -> n; i++) {
        output -> p[i] = (unsigned char *) calloc(output -> n, sizeof(unsigned char));
    } // for

    printf(" in diagonalSums\n" );

    pthread_t threads[t];
    for (int i = 0; i < t; i++) {

        struct inputVal* inputParam = malloc(sizeof(struct inputVal));
        (*inputParam).startingRow = i;
        (*inputParam).sum = s;
        (*inputParam).numberOfThreads = t;
        (*inputParam).inputGrid = input;
        (*inputParam).outputGrid = output;

        printf("Calling threads\n");
        
        pthread_create(&threads[i], NULL, computeDiagonalSums, (void*)inputParam);
    } // for

    for (int i = 0; i < t - 1; i++) {
        pthread_join(threads[i], NULL);
    } // for
} // diagonalSums

void writeGrid(grid * g, char * fileName) {
    FILE * file = fopen(fileName, "w");

    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    } // if

    for (unsigned int i = 0; i < g -> n; i++) {
        for (unsigned int j = 0; j < g -> n; j++) {
            fprintf(file, "%hhu", g -> p[i][j]);
        } // for
        fprintf(file, "\n");
    } // for

    fclose(file);
} // writeGrid

void freeGrid(grid * g) {
    for (unsigned int i = 0; i < g -> n; i++) {
        free(g -> p[i]);
    } // for

    free(g -> p);
} // freeGrid
