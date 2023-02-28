/**
 * Author : Amita Ghosh
 * Date : 9 April 2021
 * Program Description: Page replacement algorithms
 * References: https://stackoverflow.com
 */
#include "fifo_algorithm.h"

struct NodeFifo
{
    struct NodeFifo *next;
    Page *p;
};
struct NodeFifo *headFifo = NULL;
int noOfFrames = 0;
int noOfWritesFifo = 0;

// insert node at end of linked list
void insertFIFO(Page *inputP)
{
    struct NodeFifo *nodeFifo1 = (struct NodeFifo *)malloc(sizeof(struct NodeFifo)); // create a new node
    struct NodeFifo *current = NULL; // for looping through list
    // assign data to the node
    nodeFifo1->p = (struct Page *)malloc(sizeof(struct Page));
    nodeFifo1->p->address = inputP->address;
    nodeFifo1->p->readWrite = inputP->readWrite;
    nodeFifo1->next = NULL;
    // if list is empty 
    if (headFifo == NULL)
    {
        nodeFifo1->next = NULL;
        nodeFifo1->p = inputP;
        headFifo = nodeFifo1; // make the node as head
        return;
    }
    else
    {   // loop through end of the list and append the node at the end
        current = headFifo;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = nodeFifo1;
    }
}
// to remove current head from the list . Current head will have the oldest node.
void removeHeadFIFO()
{
    if (headFifo == NULL)
    {
        printf("Nothing to be removed");
    }
    else
    {
        // check if page is dirty and if yes, increase noOfWritesFifo
        if (headFifo->p->readWrite == 1)
        {
            noOfWritesFifo++;
        }
        // remove the head page
        struct NodeFifo *nodeFifo = NULL;
        nodeFifo = headFifo->next;
        headFifo = nodeFifo;
    }
}
// To perform FIFO algorithm .
// chart - denotes if the function is used for plotting , then return the number of faults
// lineNumber - total no of lines in the trace file
// noOfFrames - input no of frames
int FIFO(Page *pg, int noOfFrames, int lineNumber,int chart)
{
    int faults = 0;// no of faults
    int count = 0; // no of occupied frames
    int hits = 0;// no of hits

    // check if initially frames are empty , if no insert first few pages directly
    while (count < noOfFrames)
    {
        insertFIFO((pg + count)); // insert at the end of the list
        count++;
        faults++; // increase page faults
    }
    // once the frames are filled, start getting rest of the pages from the page array
    for (int i = count; i < lineNumber; i++)
    {
        Page *tempPage = (pg + i);

        int minValue = __INT_MAX__;
        struct NodeFifo *ptr = headFifo;
        int found = 0;
        // loop through the linked list of frames
        while (ptr->next != NULL)
        {
            // check if the new page address matches anyone existing
            if (ptr->p->address == tempPage->address)
            {
                hits++; // its is HIT !
                found = 1;
                break;// break from the loop
            }
            ptr = ptr->next;
        }
        // do the address check for the  last pointer also
        if ((found == 0) && (ptr->p->address == tempPage->address))
        {
            hits++; // its is HIT !
            found = 1;
        }
        else if (found == 0) // if no HITS are there, then its a page fault
        {
            faults++; // increase fault number
            insertFIFO(tempPage);// insert new page at the end
            removeHeadFIFO();// remove current head
        }
    }
    // to plot any chart with number of faults , chart value will be 1 
    if(chart == 1){
        return faults; // return number of faults
    }
    else
    {
        displayFifoLinkedList(hits, faults); // just display contents of the frames after whole execution
        return 0;
    }
    
}
// Display linked list of frames 
void displayFifoLinkedList(int hits, int faults)
{
    // linked list

    struct NodeFifo *ptr = headFifo;
    printf("[headFifo] =>");
    while (ptr->next != NULL)
    {
        printf(" %08x =>", ptr->p->address);
        ptr = ptr->next;
    }
    printf(" %08x =>", ptr->p->address);
    printf(" endFifo \n");

    printf("hits : %d\n", hits);
    printf("Reads : %d\n", faults);
    printf("writes : %d\n", noOfWritesFifo);
}
