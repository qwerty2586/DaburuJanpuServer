#include "statistics.h"
std::atomic<uint64_t> Statistics::in_bytes;
std::atomic<uint64_t> Statistics::out_bytes;

std::atomic<uint64_t> Statistics::in_messages;
std::atomic<uint64_t> Statistics::out_messages;