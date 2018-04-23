#ifndef CHAT_H
#define CHAT_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include "chat.h"

#ifndef PORT
  #define PORT 8888
#endif

#define MAX_NAME 25     // Max username and profile_pic filename lengths
#define MAX_FRIENDS 10  // Max number of friends a user can have
#define BUFFER_SIZE 1024
#define NAME 0
#define COMMAND 1

#define INPUT_ARG_MAX_NUM 10
#define DELIM " \n"

typedef struct client {
    int fd;
    int state; // either NAME or COMMAND
    char name[MAX_NAME];
    char buf[BUFFER_SIZE];  // each client has its own buffer
    int inbuf;              // and a personal count of current contents
    struct client *next;
}Client;


//system errors utility - print error and exit
void error(char *msg);

// sets up socket server - listen and bind 
//returns socket file descriptor to be used to accept new clients
int setup ();

//accept connection and calls addclient 
void newconnection(int listenfd);

//adds new client struct to the linked list specified by *top
void addclient(int fd, struct in_addr addr);

//handles activity of client pointed to by *p
void handle(struct client *p);

//if the full command was identified - perform this user command
void do_command(struct client *p);

// Read and process user command argumants
// Return:  	-1 for quit command
//         		0 otherwise
int process_args(struct client * p, int cmd_argc, char **cmd_argv);

//writes the message to filedescriptor in the fd field of client
void sendclient(Client *p, char *msg);

//Return a dynamically allocated string containing the list of 
//all user names in the list of connected clients.
//One name per line per line.
char *list_users(Client *clientlist);

// Private message from one user to another
// If there is a user connected with this user_name, then deliver this post to them. 
void deliver_post(Client * autor_clientp, char *message, char *user_name);

// From the list of clients pointed to by *top, delete the client
// with the given user name.
void removeclient(int fd);

//Search the first inbuf characters of buf for a network newline.
//Return the location of the '\r' if the network newline if found, or -1 otherwise.
int find_network_newline(char *buf, int inbuf); 

//called from user server handle() to tokenize the command 
//and call appropriate  handling command command. 
//return 0 on success, 1 on failure
int tokenize_line(struct client *p);

#endif