#include "CommandParser.hpp"
#include "Exceptions.hpp"
#include "ExecutableCommand.hpp"
#include "Server.hpp"
#include "Commands/Commands.hpp"

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

ExecutableCommand*
CommandParser::MessageCommand::ToExecutable(User* Emitter, Server* Context)
{
  switch (Name()[0])
  {
  case 'C':
    if (Name() == "CAP")
      return new Commands::Cap(Emitter, Context, this);
    break;
  case 'I':
    if (Name() == "INVITE")
      return new Commands::Invite(Emitter, Context, this);
    break;
  case 'J':
    if (Name() == "JOIN")
      return new Commands::Join(Emitter, Context, this);
    break;
  case 'K':
    if (Name() == "KICK")
      return new Commands::Kick(Emitter, Context, this);
    break;
  case 'L':
    if (Name() == "LIST")
      return new Commands::List(Emitter, Context, this);
    break;
  case 'M':
    if (Name() == "MODE")
      return new Commands::Mode(Emitter, Context, this);
    break;
  case 'N':
    if (Name() == "NICK")
      return new Commands::Nick(Emitter, Context, this);
    else if (Name() == "NOTICE")
      return new Commands::Notice(Emitter, Context, this);
    break;
  case 'P':
    if (Name() == "PART")
      return new Commands::Part(Emitter, Context, this);
    if (Name() == "PASS")
      return new Commands::Pass(Emitter, Context, this);
    if (Name() == "PING")
      return new Commands::Ping(Emitter, Context, this);
    if (Name() == "PONG")
      return new Commands::Pong(Emitter, Context, this);
    if (Name() == "PRIVMSG")
      return new Commands::Privmsg(Emitter, Context, this);
    break;
  case 'Q':
    if (Name() == "QUIT")
      return new Commands::Quit(Emitter, Context, this);
    break;
  case 'T':
    if (Name() == "TOPIC")
      return new Commands::Topic(Emitter, Context, this);
    break;
  case 'W':
    if (Name() == "WHOIS")
      return new Commands::Whois(Emitter, Context, this);
    if (Name() == "WHOWAS")
      return new Commands::Whowas(Emitter, Context, this);
    break;
  
  default:
    return 0;
    break;
  }
}
