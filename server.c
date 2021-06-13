#include "segel.h"
#include "request.h"
#include "queue.h"

// 
// server.c1: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c1
//

// HW3: Parse the new arguments too



void getargs(int *port, int *threads, int *queueSize, char *schedalg, int argc, char *argv[])
{
    if (argc < 5) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *threads = atoi(argv[2]);
    *queueSize = atoi(argv[3]);
    strcpy(schedalg, argv[4]);
}


int main(int argc, char *argv[])
{
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    int listenfd, connfd, port, clientlen, numThreads;
    char schedalg[MAXLINE];
    struct sockaddr_in clientaddr;

    getargs(&port, &numThreads, &buffAvailable, schedalg, argc, argv);

    // 
    // HW3: Create some numThreads...
    //
    pthread_t *threads = malloc(sizeof (pthread_t) * numThreads);
    for(int i = 0; i < numThreads; i++){
        int res = pthread_create(&threads[i], NULL, workerThreadMain, NULL);
        if(res) posix_error(res, "Posix Error!!");
    }


    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
	printf("accepted\n");
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    Rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);
    requestReadhdrs(&rio);
    Request request;
    strcpy(request.method, method);
    strcpy(request.uri, uri);
    strcpy(request.version, version);
    request.connfd = connfd;
    enqueue(request, NULL);

	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker numThreads
	// do the work. 
	// 

    }

}


    


 
