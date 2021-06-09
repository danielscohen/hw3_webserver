//
// Created by dcohe on 08/06/2021.
//
#include "queue.h"

Node *reqQueueHead = NULL;
pthread_cond_t c1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t c2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int queueSize = 0;

void enqueue(Request req) {
    Node *node = malloc(sizeof(Node));
    if(node == NULL) app_error("Memory Allocation Error");
    node->request = req;
    pthread_mutex_lock(&m);
    while(buffAvailable == 0){
        pthread_cond_wait(&c2, &m);
    }
    node->next = reqQueueHead;
    reqQueueHead = node;
    queueSize++;
    buffAvailable--;
    pthread_cond_signal(&c1);
    pthread_mutex_unlock(&m);
}

Request dequeue() {
    pthread_mutex_lock(&m);
    while(queueSize == 0){
        pthread_cond_wait(&c1, &m);
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

