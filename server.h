
#ifndef DABURUJANPUSERVER_SERVER_H
#define DABURUJANPUSERVER_SERVER_H

#ifndef __linux__
#include <elf.h>
#endif

#include "client_list.h"

const static int DEFAULT_PORT = 1234;

class Server {
public:
    explicit Server();
    int start(int port);
    void start() { start(DEFAULT_PORT); }
    void stop();
    ~Server();
    bool isRunning();

    void command_loop();

private:
    bool running = false;
    int server_sock;
    void accepting_loop();
    void update_no_lobby_changes();
    void update_lobby_changes(ClientList *lobby);
    ClientList no_lobby;
    ClientList disconecteds;
    std::vector<ClientList *> lobbies;
    CommandQueue *commandQueue;
    int last_lobby_id = 0;

    void delete_dead_lobby(ClientList *lobby);
};

#endif //DABURUJANPUSERVER_SERVER_H
