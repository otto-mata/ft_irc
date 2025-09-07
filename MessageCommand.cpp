#include "CommandParser.hpp"
#include "Exceptions.hpp"
#include "ExecutableCommand.hpp"
#include "Server.hpp"

CommandParser::MessageCommand::MessageCommand(const std::string& arg)
{
  std::vector<Token> tv = Scanner(arg).scanTokens();
  size_t i = 0;
  source = 0;
  params = 0;
  trailing = 0;
  cmd.assign("");

  for (std::vector<Token>::iterator it = tv.begin(); it < tv.end(); it++) {
    if (it->getType() == SPACE)
      continue;
    if (i == 0 && it->getType() == COLON)
      source = new std::string((++it)->getLexeme());
    else if ((i == 0 || i == 1) && it->getType() == STRING &&
             cmd.length() == 0) {
      cmd = it->getLexeme();
    } else if (it->getType() == COLON) {
      trailing = new std::string();
      while (it < tv.end()) {
        if (it->getType() == STRING)
          trailing->append(it->getLexeme());
        else if (it->getType() == SPACE)
          trailing->append(" ");
        it++;
      }
    } else if (it->getType() != EOL) {
      if (!params)
        params = new std::vector<std::string>();
      if (it->getType() == STRING)
        params->push_back(it->getLexeme());
    }
    i++;
  }
}

CommandParser::MessageCommand::~MessageCommand()
{
  if (source)
    delete source;
  if (params)
    delete params;
  if (trailing)
    delete trailing;
}

const std::string&
CommandParser::MessageCommand::Name(void)
{
  return cmd;
}

const std::vector<std::string>&
CommandParser::MessageCommand::Arguments(void)
{
  return *params;
}

const std::string&
CommandParser::MessageCommand::Argument(size_t index)
{
  return params->at(index);
}

const std::string&
CommandParser::MessageCommand::Trailing(void)
{
  return *trailing;
}

std::string
CommandParser::MessageCommand::ToString(void)
{
  std::string s;

  s = "COMMAND: " + cmd + "\n";
  if (params) {
    s += "PARAMS: [";
    for (std::vector<std::string>::iterator it = params->begin();
         it < params->end();
         it++) {
      s += *it;
      if (it < params->end() - 1)
        s += ", ";
    }
    s += "]\n";
  }
  if (source) {
    s += "SOURCE: " + *source + "\n";
  }
  if (trailing) {
    s += "TRAILING: " + *trailing + "\n";
  }
  return s;
}

_GLIBCXX_NORETURN ExecutableCommand*
CommandParser::MessageCommand::ToExecutable(
  __attribute_maybe_unused__ Server* ctx)
{
  // TODO: Create the instance of the command subclass
  // TODO: Those have to be defined.
  throw Exceptions::NotImplementedException();
}
