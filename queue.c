//
// Created by dcohe on 08/06/2021.
//

#include "queue.h"

Node *reqQueueHead = NULL;

void enqueue(Request req) {
    Node *node = malloc(sizeof(Node));
    if(node == NULL) app_error("Memory Allocation Error");
    node->request = req;
    pthread_mutex_lock(&m);
    node->next = reqQueueHead;
    reqQueueHead = node;
    queueSize++;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}

Request dequeue() {
    pthread_mutex_lock(&m);
    while(queueSize == 0){
        pthread_cond_wait(&c, &m);
    }
    Node *it = reqQueueHead;
    Node *prev = NULL;
    for(; it->next != NULL; prev = it, it = it->next);
    if(reqQueueHead == it){
        reqQueueHead = NULL;
    } else {
        prev->next = NULL;
    }

    queueSize--;
    pthread_mutex_unlock(&m);
    Request req = it->request;
    free(it);
    return req;
}

