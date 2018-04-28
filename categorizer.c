#include "qtree.h"

void print_greetings();
int ask_question(char* category_of_interest);
int read_next_user(QNode* root);
int is_user_trying_to_quit(char* user_input);

int main (int argc, char **argv) {

    QNode *root = NULL;	

    Node * interests = NULL;

    if (argc < 2) {
        printf ("To run the program ./categorizer <name of input file>\n");
        return 1;
    }

    interests = get_list_from_file ( argv[1]);

    if (interests == NULL)
        return 1;

    root = add_next_level (root,  interests);
    print_greetings();

    //main application loop
    int reading_error = 0;
    while(!reading_error) {
        reading_error = read_next_user(root);
    }

    print_qtree(root, 0);
    printf("\n");
    free_list (interests);
    free_qnode_recursive(root);
    return 0;
}

void print_greetings () {
    printf ("----------------------------------------------\n");
    printf ("Find and befriend people who share your oddly specific interests!\n");
    printf ("----------------------------------------------\n");
}

int ask_question(char* category_of_interest) {
    char buffer[MAX_LINE];	
    int answer = 2;
    while(answer == 2) {
        printf("Do you like %s? ", category_of_interest);
        scanf("%s", buffer); // How to avoid buffer overflow of answer?

        if (strlen(buffer) > 4) {
            printf("Answer must contain between 1 and 4 characters.\n");
        } else if (is_user_trying_to_quit(buffer)) {
            answer = -1;
        } else if ((buffer[0] == 'Y') || (buffer[0] == 'y')) {
            answer = 1;
        } else if ((buffer[0] == 'N') || (buffer[0] == 'n')) {
            answer = 0;
        } else {
            printf("Answer must begin with Y/y, N/n, or Q/q.\n");
        }
    }
    return answer;
}

int read_next_user(QNode* root) {
    char name[MAX_LINE];

    // Ask the user for their name
    int is_valid_username = 0;
    while(!is_valid_username) {
        printf("What is your name? ");
        scanf("%s", name);

        if(is_user_trying_to_quit(name)) {
            return 1;
        } else if(validate_username(name)){
            is_valid_username = 1;
        } else {
            printf("Username must contain between 8 and 24 alphanumeric characters. Try again. \n");
        }
    }

    // Check if user exists
    // use a for loop to check all possible friend lists
    
    Node* friend_group = opposite_interest_group_for_user(root, name);
    while(friend_group != NULL) {
        if(strcmp(friend_group->str, name)) {
            printf("You might want to meet ");
            print_users(friend_group, name);
        } else {
            printf("No one else is here, try again later! \n");
        }
        friend_group = NULL;
        return 0;
    }

    // Ask user questions
    while(root != NULL){   
        int ans = ask_question(root->question);
        if(ans == -1) {
            return 2;
        }
        Node* friends;
        if(root->node_type == LEAF){
            friends = root->children[ans].fchild;
            if(friends == NULL) {
                friends = (Node*)calloc(1, sizeof(struct str_node));
                friends->str = (char*)calloc(strlen(name)+1, sizeof(char));
                strcpy(friends->str, name);
                root->children[ans].fchild = friends;
                printf("Sorry, you are alone in the world.\n");
            } else{
                // Move to the end of the current friends list for this answer leaf node
                while(friends->next != NULL) {
                    friends = friends->next;
                }

                // Create a new friend
                struct str_node* next_friend = (struct str_node*)calloc(1, sizeof(struct str_node));
                next_friend->str = (char*)calloc(strlen(name)+1, sizeof(char));
                strcpy(next_friend->str, name);

                // Add the friend to the end of the friends list
                friends->next = next_friend;

                printf("You might want to meet ");
                print_users(root->children[ans].fchild, name);
                printf("\n");
            } 
            root = NULL;
        } else{
            root = root->children[ans].qchild;
        } 
    }
    free(root);
    return 0;
}

int is_user_trying_to_quit(char* user_input) {
    return ((strlen(user_input) <= 4) && (user_input[0] == 'Q' || user_input[0] == 'q'));
}
