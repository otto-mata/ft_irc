#include "CustomAlgo.hpp"

bool
Algo::String::SaferStoul(std::string s, unsigned long* res)
{
  bool guard = false;
  size_t i = 0;
  *res = 0;
  while (i < s.size() && (s.at(i) == ' ' || (s.at(i) > 8 && s.at(i) < 14)))
    i++;
  while (i < s.size() && s.at(i) >= '0' && s.at(i) <= '9' && !guard) {
    *res *= 10;
    guard = __builtin_uaddl_overflow(*res, s.at(i) - '0', res);
    i++;
  }
  if (guard)
    return (0);
  return (i == s.size() );
}
