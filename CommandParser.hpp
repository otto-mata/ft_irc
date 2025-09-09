#ifndef MESSAGE_COMMAND_HPP
#define MESSAGE_COMMAND_HPP
#include "Exceptions.hpp"
#include <string>
#include <vector>

class ExecutableCommand;
namespace Core {
class Server;
class User;
} // namespace Core

namespace CommandParser {

typedef enum
{
  STRING,
  COLON,
  SPACE,
  EOL
} TokenType;

class Token
{
private:
  TokenType _type;
  std::string _lex;

public:
  Token(TokenType type, std::string lexeme);
  ~Token();
  std::string toString(void);
  TokenType getType(void);
  const std::string& getLexeme(void);
};

class Scanner
{
private:
  std::vector<Token> _tokens;
  std::string _source;
  unsigned long _start;
  unsigned long _curr;
  bool isAtEnd(void);
  void scanToken(void);
  char advance(void);
  char peek(void);
  void addStringToken(void);
  void addToken(TokenType t);

public:
  Scanner(std::string source);
  ~Scanner();
  std::vector<Token> scanTokens(void);
};

class MessageCommand
{
private:
  std::string* source;
  std::string cmd;
  std::vector<std::string>* params;
  std::string* trailing;

public:
  MessageCommand(const std::string& arg);
  ~MessageCommand();
  const std::string& Name(void);
  const std::vector<std::string>& Arguments(void);
  const std::string& Argument(size_t);
  const std::string& Trailing(void);
  std::string ToString(void);
  ExecutableCommand* ToExecutable(
    Core::User* Emitter,
    Core::Server* Context); // CompileWarning("Not implemented yet.");
};

} // namespace CommandParser

#endif
