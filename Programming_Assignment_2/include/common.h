#ifndef COMMON_INCLUDED
#define COMMON_INCLUDED

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define QUEUE_SIZE 50
#define BUFFER_SIZE 20
#define PAYLOAD_SIZE 512

/* SBCP Protocol Version */
#define PROTOCOL_VERSION 3

/* Header type */
#define JOIN 2
#define SEND 4
#define FWD 3
#define ACK 7
#define NAK 5
#define ONLINE 8
#define OFFLINE 6
#define IDLE 9

/* Stdin File descriptor */
#define STDIN_FD 0

/* Attributes type */
#define REASON 1
#define USERNAME 2
#define CLIENT_COUNT 3
#define MESSAGE 4

#define IDLE_WAIT_TIME 10

#define DEBUG_CLIENT_MSG(...)                                       \
    do                                                                  \
{                                                                   \
    fprintf (stderr, "CLIENT MSG:: PID : %d, %s:%d:\t", getpid (),  __func__, __LINE__);      \
    fprintf (stderr, ##__VA_ARGS__);                                               \
    fprintf (stderr, "\n");                                                 \
    fflush (stderr);                                                        \
}                                                                   \
while (0); \

#define DEBUG_SERVER_MSG(...)                                       \
    do                                                                  \
{                                                                   \
    fprintf (stderr, "SERVER MSG:: PID : %d, %s:%d:\t", getpid (),  __func__, __LINE__);      \
    fprintf (stderr, ##__VA_ARGS__);                                               \
    fprintf (stderr, "\n");                                                 \
    fflush (stderr);                                                        \
}                                                                   \
while (0); \

/* SBCP message header format */
typedef struct
{
    unsigned int uiVrsn:9;
    unsigned int uiType:7;
    unsigned int uiLength:16;
}sbcp_header_t;

/* SBCP message attribute formate */
typedef struct
{
    unsigned int uiType:16;
    unsigned int uiLength:16;
    char         acPayload[PAYLOAD_SIZE];
}sbcp_attribute_t;

/* SBCP message structure */
typedef struct
{
    sbcp_header_t    sMsgHeader;
    sbcp_attribute_t sMsgAttribute;
}sbcp_message_t;

// User data
struct user_data{
    char user_name[16];
    int socket_fd;
    int user_number;
};


int create_socket(bool isIPv4);
void set_server_address(struct sockaddr_in *server_address, char * ip, int port);
void set_server_address_ipv6(struct sockaddr_in6 *server_address, char * ip, int port);
void bind_server(int socket_fd, struct sockaddr_in server_address);
void bind_server_ipv6(int socket_fd, struct sockaddr_in6 server_address);
void start_listening(int socket_fd);

// My functions
int accept_connection(struct sockaddr_in * client_addresses, int client_count, int socket_fd);
int join_message_process(int new_client_fd, int * client_count, int max_client, struct user_data * clients);
void send_nack_message(int new_client_fd, int client_count, struct user_data * clients, int code);
void send_ack_message(int new_client_fd, int client_count, struct user_data * clients);
int check_name(char * name, int client_count, int max_client, struct user_data * clients);
void broadcast_message(int listening_fd, int socket_fd, struct user_data * clients, int max_fd, fd_set * set1, int *client_count);
sbcp_message_t * get_join_message(char * new_user_name);
sbcp_message_t * get_hung_message(char * new_user_name);
void vSendIdleMessage (void *arg);

#endif // COMMON_INCLUDED
