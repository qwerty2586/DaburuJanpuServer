#include "client_list.h"

bool ClientList::add(Client *client) {
    mutex.lock();
    if (clients.size()<capacity) {
        clients.push_back(client);
        mutex.unlock();
        return true;
    }
    mutex.unlock();
    return false;

}

bool ClientList::remove(Client *client) {
    mutex.lock();
    for (int i = 0; i < clients.size(); ++i) {
        if (clients[i]==client) {
            clients.erase(clients.begin()+i);
            mutex.unlock();
            return true;
        }
    }
    mutex.unlock();
    return false;
}

void ClientList::search_lock() {
    mutex.lock();
}

void ClientList::search_unlock() {
    mutex.unlock();

}

std::vector<Client *> ClientList::get_list() {
    return clients;
}

bool ClientList::empty() {
    mutex.lock();
    bool r = clients.empty();
    mutex.unlock();
    return r;
}

