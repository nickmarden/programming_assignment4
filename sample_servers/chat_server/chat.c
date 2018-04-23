//#define _GNU_SOURCE 
#include "chat.h"

// this is the head of the linked list 
// of all clients currently connected
Client *top = NULL;

int main() {
    struct client *p;
    int listenfd = setup();    
    
    while (1) {
        fd_set fdlist;
        int maxfd = listenfd;
        FD_ZERO(&fdlist);
        FD_SET(listenfd, &fdlist);
        for (p = top; p; p = p->next) {
            FD_SET(p->fd, &fdlist);
            if (p->fd > maxfd)
                maxfd = p->fd;
        }
        if (select(maxfd + 1, &fdlist, NULL, NULL, NULL) < 0) {
            perror("select");
        } else {
            for (p = top; p; p = p->next)
                if (FD_ISSET(p->fd, &fdlist))
                    break;
            if (p)
                handle(p);  
            if (FD_ISSET(listenfd, &fdlist))
                newconnection(listenfd);
        }
    }
    return(0);
}

//accept connection and put new client into linked list 
void newconnection(int listenfd)  {
    int fd;
    struct sockaddr_in r;
    socklen_t socklen = sizeof(r);

    if ((fd = accept(listenfd, (struct sockaddr *)&r, &socklen)) < 0) {
        error("accept");
    } 
    
    printf("connection from %s\n", inet_ntoa(r.sin_addr));
    addclient(fd, r.sin_addr); 
}

//adds new client struct to the linked list specified by *top
void addclient(int fd, struct in_addr addr){
    struct client *p = calloc(sizeof(struct client),1);
    if (!p) {
        error("malloc failure");
    }
    printf("Adding client %s\n", inet_ntoa(addr));
    fflush(stdout);
    p->fd = fd;
    p->state = NAME;
    p->inbuf = 0;
    p->next = top;
    top = p;

    sendclient(p, "What is your user name?\n");
}


//handles activity of client pointed to by *p
//either client wrote something or disconnected
void handle(struct client *p) {
    char *after = p->buf + p->inbuf;
    int room = BUFFER_SIZE - p->inbuf;
    int nbytes;
    if ((nbytes = read(p->fd, after, room)) > 0) {
        p->inbuf += nbytes;
        int where = find_network_newline(p->buf, p->inbuf);
        if (where >= 0) {
            p->buf[where] = '\0';
            p->buf[where+1] = '\0';
            //fprintf (stderr, "Received message: %s\n", p->buf);
            //fflush(stderr);			
            do_command(p);
            where+=2;  // skip over \r\n
            p->inbuf -= where;
            memmove(p->buf, p->buf + where, p->inbuf);
        }
        room = sizeof(p->buf) - p->inbuf;
        after = p->buf + p->inbuf;
    } else {
        removeclient(p->fd);
    }
}

//if the full command was identified - perform this user command
void do_command(struct client *p) {    
    if (p->state == NAME) {
        strcpy(p->name, p->buf);
		p->state = COMMAND;  //now user identified himself - can perform commands
		fprintf(stderr, "Set user name for file descriptor %d to %s\n", p->fd, p->name);
		fflush(stderr);
        sendclient(p, "Welcome. Go ahead and type [list_users], [post to_user msg] or [quit]\r\n");        
    } else if (!strcmp(p->buf, "quit")) {
        removeclient(p->fd);
    } else {
        tokenize_line(p);
    }
}

// Read and process user commands
// Return:  	-1 for quit command
//         		0 otherwise
int process_args(struct client * p, int cmd_argc, char **cmd_argv) {
	int i;
    if (cmd_argc <= 0) {
        return 0;
    } else if (strcmp(cmd_argv[0], "quit") == 0 && cmd_argc == 1) {
        return -1;
    } else if (strcmp(cmd_argv[0], "list_users") == 0 && cmd_argc == 1) {
        char *buf; 
        buf = list_users(top);
        sendclient(p, buf);
        // memory dynamically allocated inside list_users, finished with it now
        free(buf); 
    } else if (strcmp(cmd_argv[0], "post") == 0 && cmd_argc >= 3) {
        char header [15+MAX_NAME];
        sprintf(header,"Message from %s: ", p->name);
        int space_needed = 15+MAX_NAME;
        for (i = 2; i < cmd_argc; i++) {
            space_needed += strlen(cmd_argv[i]) + 1;
        }        
        
        // allocate the space
        char *buf = malloc(space_needed);
        strcpy(buf, header);
        buf[strlen(header)]='\0';
        for (i = 2; i < cmd_argc; i++) {
            strcat(buf, " ");
            strcat(buf, cmd_argv[i]);
        }
        strcat (buf, "\r\n");
        
        deliver_post(p, buf, cmd_argv[1]);
		free(buf);
    }    
    else {
        sendclient(p, "Invalid command syntax\r\n");
    }
    return 0;
}

// Private message from one user to another
// If there is a user connected with this user_name, then deliver this post to them. 
void deliver_post(Client * autor_clientp, char *message, char *user_name) {   
   //search if target client is online (in the top user list)
    struct client *p;
    int found = 0;
    for (p = top; p!=NULL; p = p->next) {      
        if (strcmp(p->name, user_name) == 0) {
            found =1;
            sendclient(p, message);
			return;
        }
    }
   
   if(!found)
       sendclient(autor_clientp, "Cannot deliver your message. Target user is not online\r\n");
}

// From the list of clients pointed to by *top, delete the client
// with the given user name.
// if user with this name does not exist, prints message
void removeclient(int fd) {
    struct client **p;
    for (p = &top; *p && (*p)->fd != fd; p = &(*p)->next)
        ;
    if (*p) {
        struct client *t = (*p)->next;
        fprintf(stderr, "Removing client %s\n", (*p)->name);
        fflush(stderr);
        free(*p);
        if (close(fd) != 0) {
            error("closing fd on exiting client");
        }
        *p = t;
    } else {
        fprintf(stderr, "Trying to remove fd %d, but I don't know about it\n", fd);
        fflush(stderr);
    }
}





