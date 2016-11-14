#include "string_functions.h"

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string itofillstring(int i, int l) {
    std::stringstream ss;
    ss << std::setw(l) << std::setfill('0') << i;
    return ss.str();
}
