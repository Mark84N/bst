#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "bst/bst.h"

int main(int argc, char **argv)
{
    struct node *node1, *node2;
    struct tree *t;
    int id;

    t = calloc(1, sizeof(struct tree));
    if (!t)
        return ENOMEM;

    /* add root */
    tree_add_node(t, 10);

    /* add leaves */
    for (int i = 8, j = 12; i > 0; i -= 3, j += 3) {
        tree_add_node(t, i);
        tree_add_node(t, j);
        tree_add_node(t, i + 1);
        tree_add_node(t, j - 1);
        tree_add_node(t, j * 2);
    }

    /* handle duplicates */
    tree_add_node(t, 8);

    /* test node search */
    node1 = tree_find_node(t, 14);
    if (node1) {
        printf("Found node id=%d, parent id=%d\n", node1->id, node1->parent->id);
    } else {
        printf("Node not found\n");
    }

    /* test searching of sucessive */
    node2 = tree_find_successive(t, 9);
    if (node2) {
        printf("Node id=%d is successive of id=%d\n", node2->id, 9);
    }

    /* test searching for preceding */
    node2 = tree_find_preceding(t, 11);
    if (node2) {
        printf("Node id=%d is preceding of id=%d\n", node2->id, 11);
    }

    /* print before removal */
    tree_print(t);

    /* test removing */
    id = 5;
    if (tree_remove_node(t, id))
        printf("Node id=%d not found, failed to remove\n", id);
    else
        printf("Node id=%d removed\n", id);

    id = 5;
    if (tree_remove_node(t, id))
        printf("Node id=%d not found, failed to remove\n", id);
    else
        printf("Node id=%d removed\n", id);

    /* print after removal */
    tree_print(t);

    /* clean */
    tree_cleanup(t);
    free(t);

    return 0;
}

