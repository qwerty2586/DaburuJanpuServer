
#ifndef DABURUJANPUSERVER_CLIENT_H
#define DABURUJANPUSERVER_CLIENT_H

#include "client_list.h"
#include "command_queue.h"

class ClientList;
class CommandQueue;
class Command;

class Client {
public:
    void add_first_socket(int socket);
    void set_id(int id);
    int get_id();
    void set_name(std::string name);
    void set_color(int color);
    int get_color();
    void set_ready(bool ready);
    bool is_ready();
    std::string get_name();
    void set_client_list(ClientList *clientList);
    ClientList *get_client_list();
    void add_second_socket(int socket);
    void set_server_command_queue(CommandQueue *commandQueue);
    void send_command(Command *command);
    void spawn_threads();

private:
    int id;
    bool threads_running=false;
    int sock_in,sock_out;
    ClientList *clientList;
    void receving_loop();
    void sending_loop();
    CommandQueue *serverCommandQueue,*clientCommandQueue;

    std::string name = "Player";
    int color = -1;
    bool ready = false;


};

#endif //DABURUJANPUSERVER_CLIENT_H
