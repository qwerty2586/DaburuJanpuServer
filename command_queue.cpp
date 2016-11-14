
#include "command_queue.h"

void CommandQueue::add(Command *command) {
    mutex.lock();
    commands.push(command);
    mutex.unlock();
    cond.notify_all();
}

Command *CommandQueue::remove() {
    Command *c;
    mutex.lock();
    if (commands.empty()) {
        c = nullptr;
    } else {
        c = commands.front();
        commands.pop();
    }
    mutex.unlock();

    return c;
}

void CommandQueue::wait_for_nonempty() {
    if (isEmpty()) {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock);
    }
}

bool CommandQueue::isEmpty() {
    bool r;
    mutex.lock();
    r = commands.empty();
    mutex.unlock();
    return r;
}

