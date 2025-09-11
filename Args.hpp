#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP
#include <string>
#include "Logging/Logger.hpp"

namespace Core
{

struct Arguments
{
private:
  int _argc;
  char** _argv;
  int atoi_overflow_guard(char* nptr, int* res);
public:
  unsigned short port;
  std::string password;
  Arguments(int argc, char** argv);
  int parseArgs(void);
  
};  

} // namespace Core

#endif
