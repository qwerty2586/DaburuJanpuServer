
#ifndef DABURUJANPUSERVER_SPLIT_H
#define DABURUJANPUSERVER_SPLIT_H


#include <string>
#include <vector>

void split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif //DABURUJANPUSERVER_SPLIT_H
