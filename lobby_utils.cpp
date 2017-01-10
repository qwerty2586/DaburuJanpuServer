#include <sstream>
#include "lobby_utils.h"
#include "string_functions.h"


std::string lobby_list(std::vector<ClientList *> &lobbies) {

    std::stringstream ss;
    ss << lobbies.size();
    for ( ClientList *lobby : lobbies ) {
        ss << DEFAULT_DELIMITER << lobby->get_id();
        ss << DEFAULT_DELIMITER << lobby->get_name();
        ss << DEFAULT_DELIMITER << lobby->get_list().size();
        ss << DEFAULT_DELIMITER << lobby->get_capacity();
    }
    return ss.str();
}

bool conflict_lobbby_name(std::vector<ClientList *> &lobbies, std::string  name) {

    for ( ClientList *lobby : lobbies ) {
        if (lobby->get_name().compare(name) == 0) return true;
    }
    return false;
}

ClientList *get_lobby_by_id(std::vector<ClientList *> &lobbies,int id) {
    for ( ClientList *lobby : lobbies ) {
        if (lobby->get_id() == id) return lobby;
    }
    return nullptr;
}

std::string lobby_info(ClientList *lobby) {
    std::stringstream ss;
    ss << lobby->get_id();
    ss << DEFAULT_DELIMITER << lobby->get_name();
    ss << DEFAULT_DELIMITER << lobby->get_capacity();
    lobby->search_lock();
    ss << DEFAULT_DELIMITER << lobby->get_list().size();

    for ( Client *client : lobby->get_list() ) {
        ss << DEFAULT_DELIMITER << client->get_id();
        ss << DEFAULT_DELIMITER << client->get_name();
        ss << DEFAULT_DELIMITER << client->get_color();
        ss << DEFAULT_DELIMITER << (int)client->is_ready();
    }
    lobby->search_unlock();
    return ss.str();
}

int find_unused_color(ClientList *lobby, int start_color, int max_color) {
    bool color_used[max_color] = {0} ;

    lobby->search_lock();
    for ( Client *client : lobby->get_list() ) {
        if (client->get_color()>=0)
        color_used[client->get_color()] = true;
    }
    lobby->search_unlock();
    if (start_color<0) start_color=0;
    for (int i=start_color;i!=(start_color-1+max_color)%max_color;i=(i+1)%max_color) {
        if (!color_used[i]) return i;
    }
    return start_color;
}

bool is_lobby_ready(ClientList *lobby) {

    lobby->search_lock();
    for ( Client *client : lobby->get_list() ) {
        if (!client->is_ready()) {
            lobby->search_unlock();
            return false;
        }
    }
    lobby->search_unlock();
    return true;
}



