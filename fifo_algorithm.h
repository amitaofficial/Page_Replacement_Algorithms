/**
 * Author : Amita Ghosh
 * Date : 9 April 2021
 * Program Description: Page replacement algorithms
 * References: https://stackoverflow.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "common.h"

// insert node at end of linked list
void insertFIFO(Page *inputP);

// to remove current head from the list . Current head will have the oldest node.
void removeHeadFIFO();



// To perform FIFO algorithm .
// chart - denotes if the function is used for plotting , then return the number of faults
// lineNumber - total no of lines in the trace file
// noOfFrames - input no of frames
int FIFO(Page *pg, int noOfFrames, int lineNumber, int chart);

// Display linked list of frames
void displayFifoLinkedList(int hits, int faults);
