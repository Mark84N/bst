#include <stdlib.h>
#include <stdio.h>

#include "bst/bst.h"
#include "queue/queue.h"

/*
 * create_node() - Allocate a new node and assign id to it
 * @id: id of the new node
 *
 * Return: pointer to a new node or NULL on allocation fail
 * */
struct node *create_node(int id)
{
    struct node *n;

    if (!(n = calloc(1, sizeof(struct node))))
        return NULL;

    n->id = id;

    return n;
}

/*
 * __add_node() - Add a new node to a BST (auxiliary func, shouldn't be called directly)
 * @parent: pointer to the parent which will contain a new node
 * @new: a pointer to a new node
 *
 * Return: no
 * */
void __add_node(struct node *parent, struct node *new)
{
    if (new->id > parent->id) {
        if (parent->right) {
            __add_node(parent->right, new);
        } else {
            parent->right = new;
            new->parent = parent;
        }
    } else if (new->id < parent->id) {
        if (parent->left) {
            __add_node(parent->left, new);
        } else {
            parent->left = new;
            new->parent = parent;
        }
    } else {
        printf("Duplicates not allowed: id=%d\n", new->id);
    }
}

/*
 * tree_add_node() - Add a new node to a BST
 * @t: a pointer to a tree
 * @id: id of a new node
 *
 * Return: pointer to a new node or NULL
 * */
struct node *tree_add_node(struct tree *t, int id)
{
    struct node *new;

    new = create_node(id);
    if (!new)
        return new;

    if (t->root) {
        __add_node(t->root, new);
        if (new->parent)
            printf("Added node id=%d to parent id=%d\n", new->id, new->parent->id);
        else
            free(new);
    } else {
        t->root = new;
        printf("Added root id=%d\n", new->id);
    }

    t->size++;
    return new;
}

/*
 * tree_find_node() - Non-recursively find a node in a given BST by ID
 * @tree: pointer to a tree
 * @id: id of the wanted node
 *
 * Return: pointer to a wanted node or NULL if not found
 * */
struct node *tree_find_node(struct tree *t, int id)
{
    struct node *i = t->root;

    while (i) {
        if (id == i->id)
            break;

        if (id > i->id)
            i = i->right;
        else if (id < i->id)
            i = i->left;
    }

    return i && i->id == id? i : NULL;
}

/*
 * __cleanup_subtree() - Recursively cleanup a subtree of a given node (auxiliary func, shouldn't be called directly)
 * @node: pointer to a subtree root which is to be removed
 *
 * Return: no
 * */
void __cleanup_subtree(struct node *node)
{
    if (!node)
        return;

    __cleanup_subtree(node->left);
    __cleanup_subtree(node->right);
    printf("Cleaning node id=%d, parent=%d\n", node->id, node->parent? node->parent->id : -1);

    free(node);
}

/*
 * tree_cleanup() - Cleanup the entire tree
 * @tree: the tree to be removed
 *
 * Return: no
 * */
void tree_cleanup(struct tree *tree)
{
    __cleanup_subtree(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

/* subtree_find_max() - find max element in a subtree with a gived node as a root
 * @n: root of a given tree
 *
 * Return: max element of a subtree or a given node
 * */
static inline struct node *subtree_find_max(struct node *n)
{
    while (n->right)
        n = n->right;

    return n;
}

/* subtree_find_min() - find min element in a subtree with a gived node as a root
 * @n: root of a given subtree
 *
 * Return: min element of a subtree or a given node
 * */
static inline struct node *subtree_find_min(struct node *n)
{
    while (n->left)
        n = n->left;

    return n;
}

/*
 * subtree_find_successive() - Find successive element (the next key size) of provided id
 * @t: pointer to a tree
 * @id: id of an element which is preceding to successive
 *
 * Return: pointer to the successive element of a given id or NULL
 * */
struct node *tree_find_successive(struct tree *t, int id)
{
    struct node *parent, *tmp;

    if ((tmp = tree_find_node(t, id)) == NULL)
        return tmp;

    if (tmp->right && (tmp = subtree_find_min(tmp->right)))
        return tmp;

    parent = tmp->parent;

    while (parent && parent->left != tmp) {
        tmp = parent;
        parent = tmp->parent;
    }

    return parent;
}

/*
 * subtree_find_preceding() - Find preceding element (the next key size) of provided id
 * @t: pointer to a tree
 * @id: id of an element which is preceding to successive
 *
 * Return: pointer to the preceding element of a given id or NULL
 * */
struct node *tree_find_preceding(struct tree *t, int id)
{
    struct node *parent, *tmp;

    if ((tmp = tree_find_node(t, id)) == NULL)
        return tmp;

    if (tmp->left && (tmp = subtree_find_max(tmp->left)))
        return tmp;

    parent = tmp->parent;

    while (parent && parent->right != tmp) {
        tmp = parent;
        parent = tmp->parent;
    }

    return parent;
}

/*
 * tree_remove_node() - Remove a node and align existing structure
 * @t: pointer to a tree
 * @id: id to remove
 *
 * Return: 0 in success, 1 - node not found
 * */
int tree_remove_node(struct tree *t, int id)
{
    struct node *node, *parent;

    node = tree_find_node(t, id);
    if (!node)
        return 1;

    if (!node->left && !node->right)
        goto free;

    parent = node->parent;
    if (parent->right == node) {
        /* the node and ALL leaves are larger than the parent */
        struct node *max_left = subtree_find_max(node->left);
        if (!max_left) {
            /* no left nodes: append right subtree to a parent */
            parent->right = node->right;
            node->right->parent = parent;
        } else {
            /* left nodes: append right subtree to max node from the left subtree */
            parent->right = node->left;
            node->left->parent = parent;
            max_left->right = node->right;
            node->right->parent = max_left;
        }
    } else {
        /* node and leaves are smaller than the parent */
        struct node *min_right = subtree_find_min(node->right);
        if (!min_right) {
            /* no right subtree */
            parent->left = node->left;
            node->left->parent = parent;
        } else {
            /* right subtree  */
            parent->left = node->right;
            node->right->parent = parent;
            min_right->left = node->left;
            node->left->parent = min_right;
        }
    }
    t->size--;
free:
    free(node);

    return 0;
}

/*
 * tree_print() - Display a tree using BFS traversal
 * @t: pointer to a tree
 *
 * Return: no
 * */
void tree_print(struct tree *t)
{
    struct queue *q;
    struct node *n, *prev = NULL;

    if (!t->size)
        return;

    if (!(q = create_queue()))
        return;

    enqueue(q, t->root);

    do {
        n = dequeue(q);

        printf("%s[%d] ", (prev && prev->id > n->id)? "\n" : "", n->id);
        prev = n;

        if (n->left)
            enqueue(q, n->left);
        if (n->right)
            enqueue(q, n->right);
    } while (q->qlen);

    printf("\n");
    destroy_queue(q);
}