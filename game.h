
#ifndef DABURUJANPUSERVER_GAME_H
#define DABURUJANPUSERVER_GAME_H

#include "command_queue.h"
#include "player_stats.h"

#define SEED_LENGTH 256
#define STEPS_TYPE_COUNT 16
#define STEPS_RESERVED_TYPES_COUNT 2

class Game {
public:
    static const int ROUNDS_COUNT = 3;
    explicit Game();
    void eat_lobby(ClientList *lobby);
    void set_player_return(ClientList *server_no_lobby,CommandQueue *server_command_queue);
    void start();
    bool game_ended();


    bool check_reconnect_id(int id);

    void reconnect_client(Client *client, int old_id);

private:

    CommandQueue *command_queue;
    ClientList *players;
    std::vector<PlayerStats> player_stats;

    /// pro navrat do lobbyn
    ClientList *server_no_lobby;
    CommandQueue *server_command_queue;


    void generate_seed();
    void get_player_info();
    std::string get_seed_info();
    void game_loop();


    int seed[SEED_LENGTH];

    std::string get_game_info();

    int ready_count = 0;

    int update_treshold;
    int unresponded_updates = 0;

    void camera_move();

    int camerapos = 0;
    int round = 0;

    bool check_reset_conditions();



    std::string get_result_info();
};

#endif //DABURUJANPUSERVER_GAME_H
