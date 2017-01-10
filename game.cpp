
#include <thread>
#include <iostream>
#include <sstream>
#include "game.h"
#include "string_functions.h"

Game::Game() {

    command_queue = new CommandQueue();
    generate_seed();

}

void Game::eat_lobby(ClientList *lobby) {
    lobby->search_lock();
    for (Client *client : lobby->get_list()) {

    }
    lobby->search_unlock();
    players = lobby;
}

void Game::set_player_return(ClientList *server_no_lobby, CommandQueue *server_command_queue) {
    this->server_no_lobby = server_no_lobby;
    this->server_command_queue = server_command_queue;
}

void Game::generate_seed() {
    srand(time(NULL));
    for (int i = 0; i < SEED_LENGTH; ++i) {
        seed[i] = (rand() % (STEPS_TYPE_COUNT - STEPS_RESERVED_TYPES_COUNT)) + STEPS_RESERVED_TYPES_COUNT;
    }
}

void Game::start() {
    for (Client *c : players->get_list()) {
        c->set_server_command_queue(command_queue);
        c->set_ready(false);
        player_stats.push_back(PlayerStats());
        player_stats[player_stats.size() - 1].set_client(c);
    }

    update_treshold = (int)( players->get_list().size() + 1 )/ 2;

    auto t = std::thread([&]() { this->game_loop(); });
    t.detach();


}

void Game::game_loop() {
    command_queue->add(new Command(Commands::INTERNAL_GAME_INIT));
    while (players->get_list().size() > 0) {

        command_queue->wait_for_nonempty();
        Command *command = command_queue->remove();
        std::cout << "game loop " << command->code << std::endl;
        if (!command) continue;
        switch (command->code) {
            case Commands::INTERNAL_GAME_INIT: {
                for (int i = 0; i < players->get_list().size(); ++i) {
                    players->get_list()[i]->send_command(new Command(Commands::GAME_STARTED));
                }
                break;
            }
            case Commands::HW_DISCONNECT : {
                // neco se musi delat tady
                break;
            }
            case Commands::READY_FOR_GAME_INFO: {
                command->sender->send_command((new Command(Commands::GAME_INFO))->addArg(get_game_info()));
                command->sender->send_command((new Command(Commands::GAME_INFO_SEED))->addArg(get_seed_info()));
                break;
            }

            case Commands::ROUND_LOADED: {
                ready_count++;
                players->search_lock();
                if (ready_count >= players->get_list().size()) {
                    for (int i = 0; i < players->get_list().size(); ++i) {
                        players->get_list()[i]->send_command(
                                new Command(Commands::ROUND_START));
                    }
                }
                players->search_unlock();
                break;
            }
            case Commands::MY_UPDATE: { //update od hrace
                int i=0;
                while (command->sender != player_stats[i].client) i++;
                player_stats[i].update(command->args);
                unresponded_updates ++ ;
                if (unresponded_updates>=update_treshold) {
                    std::string game_info=get_game_info();
                    for (int j = 0; j < players->get_list().size(); ++j) {
                        players->get_list()[j]->send_command((new Command(Commands::GAME_INFO))->addArg(game_info));
                    }
                }


            }


        }
    }

}

std::string Game::get_seed_info() {
    std::stringstream ss;
    ss << seed[0];
    for (int i = 1; i < SEED_LENGTH; ++i) {
        ss << DEFAULT_DELIMITER << seed[i];
    }
    return ss.str();
}

std::string Game::get_game_info() {
    std::stringstream ss;
    ss << player_stats.size();
    for (int i = 0; i < player_stats.size(); ++i) {
        ss << DEFAULT_DELIMITER << player_stats[i].get_stat_string();
    }
    return ss.str();
}
