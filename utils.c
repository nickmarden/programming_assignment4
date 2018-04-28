#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/* 
 * Print a formatted error message to stderr.
 */
void error(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}

/* 
 * Read and process commands
 */
int process_args(int command_line_argc, char **command_line_argv, QNode **root, Node *interests,
				 struct client *current_client, struct client *head) {
	QNode *qtree = *root;

	if (command_line_argc <= 0) {
		return 0;
	} else if (strcmp(command_line_argv[0], "quit") == 0 && command_line_argc == 1) {
		/* Return an appropriate value to denote that the specified
		 * user is now disconnected. */
		return -1;
		
	} else if (strcmp(command_line_argv[0], "do_test") == 0 && command_line_argc == 1) {
		/* The specified user is ready to start answering questions. You
		 * need to make sure that the user answers each question only
		 * once.
		 */
		
	} else if (strcmp(command_line_argv[0], "get_all") == 0 && command_line_argc == 1) {
		/* Return the list of best mismatches related to the specified
		 * user. If the user has not taked the test yet, return the
		 * corresponding error value (different than 0 and -1).
		 */
		 
	} else if (strcmp(command_line_argv[0], "post") == 0 && command_line_argc == 3) {
		/* Send the specified message stored in command_line_argv[1] to the user
		 * stored in command_line_argv[2].
		 */
	}
	else {
		/* The input message is not properly formatted. */
		error("Incorrect syntax");
	}
	return 0;
}

/*
 * Tokenize the string stored in command_line.
 * Return the number of tokens, and store the tokens in command_line_argv.
 */
int tokenize(char *command_line, char **command_line_argv) {
    int command_line_argc = 0;
    char *next_token = strtok(command_line, DELIM);    
    while (next_token != NULL) {
        if (command_line_argc >= INPUT_ARG_MAX_NUM - 1) {
            error("Too many arguments!");
            command_line_argc = 0;
            break;
        }
        command_line_argv[command_line_argc] = next_token;
        command_line_argc++;
        next_token = strtok(NULL, DELIM);
    }

    return command_line_argc;
}
