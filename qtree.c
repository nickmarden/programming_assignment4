#include "qtree.h"

QNode *add_next_level (QNode *current, Node *list_node) {
    int str_len;

    str_len = strlen(list_node->str);
    current = (QNode*)calloc(1, sizeof(QNode));

    current->question =  (char *) calloc (str_len +1, sizeof(char ));
    strcpy(current->question, list_node->str);
    current->node_type = REGULAR;

    if (list_node->next == NULL) {
        current->node_type = LEAF;
        return current;
    }

    current->children[0].qchild = add_next_level ( current->children[0].qchild, list_node->next);
    current->children[1].qchild = add_next_level ( current->children[1].qchild, list_node->next);

    return current;
}

void print_qtree (QNode *parent, int level) {
    int i;
    for (i=0; i<level; i++)
        printf("\t");

    printf ("%s type:%d\n", parent->question, parent->node_type);
    if(parent->node_type == REGULAR) {
        print_qtree (parent->children[0].qchild, level+1);
        print_qtree (parent->children[1].qchild, level+1);
    }
    else { //leaf node
        for (i=0; i<(level+1); i++)
            printf("\t");
        print_users (parent->children[0].fchild, "q");
        for (i=0; i<(level+1); i++)
            printf("\t");
        print_users (parent->children[1].fchild, "q");
    }
}

void print_users (Node *parent, char* exceptfor) {
    if (parent == NULL)
        printf("NULL\n");
    else {
        while(parent != NULL) {
            if(strcmp(parent->str, exceptfor)) {
                printf("%s, ", parent->str);
            }
            parent = parent->next;     
        }
        printf("\n");
    }
}

int validate_username(char* username){
    // more than 8 characters, but at most 24
    // all characters must be alphanumeric
    if(strlen(username) <8){
        return 0;
    }
    if(strlen(username) > 24){
        return 0;
    }
    if(strspn(username, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != strlen(username)){
        return 0;
    }
    return 1;
}

int length_of_list(Node* head){
    int count = 0;
    while(head != NULL){
        count++;
        head = head->next;
    }
    return count;
}

Node* interest_group_for_user(QNode* root, char* username){
    int answer;
    Node* current_interest_group;
    for (answer = 0; answer < 2; answer++) {
        if (root->node_type == REGULAR) {
            current_interest_group = interest_group_for_user(root->children[answer].qchild, username);
            if (current_interest_group != NULL) {
                return current_interest_group;
            }
        } else {
            current_interest_group = root->children[answer].fchild;
            while (current_interest_group != NULL) {
                if (!strcmp(current_interest_group->str, username)) {
                    return root->children[answer].fchild;
                }
                current_interest_group = current_interest_group->next;
            }
        }
    }
    return current_interest_group;
}

Node* opposite_interest_group_for_user(QNode* root, char* username){
    int answer;
    Node* current_interest_group;
    for (answer = 0; answer < 2; answer++) {
        int not_answer = abs(answer-1);
        if (root->node_type == REGULAR) {
            current_interest_group = opposite_interest_group_for_user(root->children[not_answer].qchild, username);
            if (current_interest_group != NULL) {
                return current_interest_group;
            }
        } else {
            current_interest_group = root->children[not_answer].fchild;
            while (current_interest_group != NULL) {
                if (!strcmp(current_interest_group->str, username)) {
                    return root->children[not_answer].fchild;
                }
                current_interest_group = current_interest_group->next;
            }
        }
    }
    return current_interest_group;
}

void free_qnode_recursive(QNode* qnode) {
    int ans;
    for(ans = 0; ans < 2; ans++) {
        if(qnode->node_type == REGULAR) { 
            free_qnode_recursive(qnode->children[ans].qchild);
        } else {
                free_list(qnode->children[ans].fchild);
            }
        }
    free(qnode->question);
    free(qnode);
    }
