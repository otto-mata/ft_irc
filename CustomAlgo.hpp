#ifndef CUSTOM_ALGO_HPP
#define CUSTOM_ALGO_HPP
#include <type_traits>

namespace Algo {

namespace Iter {

template<class Iterator, class UnaryPredicate>
bool
Contains(Iterator begin, const Iterator end, UnaryPredicate pred)
{
  while (begin != end) {
    if (pred(*begin))
      return true;
    ++begin;
  }
  return false;
}

} // namespace Iter

namespace String {

std::string
Sub(std::string::iterator begin, size_t size = ~0)
{
  std::string dup;
  const std::string::iterator end = begin + size;
  while (begin != end) {
    dup += *begin;
    ++begin;
  }
  return dup;
}
} // namespace String

} // namespace Algo

#endif
