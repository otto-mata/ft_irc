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

template<class Iterator, class UnaryPredicate, class Object>
Object*
FindFirst(Iterator begin, const Iterator end, UnaryPredicate pred)
{
  Object* o = 0;
  while (begin != end) {
    o = pred(*begin);
    if (o)
      return o;
    ++begin;
  }
  return o;
}

} // namespace Iter


} // namespace Algo

#endif
