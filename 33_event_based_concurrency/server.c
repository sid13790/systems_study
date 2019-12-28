#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <unistd.h>

#include <aio.h>

#define PORT 1337
#define BACKLOG 10
#define BUFSIZE 1024
#define TRUE 1
#define FALSE 0
#define MAX_CLIENTS 30

int main()
{

    int opt = TRUE;
    int master_socket;
    int addrlen;
    int new_socket;
    int client_socket[MAX_CLIENTS];
    int activity;
    int valread;
    int sd;
    int max_sd;

    struct aiocb file_ops[MAX_CLIENTS];
    int aio_fds[MAX_CLIENTS];

    struct sockaddr_in address;

    char buffer[1025]; // data buffer of 1k
    char filebuffer[MAX_CLIENTS][10240];

    fd_set readfds;

    char *message = "ECHO Daemon v1.0 \r\n";

    // initialize all client_socket[] to 0 so not checked
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        client_socket[i] = 0;
    }

    // create a master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("creating master socket failed");
        exit(EXIT_FAILURE);
    }

    // set master socket to allow multiple connections
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        perror("setting master to allow multiple connections failed");
        exit(EXIT_FAILURE);
    }

    // type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind the socket to localhost 1337
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("binding master socket to port failed");
        exit(EXIT_FAILURE);
    }

    printf("Listener on port %d\n", PORT);

    // try to specify maximum of 10 pending connections for the master socket
    if (listen(master_socket, BACKLOG) < 0)
    {
        perror("failed to specify backlog with `listen`");
        exit(EXIT_FAILURE);
    }

    // accept the incoming connection
    while (TRUE)
    {
        // clear the socket set
        FD_ZERO(&readfds);

        // add the master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        // add child sockets to set
        for (int i = 0; i < MAX_CLIENTS; i++)
        {

            // socket descriptor
            sd = client_socket[i];

            // if valid socket descriptor then add to read list
            if (sd > 0)
            {
                FD_SET(sd, &readfds);
            }

            // highest file descriptor number, need it for select function
            if (sd > max_sd)
            {
                max_sd = sd;
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error\n");
        }

        /*
            If something happened on the master socket, then
            it's an incoming connection
        */
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accepting new incoming connection failed");
                exit(EXIT_FAILURE);
            }

            printf("[New connection] socket fd: %d, ip is %s, port: %d\n",
                   new_socket,
                   inet_ntoa(address.sin_addr),
                   ntohs(address.sin_port));

            if (send(new_socket, message, strlen(message), 0) != strlen(message))
            {
                perror("failed to send response to client");
            }

            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                // if position is empty
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds))
            {
                // check if it was for closing, also read the incoming message
                if ((valread = read(sd, buffer, 1024)) == 0)
                {
                    // someone disconnected, get their details and print
                    getpeername(sd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen);

                    printf("[Host disconnected] ip: %s, port: %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    // close the socket and mark as 0 in the list for reuse
                    close(sd);
                    client_socket[i] = 0;
                }

                // return file results
                else 
                {
                    if (file_ops[i].aio_fildes != NULL) {
                        if (aio_error(&file_ops[i]) == 0) {
                            if (send(sd, filebuffer[i], strlen(filebuffer[i]), 0) != strlen(filebuffer[i])) {
                                perror("failed to send message back to client");
                            }
                            bzero(filebuffer[i], 10240);
                            close(aio_fds[i]);
                            file_ops[i] = (struct aiocb) { 0 };
                        } else {
                            continue;
                        }
                    } else {
                        /*
                            set the string terminating NULL byte on the end of 
                            the data read
                        */
                        buffer[valread - 2] = '\0';
                        printf("file requested: \"%s\"\n", buffer);
                        int fd = open(buffer, O_RDONLY);
                        if (fd >= 0) {
                            int length = lseek(fd, 0, SEEK_END);

                            struct aiocb aio_ = {
                                fd,
                                0,
                                filebuffer[i],
                                length
                            };

                            file_ops[i] = aio_;
                            aio_fds[i] = fd;
                            aio_read(&aio_);
                        } else {
                            perror("failed to open file");
                            continue;
                        }   
                    }
                }
            }
        }
    }

    return 0;
}
