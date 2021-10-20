/*
 * Name: Jake Beslanowitch
 * Email: jakebeslanowitch@u.boisestate.edu
 * @Github Username: jakebeslanowitch
 */
 
#include "ThreadsafeBoundedList.h"

struct tsb_list {
    struct list *list;
    int capacity;
    Boolean stop_requested;
    pthread_mutex_t mutex;
    pthread_cond_t listNotFull;
    pthread_cond_t listNotEmpty;
};

struct tsb_list * tsb_createList(int (*equals)(const void *, const void *), 
                    char * (*toString)(const void *),
                    void (*freeObject)(void *),
				    int capacity) {
                       struct tsb_list *tsl;
                       tsl = malloc(sizeof(struct tsb_list));
                       (*tsl).list = createList(equals, toString, freeObject);
                       (*tsl).capacity = capacity;
                       (*tsl).stop_requested = FALSE;
                       pthread_mutex_init(&(*tsl).mutex, NULL);
                       pthread_cond_init(&(*tsl).listNotEmpty, NULL);
                       pthread_cond_init(&(*tsl).listNotFull, NULL);
                       return tsl;
                   }

/**
 * Frees all elements of the given list and the <code>struct *list</code> itself.
 * Does nothing if list is <code>NULL</code>. Also frees the associated mutex and
 * condition varibles and the wrapper structure.
 *
 * @param list a pointer to a <code>List</code>.
 */
void tsb_freeList(struct tsb_list * list) {
    pthread_mutex_lock(&(*list).mutex);
    freeList((*list).list);
    pthread_mutex_unlock(&(*list).mutex);
    pthread_mutex_destroy(&(*list).mutex);
    pthread_cond_destroy(&(*list).listNotEmpty);
    pthread_cond_destroy(&(*list).listNotFull);
    free(list);
}

/**
 * Returns the size of the given list.
 *
 * @param list a pointer to a <code>List</code>.
 * @return The current size of the list.
 */
int tsb_getSize(struct tsb_list * list) {
    int size = 0;
    size = getSize((*list).list);
    return size;
}

/**
 * Returns the maximum capacity of the given list.
 *
 * @param list a pointer to a <code>List</code>.
 * @return The macimum capacity of the list.
 */
int tsb_getCapacity(struct tsb_list * list) {
    return (*list).capacity;
}

/**
 * Sets the maximum capacity of the given list.
 *
 * @param list a pointer to a <code>List</code>.
 * @param capacity the maximum allowed capacity of the list
 * @return none
 */
void tsb_setCapacity(struct tsb_list * list, int capacity) {
    pthread_mutex_lock(&(*list).mutex);
    (*list).capacity = capacity;
    pthread_mutex_unlock(&(*list).mutex);
}

/**
 * Checks if the list is empty.
 *
 * @param  list a pointer to a <code>List</code>.
 * @return true if the list is empty; false otherwise.
 */
Boolean tsb_isEmpty(struct tsb_list * list) {
    int empty = TRUE;
    empty = isEmpty((*list).list);
    return empty;
}

/**
 * Checks if the list is full.
 *
 * @param  list a pointer to a <code>List</code>.
 * @return true if the list is full to capacity; false otherwise.
 */
Boolean tsb_isFull(struct tsb_list * list) {
    int full = FALSE;
    if(tsb_getSize(list) >= tsb_getCapacity(list)) {
        full = TRUE;
    }
    return full;
}

/**
 * Adds a node to the front of the list. After this method is called,
 * the given node will be the head of the list. (Node must be allocated
 * before it is passed to this function.) If the list and/or node are NULL,
 * the function will do nothing and return.
 *
 * @param list a pointer to a <code>List</code>.
 * @param node a pointer to the node to add.
 */
void tsb_addAtFront(struct tsb_list * list, NodePtr node) {
    pthread_mutex_lock(&(*list).mutex);
    while((*list).stop_requested == FALSE && tsb_isFull(list)) {
        pthread_cond_wait(&(*list).listNotFull, &(*list).mutex);
    }
    addAtFront((*list).list, node);            // maybe, add pointer to node because of addToFront's method (List.h)
    pthread_cond_signal(&(*list).listNotEmpty);
    pthread_mutex_unlock(&(*list).mutex);
   
}

/**
 * Adds a node to the front of the list. After this method is called,
 * the given node will be the head of the list. (Node must be allocated
 * before it is passed to this function.) If the list and/or node are NULL,
 * the function will do nothing and return.
 *
 * @param list a pointer to a <code>List</code>.
 * @param node a pointer to the node to add.
 */
