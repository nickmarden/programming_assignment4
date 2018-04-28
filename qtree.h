#ifndef QTREE_H
#define QTREE_H
#include "questions.h"

typedef enum {
    REGULAR, LEAF
} NodeType;

union Child {
	struct str_node *fchild;
	struct QNode *qchild;
} Child;

typedef struct QNode {
	char *question;
	NodeType node_type;
	union Child children[2];
} QNode;

QNode* add_next_level (QNode* current, Node* list_node);

Node* interest_group_for_user(QNode* root, char* username);
Node* opposite_interest_group_for_user(QNode* root, char* username);

int length_of_list(Node* head);
int validate_username(char *username);
void print_qtree (QNode *parent, int level);
void print_users (Node *parent, char* exceptfor);
void free_qnode_recursive (QNode* qnode);
#endif
