
#ifndef DABURUJANPUSERVER_PLAYER_STATS_H
#define DABURUJANPUSERVER_PLAYER_STATS_H

#include "client.h"

#define STATS_LENGTH 16

class PlayerStats {
public:
    static const int I_ID = 0;
    static const int I_NAME = 1;
    static const int I_COLOR = 2;
    static const int I_Y = 6;
    static const int I_MAX_STEP = 15;
    static const int I_DEAD = 12; // kurva todle premenovat

    Client *client; //NULL znamena odpojenej
    std::string stats[STATS_LENGTH];


    void set_client(Client *c);
    void unlink_client();

    std::string get_stat_string();
    void update(std::vector<std::string> data);
    void move_to_00();

    int y();
    int max_step();
    bool dead();
    int score = 0;
    int max_max_step = 0; // napric kolama


};

#endif //DABURUJANPUSERVER_PLAYER_STATS_H
