#ifndef CUSTOM_ALGO_HPP
#define CUSTOM_ALGO_HPP
#include <string>
#include <vector>
#include <cctype>

namespace Algo {
    namespace Iter {
        template<class Iterator, class UnaryPredicate>
        bool
        Contains(Iterator begin, const Iterator end, UnaryPredicate pred) {
            while (begin != end) {
                if (pred(*begin))
                    return true;
                ++begin;
            }
            return false;
        }

        template<class Iterator, class UnaryPredicate, class Object>
        Object *
        FindFirst(Iterator begin, const Iterator end, UnaryPredicate pred) {
            Object *o = 0;
            while (begin != end) {
                o = pred(*begin);
                if (o)
                    return o;
                ++begin;
            }
            return o;
        }
    } // namespace Iter

    namespace Array {
        template<typename T>
        class Find {
        private:
            T obj;

        public:
            Find(T o)
                : obj(o) {
            }

            bool In(T *arr, size_t n) {
                for (size_t i = 0; i < n; i++) {
                    if (arr[i] == obj)
                        return true;
                }
                return false;
            }
        };
    } // namespace Array

    namespace String {
        std::vector<std::string>
        Split(std::string s, const std::string &delimiter);

        bool
        SaferStoul(std::string s, unsigned long *res);

        std::string
        EscapeSequence(std::string s);

        std::string
        ToLower(std::string s);
    } // namespace String
} // namespace Algo

#endif
