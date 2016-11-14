#ifndef DABURUJANPUSERVER_CLIENT_LIST_H
#define DABURUJANPUSERVER_CLIENT_LIST_H

#include <vector>
#include <mutex>
#include "client.h"

class Client;

class ClientList {
public:
    bool add(Client *client);
    bool remove(Client *client);
    //  explicit void ClientList();
    void search_lock();
    void search_unlock();
    std::vector<Client *> get_list();

    void set_id(int id) {this->id=id;}
    int get_id() { return id;}
    void set_name(std::string name) {this->name=name;}
    std::string get_name() { return name;}
    void set_capacity(int capacity) {this->capacity=capacity;}
    int get_capacity() { return capacity;}

    static const int DEFAULT_LOBBY_CAPACITY = 32;
    static const int MAX_COLOR = 32;

    bool empty();

private:
    std::vector<Client *> clients;
    std::mutex mutex;

    int id =-1;
    std::string name = "";
    int capacity = std::numeric_limits<int>::max();;


};

#endif //DABURUJANPUSERVER_CLIENT_LIST_H
