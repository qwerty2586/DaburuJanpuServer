#ifndef DABURUJANPUSERVER_STATISTICS_H
#define DABURUJANPUSERVER_STATISTICS_H

#include <cstdint>
#include <atomic>

class Statistics{

public:
    static void add_in (uint64_t length) { in_messages++;  in_bytes+=length;}
    static void add_out(uint64_t length) { out_messages++; out_bytes+=length;}

    static uint64_t get_in_bytes()      { return (uint64_t) in_bytes;}
    static uint64_t get_in_messages()   { return (uint64_t) in_messages;}
    static uint64_t get_out_bytes()     { return (uint64_t) out_bytes;}
    static uint64_t get_out_messages()  { return (uint64_t) out_messages;}

private:

    static std::atomic<uint64_t> in_bytes;
    static std::atomic<uint64_t> in_messages;
    static std::atomic<uint64_t> out_bytes;
    static std::atomic<uint64_t> out_messages;

};




#endif //DABURUJANPUSERVER_STATISTICS_H
