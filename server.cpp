#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#else
#include <pthread.h>
#include <cygwin/socket.h>
#include <sys/socket.h>
#include <asm/byteorder.h>
#include <cygwin/in.h>
#include <unistd.h>
#endif
#include <thread>
#include <iostream>
#include <cstring>

#include "server.h"
#include "string_functions.h"
#include "lobby_utils.h"

#define FIRST_LEN 8

Server::Server() {
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    command_queue = new CommandQueue();

}

int Server::start(int port) {

    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(struct sockaddr_in));

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    int err = bind(server_sock, (struct sockaddr *) &local_addr, sizeof(struct sockaddr_in));
    if (err == 0)
        printf("Bind OK\n");
    else {
        printf("Bind ER\n");
        return err;
    }

    err = listen(server_sock, 5);
    if (err == 0)
        printf("Listen OK\n");
    else {
        printf("Listen ER\n");
        return err;
    }


    auto _thread = std::thread([&]() { accepting_loop(); });
    _thread.detach();

    running = true;

    return 0;


}

void Server::stop() {


    std::terminate();
}

bool Server::isRunning() {
    return running;
}

void Server::command_loop() {
    while (1) {

        command_queue->wait_for_nonempty();
        Command *command = command_queue->remove();
        std::cout << "command loop " << command->code << std::endl;
        if (!command) continue;
        switch (command->code) {

            case Commands::HW_DISCONNECT : {
                Client *c = command->sender;
                ClientList* lobby = c->get_client_list();
                lobby->remove(c);
                disconecteds.add(c);
                c->set_client_list(&disconecteds);
                c->send_command(new Command(Commands::HW_DISCONNECT)); // reknem odesilacimu thredu aby se ukoncil
                if (lobby->empty() && lobby != &no_lobby) {
                   delete_dead_lobby(lobby);
                   update_no_lobby_changes(); //reknem klientum ze uz neni tato lobby
                }
                break;
            }

            case Commands::SW_DISCONNECT : {
                Client *c = command->sender;
                c->get_client_list()->remove(c);
                c->set_client_list(&disconecteds);
                c->send_command(new Command(Commands::SW_DISCONNECT));
                std::cout << "disconect" << std::endl;
                break;
            }
            case Commands::PING : {
                command->sender->send_command(new Command(Commands::PONG));
                break;
            }

            case Commands::SET_PLAYER_NAME : {
                command->sender->set_name(command->args[0]);
                Command *c = (new Command(Commands::RESULT_PLAYER_NAME))->addArg(command->sender->get_name());
                command->sender->send_command(c);
                break;
            }
            case Commands::REQUEST_LOBBY_LIST : {
                command->sender->send_command((new Command(Commands::LOBBY_LIST))->addArg(lobby_list(lobbies)));
                break;
            }
            case Commands::CREATE_LOBBY : {
                std::string name = command->args[0];
                if (conflict_lobbby_name(lobbies, name)) {
                    command->sender->send_command((new Command(Commands::RESULT_CREATE_LOBBY))->addArg(Commands::FAIL));
                    break;
                }
                ClientList *lobby = new ClientList();
                lobby->set_name(name);
                lobby->set_capacity(ClientList::DEFAULT_LOBBY_CAPACITY);
                lobby->set_id(++last_lobby_id);
                lobbies.push_back(lobby);
                command->sender->get_client_list()->remove(command->sender);
                command->sender->set_ready(false);
                lobby->add(command->sender);
                command->sender->set_client_list(lobby);
                command->sender->send_command((new Command(Commands::RESULT_CREATE_LOBBY))->addArg(Commands::SUCCESS));
                command->sender->set_color(find_unused_color(lobby,0,ClientList::MAX_COLOR)); //nastavime si barvicku
                update_no_lobby_changes(); // homelessaci se dovi ze je novej barak
                break;
            }
            case Commands::JOIN_LOBBY : {
                int id = std::stoi(command->args[0]);
                ClientList *lobby = get_lobby_by_id(lobbies, id);
                if (lobby) {
                    if (lobby->add(command->sender)) { // fail znamena ze se prekrocila kapacita lobby
                        command->sender->set_ready(false);
                        no_lobby.remove(command->sender);
                        command->sender->set_client_list(lobby);
                        command->sender->set_color(find_unused_color(lobby,0,ClientList::MAX_COLOR));
                        command->sender->send_command(
                                (new Command(Commands::RESULT_JOIN_LOBBY))->addArg(Commands::SUCCESS));
                        update_no_lobby_changes(); // homelessaci se dovi ze v baraku je vic nehomelessaku
                        update_lobby_changes(lobby); // jeste dame vedet soucasnikum
                    } else {
                        command->sender->send_command(
                                (new Command(Commands::RESULT_JOIN_LOBBY))->addArg(Commands::FAIL)->addArg(
                                        "LOBBY FULL"));
                    }
                } else {
                    command->sender->send_command(
                            (new Command(Commands::RESULT_JOIN_LOBBY))->addArg(Commands::FAIL)->addArg(
                                    "LOBBY DOESNT EXIST"));
                }
                break;
            }
            case Commands::REQUEST_LOBBY_INFO : {
                ClientList *lobby = command->sender->get_client_list();
                std::string arg = lobby_info(lobby);
                command->sender->send_command((new Command(Commands::LOBBY_INFO))->addArg(arg));
                break;
            }
            case Commands::LEAVE_LOBBY : {
                ClientList *lobby = command->sender->get_client_list();
                lobby->remove(command->sender);
                no_lobby.add(command->sender);
                command->sender->set_client_list(&no_lobby);
                if (lobby->empty()) {
                    delete_dead_lobby(lobby);
                    update_no_lobby_changes(); //reknem klientum ze uz neni
                } else {
                    update_lobby_changes(lobby); //dame vedet soucasnikum ze nekdo odesel
                }
                break;
            }
            case Commands::SEND_CHAT : {
                std::string message = command->sender->get_name() + "> " + command->args[0];
                for (Client *client : command->sender->get_client_list()->get_list()) {
                    client->send_command((new Command(Commands::RECEIVE_CHAT))->addArg(message));
                }
                break;
            }
            case Commands::CHANGE_COLOR : {
                int old_color = command->sender->get_color();
                int new_color = find_unused_color(command->sender->get_client_list(),old_color,ClientList::MAX_COLOR);
                if (old_color!=new_color) {
                    command->sender->set_color(new_color);
                    update_lobby_changes(command->sender->get_client_list()); //dame vedet ze si nekdo zmenil barvicku
                }
                break;
            }

            case Commands::SET_READY : {
                command->sender->set_ready(!command->sender->is_ready());
                update_lobby_changes(command->sender->get_client_list());
                if (is_lobby_ready(command->sender->get_client_list())) {
                    Game *game = new Game();
                    game->eat_lobby(command->sender->get_client_list());
                    game->set_player_return(&no_lobby,command_queue);
                    game->start();
                    games.push_back(game);

                    delete_dead_lobby(command->sender->get_client_list());
                    update_no_lobby_changes(); //reknem klientum ze uz neni
                }
            }


        }
        delete command;
    };

}

