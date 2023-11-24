#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

/**
*   Close a socket by given socket file descriptor(sockfd).
*/
int close_socket(int sockfd) {
    printf("Closing socket with id: %d\n\n", sockfd);
    int close_value = close(sockfd);

    // If return value -1, then log error.
    if(close_value < 0) {
        perror("Error while closing socket");
        exit(EXIT_FAILURE);
    }

    return close_value;
}

/**
*   Create and return socket address object
*/
struct sockaddr_in get_socket_addr() {
    struct sockaddr_in sock_addr;

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(8080);
    sock_addr.sin_addr.s_addr = INADDR_ANY;

    return sock_addr;
}

/**
*   Create a socket listening on give address(sock_addr).
*   Return the socket's file descriptor.
*/
int get_listening_socket(struct sockaddr_in sock_addr) {

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0) {
        perror("Error while creating socket");
        close_socket(sockfd);
        exit(EXIT_FAILURE);
    }

    int bind_value = bind(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));

    if(bind_value < 0) {
        perror("Error while binding");
        close_socket(sockfd);
        exit(EXIT_FAILURE);
    }

    int listen_value = listen(sockfd, 1);

    if(listen_value < 0) {
        perror("Error while listening");
        close_socket(sockfd);
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

void handle_request(char data[]) {

    printf("------------------------------\n");
    printf("Printing data:\n%s\n", data);

}

int main() {

    struct sockaddr_in sock_addr = get_socket_addr();
    int buffer_size = 1000;
    char buffer[buffer_size];
    int sock_addr_len = sizeof(sock_addr);

    /*
    * Creating a socket is abstracted here.
    */
    int socketfd = get_listening_socket(sock_addr);

    while (1) {
        /*
        * Start looking for requests/Accept requests.
        * Note that accept() is blocking function so it will wait for requests being made.
        */ 
        int new_socketfd = accept(socketfd, (struct sockaddr*)&sock_addr, &sock_addr_len);
        if(new_socketfd < 0) {
            perror("Error while accepting");
            close_socket(socketfd);
            exit(EXIT_FAILURE);
        }

        // Read data from requests.
        int recv_status = recv(new_socketfd, (void*)&buffer, buffer_size-1, 0);
        if(recv_status < 0) {
            perror("Error while receiving");
            close_socket(new_socketfd);
            close_socket(socketfd);
            exit(EXIT_FAILURE);
        }

        // Handle Request
        handle_request(buffer);

        close_socket(new_socketfd);
    }

    close_socket(socketfd);

}