#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <unistd.h>

#define PORT 1337
#define BACKLOG 10
#define BUFSIZE 1024

int main() {

    int sockfd, new_fd;              // listening on sock_fd, new connections on new_fd
    struct sockaddr_in server_addr;  // server address information
    struct sockaddr_in client_addr;  // client address information
    char buf[BUFSIZE];               // the buffer to read the incoming packet
    int msg_byte_size;

    socklen_t sin_size = sizeof(struct sockaddr_in); // size of socket address

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family      = AF_INET;     // host/server byte order
    server_addr.sin_port        = htons(PORT); // short, network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY;  // auto-fill with my IP addr
    bzero(&(server_addr.sin_zero), 8);         // zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    // main accept() loop
    while(1) {

        // accept a connection here
        if ((new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }

        printf("server: got connection from %s\n", inet_ntoa(client_addr.sin_addr));

        // read the input string from the client
        bzero(buf, BUFSIZE);
        msg_byte_size = read(new_fd, buf, BUFSIZE);
        if (msg_byte_size < 0) {
            perror("ERROR reading from socket");
            continue;
        }
        printf("server received %d bytes: %s", msg_byte_size, buf);

        if (send(new_fd, "Hello, world!\n", 14, 0) == -1) {
            perror("send");
            close(new_fd);
            exit(0);
        }
    }

    return 0;
}