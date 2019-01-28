#ifndef __BST_H__
#define __BST_H__

struct node {
    struct node *parent;
    struct node *left;
    struct node *right;
    int id;
};

struct tree {
    struct node *root;
    int size;
};

struct node *create_node(int id);
void __add_node(struct node *parent, struct node *new);
struct node *tree_add_node(struct tree *t, int id);
int tree_remove_node(struct tree *t, int id);
void __cleanup_subtree(struct node *node);
void tree_cleanup(struct tree *tree);
struct node *tree_find_node(struct tree *t, int id);
struct node *tree_find_successive(struct tree *t, int id);
struct node *tree_find_preceding(struct tree *t, int id);
void tree_print(struct tree *t);



#endif