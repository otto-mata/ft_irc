#include <CustomAlgo.hpp>

static std::string
replaceOccurenceInString(std::string str, std::string from, std::string to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

std::string
Algo::String::EscapeSequence(std::string s) {
    s = replaceOccurenceInString(s, "\a", "\\a");
    s = replaceOccurenceInString(s, "\b", "\\b");
    s = replaceOccurenceInString(s, "\t", "\\t");
    s = replaceOccurenceInString(s, "\n", "\\n");
    s = replaceOccurenceInString(s, "\v", "\\v");
    s = replaceOccurenceInString(s, "\f", "\\f");
    s = replaceOccurenceInString(s, "\r", "\\r");
    return s;
}