void tsb_addAtRear(struct tsb_list * list, NodePtr node) {
    pthread_mutex_lock(&(*list).mutex);
    while((*list).stop_requested == FALSE && tsb_isFull(list)) {
        pthread_cond_wait(&(*list).listNotFull, &(*list).mutex);
    }

    addAtRear((*list).list, node);
    pthread_cond_signal(&(*list).listNotEmpty);
    pthread_mutex_unlock(&(*list).mutex);
    
}

/**
 * Removes the node from the front of the list (the head node) and returns
 * a pointer to the node that was removed. If the list is NULL or empty,
 * the function will do nothing and return NULL.
 *
 * @param list a pointer to a <code>List</code>.
 * @return a pointer to the node that was removed.
 */
NodePtr tsb_removeFront(struct tsb_list * list) {
    NodePtr node;
    pthread_mutex_lock(&(*list).mutex);
    while((*list).stop_requested == FALSE && tsb_isEmpty(list)) {
        pthread_cond_wait(&(*list).listNotEmpty, &(*list).mutex);
    }
    node = removeFront((*list).list);
    
    pthread_mutex_unlock(&(*list).mutex);
    pthread_cond_signal(&(*list).listNotFull);
    return node;
}

/**
 * Removes the node from the rear of the list (the tail node) and returns
 * a pointer to the node that was removed. If the list is NULL or empty,
 * the function will do nothing and return NULL.
 *
 * @param list a pointer to a <code>List</code>.
 * @return a pointer to the node that was removed.
 */
NodePtr tsb_removeRear(struct tsb_list * list) {
    NodePtr node;
    pthread_mutex_lock(&(*list).mutex);
    while((*list).stop_requested == FALSE && tsb_isEmpty(list)) {
        pthread_cond_wait(&(*list).listNotEmpty, &(*list).mutex);
    }
    node = removeRear((*list).list);
    pthread_cond_signal(&(*list).listNotFull);
    pthread_mutex_unlock(&(*list).mutex);
    
    return node;
}

/**
 * Removes the node pointed to by the given node pointer from the list and returns
 * the pointer to it. Assumes that the node is a valid node in the list. If the node
 * pointer is NULL, the function will do nothing and return NULL.
 *
 * @param list a pointer to a <code>List</code>.
 * @param node a pointer to the node to remove.
 * @return a pointer to the node that was removed.
 */
NodePtr tsb_removeNode(struct tsb_list * list, NodePtr node) {
    pthread_mutex_lock(&(*list).mutex);
    while((*list).stop_requested == FALSE && tsb_isEmpty(list)) {
        pthread_cond_wait(&(*list).listNotEmpty, &(*list).mutex);
    }

    node = removeNode((*list).list, node);
    pthread_cond_signal(&(*list).listNotFull);
    pthread_mutex_unlock(&(*list).mutex);
    
    return node;
}

// non needed function, implemented for my sake but commented out
/**
 * Searches the list for a node with the given key and returns the pointer to the
 * found node.
 *
 * @param list a pointer to a <code>List</code>.
 * @param the object to search for.
 * @return a pointer to the node that was found. Or <code>NULL</code> if a node 
 * with the given key is not found or the list is <code>NULL</code> or empty.
 */
// NodePtr tsb_search(struct tsb_list * list, const void *obj) {
//     NodePtr node = NULL;
//     pthread_mutex_lock(&(*list).mutex);
//     node = search((*list).list, node);
//     pthread_cond_signal(&(*list).listNotEmpty);
//     pthread_mutex_unlock(&(*list).mutex);
//     return node;
// }

/**
 * Reverses the order of the given list.
 *
 * @param list a pointer to a <code>List</code>.
 */
void tsb_reverseList(struct tsb_list *  list) {
    pthread_mutex_lock(&(*list).mutex);
    reverseList((*list).list);
    pthread_mutex_unlock(&(*list).mutex);
}

/**
 * Prints the list.
 *
 * @param list a pointer to a <code>List</code>.
 */
void tsb_printList(struct tsb_list * list) {
    pthread_mutex_lock(&(*list).mutex);
    printList((*list).list);
    pthread_mutex_unlock(&(*list).mutex);
}

/**
 * Finish up the monitor by broadcasting to all waiting threads
 */
void tsb_finishUp(struct tsb_list * list) {
    pthread_mutex_lock(&(*list).mutex);
    printList((*list).list);
    (*list).stop_requested = TRUE;
    pthread_cond_broadcast(&list->listNotEmpty);
    pthread_mutex_unlock(&(*list).mutex);
    
}




















