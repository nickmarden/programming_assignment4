#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "questions.h"

Node* get_list_from_file (char* input_file_name) {
    char buffer[MAX_LINE];
	Node* head = NULL;
    Node* tail = NULL;
    FILE* input_file_handle = fopen(input_file_name, "r");
    if(input_file_handle == NULL) {
        return head;
    }
    while(fgets(buffer, MAX_LINE, input_file_handle) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->str = (char*)malloc((strlen(buffer)+1)*sizeof(char));
        strcpy(new_node->str, buffer); 
        new_node->next = (Node*) NULL;
        if(head == NULL) {
            head = new_node;
        } else {
            tail->next = new_node;
        }
        tail = new_node;
    }
    fclose(input_file_handle);
	return head;
}

void print_list (Node* head) {
    Node* ptr;
    for(ptr = head; ptr != NULL; ptr = ptr->next){
        printf("%s\n", ptr->str);
    }
}

void free_list (Node *head) {
    Node* ptr = head;
    while(ptr != NULL) {
        Node* next_copy = ptr->next;
        free(ptr->str);
        free(ptr);
        ptr = next_copy;
    }
}
