#include "CustomAlgo.hpp"

std::string
Algo::String::ToLower(std::string s)
{
  for (std::string::iterator it = s.begin(); it != s.end(); ++it)
  {
    *it = std::tolower(*it);
  }
  return s;
}
