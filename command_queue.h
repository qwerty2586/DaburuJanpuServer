#ifndef DABURUJANPUSERVER_COMMAND_QUEUE_H
#define DABURUJANPUSERVER_COMMAND_QUEUE_H


#include <mutex>
#include <queue>
#include <condition_variable>
#include "command.h"

class Command;

class CommandQueue {

public:
    void add(Command *command);
    Command *remove();
    void wait_for_nonempty();
    bool isEmpty();

private:
    std::queue<Command *>commands;
    std::mutex mutex;
    std::condition_variable cond;
};


#endif //DABURUJANPUSERVER_COMMAND_QUEUE_H
