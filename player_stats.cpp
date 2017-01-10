
#include <sstream>
#include "player_stats.h"
#include "string_functions.h"

void PlayerStats::set_client(Client *c) {
    this->client = c;
    stats[0] = std::to_string(c->get_id());
    stats[1] = c->get_name();
    stats[2] = std::to_string( c->get_color());
    stats[3] = c->is_ready() ? "1" : "0";
    stats[4] = "0"; //frame
    stats[5] = "0"; //x
    stats[6] = "0"; //y
    stats[7] = "0"; //up
    stats[8] = "0"; //l
    stats[9] = "0"; //p
    stats[10] = "0"; //fall
    stats[11] = "0"; //jump
    stats[12] = "0"; //dead
    stats[13] = "0"; //speedx
    stats[14] = "0"; //speedy
    stats[15] = "0"; //MAXSTEP


}

std::string PlayerStats::get_stat_string() {
    std::stringstream ss;
    ss << stats [0];
    for (int i = 1; i < STATS_LENGTH; ++i) {
        ss << DEFAULT_DELIMITER << stats[i] ;
    }
    return ss.str();
}

void PlayerStats::update(std::vector<std::string> data) {
    for (int i = 0; i < STATS_LENGTH; ++i) {
        stats[i] = data[i];
    }
}
