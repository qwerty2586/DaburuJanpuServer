
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
        if (!command) continue;
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
                command->sender->send_command(new Command(Commands::HW_DISCONNECT));
                players->remove(command->sender);
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
                    ready_count=0;
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
                    camera_move();
                    std::string game_info=get_game_info();
                    for (int j = 0; j < players->get_list().size(); ++j) {
                        players->get_list()[j]->send_command((new Command(Commands::GAME_INFO))->addArg(game_info));
                    }

                    if (check_reset_conditions()) {
                        for (int j = 0; j < player_stats.size(); ++j) {
                            player_stats[j].move_to_00();
                        }
                        camerapos = 0;
                        std::string game_info=get_game_info();
                        for (int j = 0; j < players->get_list().size(); ++j) {
                            players->get_list()[j]->send_command((new Command(Commands::GAME_INFO))->addArg(game_info));
                        }
                        for (int i = 0; i < players->get_list().size(); ++i) {
                            players->get_list()[i]->send_command(
                                    new Command(Commands::GAME_NEW_ROUND));
                        }

                    }
                }
                break;
            }
            case Commands::LEAVE_GAME: {
                int i=0;
                while (command->sender != player_stats[i].client) i++;
                player_stats[i].unlink_client();
                players->remove(command->sender);
                command->sender->set_server_command_queue(server_command_queue);
                server_no_lobby->add(command->sender);
                update_treshold = (int)( players->get_list().size() + 1 )/ 2;
                break;
            }


        }
    }

    std::cout << "game loop stop" << std::endl;


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
    ss << DEFAULT_DELIMITER << camerapos;
    return ss.str();
}

void Game::camera_move() {
    int count=0;
    int center_of_mass=0;
    for (PlayerStats st :player_stats) {
        if (!st.dead()) {
            count++;
            center_of_mass+=st.y();
        }
    }

    if (count>0) {center_of_mass = center_of_mass / count;}

    if (center_of_mass > camerapos) {
        camerapos += (center_of_mass - camerapos) / 4;
    }

    for (PlayerStats &st :player_stats) {
        if (camerapos-200 > st.y()) {
            st.stats[PlayerStats::I_DEAD] = "1"; //zabijem hrace
        }
    }


}

bool Game::check_reset_conditions() {
    int living = 0;
    for (PlayerStats st :player_stats) {
        if (!st.dead() && st.client!=NULL) {living++;}
    }
    return living<1; // v produkcni verzi 1
}
