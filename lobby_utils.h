#ifndef DABURUJANPUSERVER_LOBBY_UTILS_H
#define DABURUJANPUSERVER_LOBBY_UTILS_H

#include "client_list.h"

std::string lobby_list(std::vector<ClientList *> &lobbies);
bool conflict_lobbby_name(std::vector<ClientList *> &lobbies, std::string  name);
ClientList *get_lobby_by_id(std::vector<ClientList *> &lobbies,int id);
std::string lobby_info(ClientList *lobby);
int find_unused_color(ClientList *lobby,int start_color,int max_color);
bool is_lobby_ready(ClientList *lobby);



#endif //DABURUJANPUSERVER_LOBBY_UTILS_H
