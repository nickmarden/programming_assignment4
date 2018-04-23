#ifndef _UTILS_H
#define _UTILS_H

#include <netinet/in.h>
#include "qtree.h"

#define MAX_NAME 128
#define INPUT_ARG_MAX_NUM 3
#define DELIM " \r\n"

/*
 * Τhe definition of the following structure is copied directly from the
 * muffinman server (http://www.cdf.toronto.edu/~ajr/209/a4/muffinman.c).
 * You are free to add more fields to the struct's definition; in fact,
 * it is strongly recommended.
 */
struct client {
    int fd;
    struct in_addr ipaddr;
    char username[MAX_NAME];
    struct client *next;
};

/* 
 * Print a formatted error message to stderr.
 */
void error(char *);

/* 
 * Read and process commands
 */
int process_args(int, char **, QNode **, Node *, struct client *, struct client *);


/*
 * Tokenize the string stored in cmd.
 * Return the number of tokens, and store the tokens in cmd_argv.
 */
int tokenize(char *, char **);

#endif /* _UTILS_H */
