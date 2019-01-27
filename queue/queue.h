#ifndef __QUEUE__H__
#define __QUEUE__H__

#include <stdlib.h>

#define offsetof(field, type) \
			(unsigned long)&(((type *)0)->field)

#define container_of(ptr, field, type) \
			(type *)(ptr - offsetof(field, type))

struct nodewrap {
    struct nodewrap *prev;
    struct node *node;
};

struct queue {
    struct nodewrap *qhead;
    struct nodewrap *qtail;
    unsigned int qlen;
};

struct queue *create_queue(void);
void destroy_queue(struct queue *q);
int enqueue(struct queue *q, struct node *n);
struct node *dequeue(struct queue *q);

#endif /* __QUEUE__H__ */
