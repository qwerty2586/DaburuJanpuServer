#ifdef __linux__
#include <zconf.h>

#else
#include <unistd.h>
#endif
#include <thread>

#include <iostream>
#include <sys/socket.h>
#include <signal.h>

#include "client.h"
#include "statistics.h"


void Client::add_first_socket(int socket) {
    sock_in  = socket;
}

void Client::set_id(int id) {
    this->id = id;
}

int Client::get_id() {
    return id;
}

void Client::add_second_socket(int socket) {
    sock_out  = socket;
}

void Client::set_client_list(ClientList *clientList) {
    this->clientList = clientList;
}

void Client::spawn_threads() {
    if (threads_running) return;
    clientCommandQueue = new CommandQueue();
    auto t1 = std::thread([&]() {this->receving_loop();});
    t1.detach();
    auto t2 = std::thread([&]() {this->sending_loop();});
    t2.detach();
    threads_running=true;
}

#define SIZE_BYTES 2
void Client::receving_loop() {
    char size_buffer[SIZE_BYTES];
    ssize_t size=0;
    ssize_t a=0;
    ssize_t len;
    while (1) {
        std::cout << "receiving loop" << std::endl;

        len = 0;
        while (len<SIZE_BYTES) {
            a = recv(sock_in, size_buffer + len, SIZE_BYTES - len, 0);
            std::cout << len << std::endl;
            if (a < 1) {
                close(sock_in);
                serverCommandQueue->add(new Command(this, Commands::HW_DISCONNECT));
                return;
            };
            len += a;
        }
        size = ((size_t)size_buffer[0] << 8) + size_buffer[1];
        std::cout << size << std::endl;
        char message_buffer[size];
        len = 0;
        while (len<SIZE_BYTES) {
            a = recv(sock_in, message_buffer + len, size - len, 0);
            if ( a<1 ) {
                close(sock_in);
                serverCommandQueue->add(new Command(this, Commands::HW_DISCONNECT));
                return;
            };
            len += a;
        }
        Statistics::add_in(size + 2);
        auto command = Command::fromText(this,std::string(message_buffer,size));
        if (command) {
            serverCommandQueue->add(command);
            if (command->code==Commands::SW_DISCONNECT) {
                close(sock_in);
                return;
            }
        }
    }
}

void Client::sending_loop() {
    char size_buffer[SIZE_BYTES];
    size_t size=0;
    signal(SIGPIPE,SIG_IGN);
    while (1) {
        std::cout << "sending loop" << std::endl;
        clientCommandQueue->wait_for_nonempty();
        Command *command = clientCommandQueue->remove();
        if (!command) continue; // mozna redundantni
        if (command->code == Commands::HW_DISCONNECT || command->code == Commands::SW_DISCONNECT) {
            close(sock_out);
            std::cout << "disconnect sending loop" << std::endl;
            delete clientCommandQueue;
            if (command->code == Commands::SW_DISCONNECT) delete this;
            delete command;
            return;
        };
        std::string string = command->to_string();
        size = (int)string.size();
        size_buffer[0] = (char)(size / 256) ;
        size_buffer[1] = (char)(size % 256) ;
        const char *xx = string.c_str();
        ssize_t len = send(sock_out,size_buffer,SIZE_BYTES,0);
        if (len>=0) send(sock_out,string.c_str(),size,0);
        Statistics::add_out(size+2);
        delete command;
    }
}


void Client::send_command(Command *command) {
    this->clientCommandQueue->add(command);
}

void Client::set_server_command_queue(CommandQueue *commandQueue) {
    this->serverCommandQueue = commandQueue;
}

ClientList *Client::get_client_list() {
    return clientList;
}

void Client::set_name(std::string name) {
    this->name = name;
}

std::string Client::get_name() {
    return name;
}

void Client::set_color(int color) {
    this->color = color;
}

int Client::get_color() {
    return color;
}

void Client::set_ready(bool ready) {
    this->ready = ready;
}

bool Client::is_ready() {
    return ready;
}



