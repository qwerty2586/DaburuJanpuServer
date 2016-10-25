
#ifndef DABURUJANPUSERVER_SERVER_H
#define DABURUJANPUSERVER_SERVER_H

#include <elf.h>

const static int DEFAULT_PORT = 1234;

class Server {
public:
    explicit Server();
    void start(int port);
    void start() { start(DEFAULT_PORT); }
    void stop();
    ~Server() { if (isRunning()) stop(); }

    bool isRunning();


    void loop();

private:
    bool running = false;
    int server_sock;
    pthread_t accepting_thread;
    void *accepting_loop();



};

#endif //DABURUJANPUSERVER_SERVER_H
