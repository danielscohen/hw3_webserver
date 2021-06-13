//
// Created by dcohe on 08/06/2021.
//

#ifndef HW3_WEBSERVER_QUEUE_H
#define HW3_WEBSERVER_QUEUE_H

#include "segel.h"

#define MAXLINE  8192  /* max text line length */

typedef struct Request{
    int connfd;
    char method[MAXLINE];
    char uri[MAXLINE];
    char version[MAXLINE];
    struct timeval arrivalTime;
    struct timeval dispatchInt;
} Request;

typedef struct Node {
    Request request;
    struct Node *next;
} Node;

typedef struct WorkerThread{
    int id;
    pthread_t threadId;
    int threadCount;
    int staticCount;
    int dynamicCount;
} WorkerThread;

WorkerThread *workerThreads;
int numWorkerThreads;

Node *reqQueueHead;
Node *reqQueueTail;

void enqueue(Request req, char *schedalg);
Request dequeue();

pthread_cond_t c1;
pthread_cond_t c2;
pthread_mutex_t m;
int queueSize;
int buffAvailable;

struct timeval getCurrentTime(void);
struct timeval timeval_sub(struct timeval, struct timeval);
#endif //HW3_WEBSERVER_QUEUE_H
