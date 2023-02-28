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

// Display linked list of frames
void displayLinkedList(int hits, int faults);

// insert node at head of linked list
void insert(Page *inputP);

//to update HIT node to head position
void updateHead(Page *inputP);

//to remove tail when there is a page fault
void removeTail();

// To perform LRU algorithm .
// chart - denotes if the function is used for plotting , then return the number of faults
// lineNumber - total no of lines in the trace file
// noOfFrames - input no of frames
int LRU(Page *pg, int noOfFrames, int lineNumber, int chart);
