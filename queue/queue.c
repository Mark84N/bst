#include <errno.h>
#include "queue.h"

void __queue_add(struct queue *q, struct nodewrap *nw)
{
    if (q->qlen == 0) {
        q->qhead = q->qtail = nw;
    } else  {
        q->qtail->prev = nw;
        q->qtail = nw;
    }
    q->qlen++;
}

struct nodewrap *__queue_pop(struct queue *q)
{
    struct nodewrap *nw = q->qhead;
    q->qhead = nw->prev;
    q->qlen--;
    return nw;
}

struct queue *create_queue(void)
{
    return calloc(1, sizeof(struct queue));
}

void destroy_queue(struct queue *q)
{
    while (q->qlen != 0)
        free(__queue_pop(q));
    free(q);
}

int enqueue(struct queue *q, struct node *n)
{
    struct nodewrap *nw = calloc(1, sizeof(struct nodewrap));
    if (!nw)
        return ENOMEM;

    nw->node = n;
    __queue_add(q, nw);

    return 0;
}

struct node *dequeue(struct queue *q)
{
    struct node *n = NULL;
    struct nodewrap *nw = NULL;

    if (q->qlen == 0)
        return NULL;

    nw = __queue_pop(q);
    n = nw->node;
    free(nw);

    return n;
}
