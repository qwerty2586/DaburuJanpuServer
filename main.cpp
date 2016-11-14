#include "server.h"

// ダブル ジャンプ


int main() {
    Server *s = new Server();
    s->start(5000);
    s->command_loop();



}