#include <pthread.h>
#include <iostream>
#include <cygwin/socket.h>
#include <sys/socket.h>
#include <cstring>
#include <asm/byteorder.h>
#include <cygwin/in.h>
#include <vector>
#include <unistd.h>
#include "server.h"
#include "client.h"

#define FIRST_LEN 8

Server::Server() {
    server_sock = socket(AF_INET, SOCK_STREAM, 0);

}

void Server::start(int port) {

    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(struct sockaddr_in));

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    int rv_socket = bind(server_sock, (struct sockaddr *) &local_addr, sizeof(struct sockaddr_in));
    if (rv_socket == 0)
        printf("Bind OK\n");
    else {
        printf("Bind ER\n");
        return;
    }

    int rv_listen = listen(server_sock, 5);
    if (rv_listen == 0)
        printf("Listen OK\n");
    else{
        printf("Listen ER\n");
        return;
    }
    std::cout<< rv_socket << std::endl;
    int x = EACCES;

    int rv_thread = pthread_create(&accepting_thread, NULL, (void *(*)(void *)) this->accepting_loop(), NULL);
    if (rv_thread == 0)
        printf("Thread OK\n");
    else{
        printf("Thread ER\n");
        return;
    }

    running = true;


}

void Server::stop() {


}

bool Server::isRunning() {
    return running;
}

void Server::loop() {
 while (1) {
     usleep(1000);
 };

}

void *Server::accepting_loop() {
    std::cout << "heh" << std::endl;
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len;
    bool exit_command = false;
    std::vector<Client> unfinished_clients;

    while (!exit_command) {
        int client_sock = accept(server_sock, (struct sockaddr *) &remote_addr, &remote_addr_len);
        if (client_sock > 0) {
            char recv_buffer[FIRST_LEN];
            int bytes_count = recv(client_sock, recv_buffer, FIRST_LEN, 0);
            std::cout<< bytes_count << std::endl;
            std::cout<< recv_buffer << std::endl;

        } else {
           printf("Trable\n");

        }
    }
}
