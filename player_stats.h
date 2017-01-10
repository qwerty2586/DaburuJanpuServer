
#ifndef DABURUJANPUSERVER_PLAYER_STATS_H
#define DABURUJANPUSERVER_PLAYER_STATS_H

#include "client.h"

#define STATS_LENGTH 16

class PlayerStats {
public:
    Client *client; //NULL znamena odpojenej
    std::string stats[STATS_LENGTH];

    void set_client(Client *c);

    std::string get_stat_string();
    void update(std::vector<std::string> data);

};

#endif //DABURUJANPUSERVER_PLAYER_STATS_H
