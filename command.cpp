#include "command.h"
#include "string_functions.h"

Command *Command::fromText(Client *sender, std::string string) {
    std::vector<std::string> args = split(string,DEFAULT_DELIMITER);
    if (args.empty()) return nullptr;

    Command *c = new Command();
    try {
        c->code = std::stoi(args[0]);
    }
    catch (std::exception const &e) {
        delete c;
        return nullptr;
    }
    args.erase(args.begin());
    c->args = args;
    c->sender=sender;
    return c;
}

Command *Command::addArg(std::string arg) {
    args.push_back(arg);
    return this;
}

Command *Command::addArg(int arg) {
    return addArg(std::to_string(arg));
}

Command::Command(Client *sender, int code) {
    this->sender = sender;
    this->code = code;
}

Command::Command(int code) {
    this->sender = NULL;
    this->code = code;
}

std::string Command::to_string() {
    std::string r(std::to_string(code));
    for (std::string arg : args) {
        r.push_back(DEFAULT_DELIMITER);
        r.append(arg);
    }
    return r;
}



