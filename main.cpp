#include "server.h"

// ダブル ジャンプ

#define DEFAULT_PORT 5000

int main(int argc, char** args) {
    Server *s = new Server();
    if (argc>1)
        s->start(atoi(args[1]));
    else s->start(DEFAULT_PORT);
    s->command_loop();



}