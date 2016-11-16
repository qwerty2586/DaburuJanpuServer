#ifndef DABURUJANPUSERVER_SPLIT_H
#define DABURUJANPUSERVER_SPLIT_H


#include <iostream>
#include <vector>


const char DEFAULT_DELIMITER = ';';

void split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
std::string itofillstring(int i, int l);


#endif //DABURUJANPUSERVER_SPLIT_H
