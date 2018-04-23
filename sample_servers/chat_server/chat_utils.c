#include "chat.h"

//system errors utility - print error and exit
void error(char *msg){
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(1);
}

//Search the first inbuf characters of buf for a network newline.
//Return the location of the '\r' if the network newline if found, or -1 otherwise.
int find_network_newline(char *buf, int inbuf) {
  int i;
  for (i = 0; i < inbuf - 1; i++)
    if ((buf[i] == '\r') && (buf[i + 1] == '\n'))
      return i; 
  return -1;
}


// sets up socket server - listen and bind 
//returns socket file descriptor to be used to accept new clients
int setup () {
  int on = 1, status;
  struct sockaddr_in self;
  int listenfd;  
 
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    error("socket");
  }

  status = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                      (const char *) &on, sizeof(on));
  if(status == -1) {
    error("setsockopt -- REUSEADDR");
  }

  memset(&self, '\0', sizeof(self));
  self.sin_family = AF_INET;
  self.sin_addr.s_addr = INADDR_ANY;
  self.sin_port = htons(PORT);
  printf("Listening on %d\n", PORT);

  if (bind(listenfd, (struct sockaddr *)&self, sizeof(self)) == -1) {
    error("bind"); // probably means port is in use
  }

  if (listen(listenfd, 10) == -1) {
    error("listen");
  }
  
  return listenfd;
}

//sends message to the client, ignores if not successful
void sendclient(struct client *p, char *msg) {    
    int res =  write(p->fd, msg, strlen(msg));
    if (res <=0) printf("Message dropped\n");
}

//called from user server  handle() to tokenize the command 
//and call appropriate  user command. 
//return 0 on success, 1 on failure
int tokenize_line(struct client *p) {
    char * input = p->buf;
    // for holding arguments to individual commands passed to sub-procedure
    char *cmd_argv[INPUT_ARG_MAX_NUM];
    int cmd_argc;

    // tokenize arguments
    char *next_token = strtok(input, DELIM);
    cmd_argc = 0;
    while (next_token != NULL) {
        if (cmd_argc >= INPUT_ARG_MAX_NUM - 1) {
            sendclient(p, "Too many command arguments!\n");
            return 1;
        }
        cmd_argv[cmd_argc] = next_token;
        cmd_argc++;
        next_token = strtok(NULL, DELIM);
    }
	
	if (cmd_argc == 0)
		return 1;
    return (process_args(p, cmd_argc, cmd_argv)); 
}

char *list_users(Client *clientlist){
	const Client *curr = clientlist;  
    // first work out the size you need
    int size = 1; // start with empty string so this is the null terminator
    while (curr != NULL) {
        size +=  strlen(curr->name) + 2 ;// each name gets a /r/n after it
        curr = curr->next;
    }
    // now malloc the space we need
    char *result = malloc(size);
    // now go over the datastructure again and build the string
    result[0] = '\0';
    curr = clientlist;
    while (curr != NULL) {
        strcat(result, curr->name);
        strcat(result,"\r\n");     // adds newline & terminator each time
        curr = curr->next;
    }
    return result;
}

