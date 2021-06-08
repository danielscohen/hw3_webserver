//
// Created by dcohe on 08/06/2021.
//

#ifndef HW3_WEBSERVER_QUEUE_H
#define HW3_WEBSERVER_QUEUE_H

#include "segel.h"

typedef struct Node {
    Request request;
    struct Node *next;
} Node;

Node *reqQueueHead = NULL;

void enqueue(Request req);
Request dequeue();

#endif //HW3_WEBSERVER_QUEUE_H
