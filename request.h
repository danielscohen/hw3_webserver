#ifndef __REQUEST_H__

void requestHandle(Request req, WorkerThread *wThreadData);

void requestReadhdrs(rio_t *rp);

#endif
