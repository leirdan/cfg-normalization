#ifndef LOGGER_H
#define LOGGER_H
#include <ostream>

using namespace std;

class Logger
{
private:
    inline static ostream *out = nullptr;

public:
    inline static void setStream(ostream *os) { out = os; };
    inline static ostream &log() { return *out; };
};

#endif //! LOGGER_H