#ifndef DABURUJANPUSERVER_COMMAND_H
#define DABURUJANPUSERVER_COMMAND_H

#include "client.h"


namespace Commands {
    static const int HW_DISCONNECT = -1;
    static const int PING = 1;
    static const int PONG = 2;
    static const int SW_DISCONNECT = 3;
    static const int SET_PLAYER_NAME = 5;
    static const int RESULT_PLAYER_NAME = 6;
    static const int REQUEST_LOBBY_LIST = 7;
    static const int LOBBY_LIST = 8;
    static const int CREATE_LOBBY = 9;
    static const int RESULT_CREATE_LOBBY = 10;
    static const int JOIN_LOBBY = 11;
    static const int RESULT_JOIN_LOBBY = 12;
    static const int REQUEST_LOBBY_INFO = 13;
    static const int LOBBY_INFO = 14;
    static const int LEAVE_LOBBY = 15;
    static const int SEND_CHAT = 17;
    static const int RECEIVE_CHAT = 18;
    static const int SET_READY = 19;
    static const int CHANGE_COLOR = 20;

    static const int GAME_STARTED = 30;
    static const int READY_FOR_GAME_INFO = 31;
    static const int GAME_INFO = 32;
    static const int ROUND_LOADED = 33;
    static const int ROUND_START = 34;
    static const int MY_UPDATE = 35;
    static const int LEAVE_GAME = 36;
    static const int GAME_INFO_SEED = 37;
    static const int GAME_NEW_ROUND = 38;

    static const int INTERNAL_GAME_INIT = -2;






    static const int SUCCESS = 1;
    static const int FAIL = 0;
}

class Client;

class Command {
public:
    Client *sender;
    int code;
    std::vector<std::string> args;

    explicit Command() {};

    Command(Client *sender, int code);

    Command(int code);

    Command *addArg(std::string arg);

    Command *addArg(int arg);

    static Command *fromText(Client *sender, std::string string);

    std::string to_string();
};

#endif //DABURUJANPUSERVER_COMMAND_H
