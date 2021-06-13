//
// Created by dcohe on 08/06/2021.
//
#include "queue.h"

Node *reqQueueHead = NULL;
Node *reqQueueTail = NULL;
pthread_cond_t c1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t c2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int queueSize = 0;

 struct timeval getCurrentTime(void){
    struct timeval tv;
    if(gettimeofday(&tv, NULL)){
        unix_error("Time Of Day Error");
    }
    return tv;
}

struct timeval timeval_sub(struct timeval x, struct timeval y){
     struct timeval res;
    if (x.tv_usec < y.tv_usec) {
        int nsec = (y.tv_usec - x.tv_usec) / 1000000 + 1;
        y.tv_usec -= 1000000 * nsec;
        y.tv_sec += nsec;
    }
    if (x.tv_usec - y.tv_usec > 1000000) {
        int nsec = (x.tv_usec - y.tv_usec) / 1000000;
        y.tv_usec += 1000000 * nsec;
        y.tv_sec -= nsec;
    }

    res.tv_sec = x.tv_sec - y.tv_sec;
    res.tv_usec = x.tv_usec - y.tv_usec;

    return res;
 }


void enqueue(Request req, char *schedalg) {
    Node *node = malloc(sizeof(Node));
    if(node == NULL) app_error("Memory Allocation Error");
    node->request = req;
    node->next = NULL;
    pthread_mutex_lock(&m);
    if(buffAvailable == 0) {
        if (strcmp(schedalg, "block") == 0) {
            while (buffAvailable == 0) {
                pthread_cond_wait(&c2, &m);
            }
        } else if (strcmp(schedalg, "dt") == 0) {
            Close(req.connfd);
            return;
        } else if (strcmp(schedalg, "dh") == 0) {
            Node *temp = reqQueueHead->next;
            Close(temp->request.connfd);
            reqQueueHead->next = temp->next;
            if(temp->next == NULL) reqQueueTail = reqQueueHead;
            free(temp);
        } else {
            int numToDrop = (queueSize + 3) / 4;
            int *toDrop = malloc(sizeof(int) * numToDrop);
            if(toDrop == NULL) app_error("Memory Allocation Error");
            for(int i = 0; i < numToDrop; i++){
                int goodR = 1;
                int r;
                do{
                    r = rand() % numToDrop + 1;
                    for(int j = 0; j != i; j++){
                        if(toDrop[j] == r){
                            goodR = 0;
                            break;
                        }
                    }
                } while(!goodR);
                toDrop[i] = r;
            }

            for(int i = 0; i < numToDrop; i++){
                Node *it = reqQueueHead;
                Node *prev = reqQueueHead;
                for(int j = 0; j < toDrop[i]; j++){
                    prev = it;
                    it = it->next;
                }
                prev->next = it->next;
                if(reqQueueTail == it) reqQueueTail = prev;
                Close(it->request.connfd);
                free(it);
            }



            Node *temp = reqQueueHead->next;
            Close(temp->request.connfd);
            reqQueueHead->next = temp->next;
            if(temp->next == NULL) reqQueueTail = reqQueueHead;
            free(temp);
        }
    }
    if(reqQueueHead == NULL){
        reqQueueHead = malloc(sizeof(Node));
        if(reqQueueHead == NULL) app_error("Memory Allocation Error");
        reqQueueHead->request.connfd = -1;
        reqQueueHead->next = NULL;
        reqQueueTail = reqQueueHead;
    }

    reqQueueTail->next = node;
    reqQueueTail = node;
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

    Node *node = reqQueueHead->next;
    reqQueueHead->next = node->next;
    if(reqQueueHead->next == NULL) reqQueueTail = reqQueueHead;

    queueSize--;
    pthread_mutex_unlock(&m);
    Request req = node->request;
    free(node);
    return req;
}

