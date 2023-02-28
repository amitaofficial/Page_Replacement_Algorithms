
#include "lru_algorithm.h"

struct NodeLru *head = NULL;
struct NodeLru *tail = NULL;

int noOfWritesLru = 0;

struct NodeLru
{
    struct NodeLru *next;
    struct NodeLru *previous;
    Page *p;
};

// insert node at head of linked list
void insert(Page *inputP)
{
    // inserting at head
    struct NodeLru *nodeLru1 = (struct NodeLru *)malloc(sizeof(struct NodeLru));
    struct NodeLru *current = NULL;
    nodeLru1->p = (struct Page *)malloc(sizeof(struct Page));
    nodeLru1->p->address = inputP->address;
    nodeLru1->p->readWrite = inputP->readWrite;
    nodeLru1->next = NULL;

    nodeLru1->previous = NULL;
    if (head == NULL) // check if list is empty
    {
        head = nodeLru1;
        tail = nodeLru1;
        return;
    }
    else
    {
        current = head;
        current->previous = nodeLru1;
        nodeLru1->next = current;
        head = nodeLru1; // insert at head
    }
}
//to update HIT node to head position
void updateHead(Page *inputP)
{
    struct NodeLru *current = head; // current

    int found = 0;

    while (current->next != NULL)
    {
        // find the current HIT node
        if (current->p->address == inputP->address) 
        {
            // check if the HIT node is already in head position
            if (current->p->address == head->p->address)
            {
                // do nothing
                found = 1;
            }
            else
            {
                if (current->previous != NULL) // validation check
                {
                   // move the HIT node to head position 
                    struct NodeLru *oldhead = head;
                    struct NodeLru *beforeNodeLru = current->previous;
                    struct NodeLru *afterNodeLru = current->next;

                    oldhead->previous = current;
                    beforeNodeLru->next = afterNodeLru;
                    afterNodeLru->previous = beforeNodeLru;
                    head = current;
                    head->previous = NULL;
                    head->next = oldhead;
                    found = 1;
                }
                else
                {
                    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% no previous value to %d\n", current->p->address);
                }
            }

            break;
        }
        current = current->next;
    }
    // do the check for last pointer also
    if (found == 0 && (current->p->address == inputP->address))
    {
        if (current->previous != NULL)
        {
            // move the HIT node to head position
            struct NodeLru *oldhead = head;
            struct NodeLru *beforeNodeLru = current->previous;
            struct NodeLru *afterNodeLru = NULL;

            beforeNodeLru->next = NULL; //afterNodeLru
            oldhead->previous = current;
            head = current;
            head->previous = NULL;
            head->next = oldhead;
            found = 1;

            // update tail pointer also (this happens incase HIT node is in tail position)
            struct NodeLru *ptr = head;
            struct NodeLru *secondLast = NULL;

            while (ptr->next != NULL)
            {
                secondLast = ptr;
                ptr = ptr->next;
            }
            tail = ptr;
        }
        else
        {
            printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% LASTno previous value\n");
        }
    }
}
//to remove tail when there is a page fault
void removeTail()
{
    struct NodeLru *ptr = head;
    struct NodeLru *secondLast = NULL;

    // loop through linked list
    while (ptr->next != NULL)
    {
        secondLast = ptr; //get secondlast pointer
        ptr = ptr->next;
    }
    if (ptr->p->readWrite == 1)
    {
        noOfWritesLru++; // write to disk if page is dirty before replacing.
    }

    secondLast->next = NULL; // removed current tail
    tail = secondLast; // tail pointer updated
}

//#############################################################################################
// To perform LRU algorithm .
// chart - denotes if the function is used for plotting , then return the number of faults
// lineNumber - total no of lines in the trace file
// noOfFrames - input no of frames

int LRU(Page *pg, int noOfFrames, int lineNumber, int chart)
{
    int faults = 0; // no of faults
    int count = 0;  // no of occupied frames
    int hits = 0;   // no of hits

    // check if initially frames are empty , if no insert first few pages directly
    while (count < noOfFrames)
    {
        insert((pg + count)); // insert at the head of the list
        count++;
        faults++; // increase page faults
    }
    // once the frames are filled, start getting rest of the pages from the page array
    for (int i = count; i < lineNumber; i++)
    {
        Page *tempPage = (pg + i);
        struct NodeLru *ptr = head;
        int found = 0;
        struct NodeLru *currentNodeLru = NULL;

        // loop through the linked list of frames
        while (ptr->next != NULL)
        {
            // check if the new page address matches anyone existing
            if (ptr->p->address == tempPage->address)
            {
                hits++; // its is HIT !
                found = 1;
                currentNodeLru = ptr; // store that HIT node to update its position as head
                break; // break from the loop
            }
            ptr = ptr->next;
        }
        count++;
        
        // do the address check for the  last pointer also
        if ((found == 0) && (ptr->p->address == tempPage->address))
        {
            hits++; // its is HIT !
            updateHead(ptr->p); // update the HIT node position to HEAD position
        }
        else if (found == 1)
        {
            updateHead(currentNodeLru->p); // update the HIT node position to HEAD position
        }
        else if (found == 0)
        {
            faults++; // increase fault number
            insert(tempPage); // insert new page at the end
            removeTail();     // remove current tail
        }

    }
    // to plot any chart with number of faults , chart value will be 1
    if (chart == 1)
    {
        return faults; // return number of faults
    }
    else
    {
        displayLinkedList(hits, faults); // just display contents of the frames after whole execution
        return 0;
    }
}
// Display linked list of frames
void displayLinkedList(int hits, int faults)
{
    // linked list

    struct NodeLru *ptr = head;

    printf("[headLRU] =>");
    while (ptr->next != NULL)
    {
        printf(" %08x =>", ptr->p->address);
        ptr = ptr->next;
    }
    printf(" %x =>", ptr->p->address);
    printf(" endLRU \n");

    printf("hits : %d\n", hits);
    printf("Reads : %d\n", faults);
    printf("writes : %d\n", noOfWritesLru);
}
