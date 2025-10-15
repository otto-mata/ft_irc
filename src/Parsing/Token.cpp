#include "CommandParser.hpp"

CommandParser::Token::Token(TokenType type, std::string lexeme)
    : _type(type)
      , _lex(lexeme) {
}

CommandParser::Token::~Token() {
}

std::string
CommandParser::Token::toString(void) {
    std::string t;

    switch (_type) {
#define ENUMSW(X) case X: t.assign(#X); break;
        ENUMSW(STRING)
        ENUMSW(COLON)
        ENUMSW(SPACE)
        ENUMSW(EOL)
#undef ENUMSW
    }
    return t + " " + _lex;
}

CommandParser::TokenType
CommandParser::Token::getType(void) {
    return _type;
}

const std::string &
CommandParser::Token::getLexeme(void) {
    return _lex;
}