void Server::delete_dead_lobby(ClientList *lobby) {
    for (auto iter = lobbies.begin(); iter != lobbies.end(); iter++) {
        if (*iter == lobby) {
            lobbies.erase(iter);
            break;
        }
    }
}

void Server::update_no_lobby_changes() {
    std::string arg = lobby_list(lobbies);
    no_lobby.search_lock(); // zrovna tady se hodi locknout protoze muzed prijit accept loop a hodit tam dalsiho klienta
    for (Client *c : no_lobby.get_list()) {
        c->send_command((new Command(Commands::LOBBY_LIST))->addArg(arg));
    }
    no_lobby.search_unlock();
}

void Server::update_lobby_changes(ClientList *lobby) {
    std::string arg = lobby_info(lobby);
    for (Client *c : lobby->get_list()) {
        c->send_command((new Command(Commands::LOBBY_INFO))->addArg(arg));
    }
}


void Server::accepting_loop() {
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len;
    bool exit_command = false;
    ClientList unfinished_clients;
    int last_id = 0;

    while (!exit_command) {
        std::cout << "accepting loop" << std::endl;
        int client_sock = accept(server_sock, (struct sockaddr *) &remote_addr, &remote_addr_len);
        if (client_sock > 0) {
            auto _thread = std::thread([&]() {
                char recv_buffer[FIRST_LEN + 1];// = new char[FIRST_LEN+1];
                int bytes_count = (int) recv(client_sock, recv_buffer, FIRST_LEN, 0);
                if (bytes_count != FIRST_LEN) {
                    send(client_sock, "9WRONG INPUT", 12, 0);
                    close(client_sock);
                } else {

                    // incoming structure ascii number CIIIIIII C=command I=identifier 0000000 means first connection
                    // commands:
                    // 0 = opening first socket - can be new connection
                    // 1 = server returning ok here is your new identifier
                    // 2 = opening second socket - this must have server designated id
                    // 3 = ok you are good to go
                    // 5 - 8 same with reconnects -- not implemented yet, maybe not implemented ever
                    // 9 = halt and catch fire
                    recv_buffer[FIRST_LEN] = '\0';
                    std::string string_id = std::string(recv_buffer);
                    string_id = string_id.substr(1, FIRST_LEN - 1);

                    int id = 0;
                    bool error = false;
                    try {
                        id = std::stoi(string_id);
                    }
                    catch (std::exception const &e) {
                        error = true;
                    }

                    if (!error) {
                        switch (recv_buffer[0]) {
                            case '0' : {
                                if (id == 0) {
                                    Client *c = new Client();
                                    c->add_first_socket(client_sock);
                                    c->set_id(++last_id);

                                    std::string output("1");
                                    output.append(itofillstring(c->get_id(), FIRST_LEN - 1));
                                    send(client_sock, output.c_str(), output.length(), 0);
                                    unfinished_clients.add(c);
                                }

                            }
                                break;
                            case '2': {
                                unfinished_clients.search_lock();
                                std::vector<Client *> list = unfinished_clients.get_list();
                                Client *c = NULL;
                                for (int i = 0; i < list.size(); ++i) {
                                    if (list[i]->get_id() == id) {
                                        c = list[i];
                                        break;
                                    }
                                }
                                unfinished_clients.search_unlock();
                                if (c) {
                                    unfinished_clients.remove(c);
                                    c->add_second_socket(client_sock);
                                    std::string output("3");
                                    output.append(itofillstring(c->get_id(), FIRST_LEN - 1));
                                    send(client_sock, output.c_str(), output.length(), 0);
                                    no_lobby.add(c);
                                    c->set_client_list(&no_lobby);
                                    c->set_server_command_queue(command_queue);
                                    c->spawn_threads();
                                } else {
                                    error = true;
                                }
                            }
                        }
                    }
                    if (error) {
                        send(client_sock, "9WRONG INPUT", 12, 0);
                        close(client_sock);
                    }
                }
            });
            _thread.detach();


        } else {
            printf("Trable\n");

        }
    }
}

Server::~Server() {
    if (isRunning()) stop();
    delete command_queue;
}





