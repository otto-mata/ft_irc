#include "List.hpp"
#include <Channel.hpp>
#include <CustomAlgo.hpp>
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>
#include <sstream>

Commands::List::List(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::List::ValidateInput(void)
{
  if (raw->HasArguments()) {
    std::string argCpy(raw->Argument(0));
    std::vector<std::string> v = Algo::String::Split(argCpy, ",");
    for (std::vector<std::string>::iterator it = v.begin(); it != v.end();
         it++) {
      if ((*it).at(0) != '#')
        return Replies::ERR_NOSUCHCHANNEL(emitter, *it);
    }
  }
  return 0;
}

int
Commands::List::Execute(void)
{
  typedef std::vector<std::string> ChanNames;

  ChanNames channels;

  if (!raw->HasArguments())
    channels = ctx->GetAllChannelNames();
  else {
    ChanNames v =
      Algo::String::Split(std::string(raw->Argument(0)), ",");
    for (ChanNames::iterator it = v.begin(); it != v.end();
         it++) {
      channels.push_back((*it).substr(1));
    }
  }
  for (ChanNames::iterator it = channels.begin(); it != channels.end(); it++) {
    Core::Channel* chan = ctx->GetChannel(*it);
    std::ostringstream os;
    os << ":" << ctx->Hostname() << " 322 " << emitter->GetNickname() << " "
       << chan->GetName() << " " << chan->GetUsers().size() << " :"
       << chan->GetTopic();
    emitter->AppendToOutgoingBuffer(os.str());
  }
  emitter->AppendToOutgoingBuffer(":" + ctx->Hostname() + " " +
                                  emitter->GetNickname() + " :End of list");
  return 0;
}
