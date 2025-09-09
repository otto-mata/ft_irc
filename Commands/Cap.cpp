
#include "Cap.hpp"
#include "../CustomAlgo.hpp"
#include "../ExecutableCommand.hpp"
#include "../Replies/Replies.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Cap::Cap(Core::User* Emitter,
                   Core::Server* Context,
                   CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Cap::ValidateInput(void)
{
  if (raw->Arguments().size() == 0) {
    Replies::SendReply461ToUserForCommand(emitter, raw->Name());
    return 461; //! Invalid arguments count (not enough)
  }
  const char* subs[] = {
    "LS", "LIST", "REQ", "ACK", "NAK", "END", "NEW", "DEL",
  };
  if (!Algo::Array::Find<const char*>(raw->Argument(0).c_str())
         .In(subs, sizeof(subs) / sizeof(subs[0])))
    return 2;
  return 0;
}

int
Commands::Cap::Execute(void)
{
  if (raw->Argument(0) != "END")
    emitter->PendingCapabilitiesNegotiation();
  else
    emitter->FinishCapabilitiesNegotiation();
  if (raw->Argument(0) == "LS") {
    if (emitter->GetNickname().size() == 0)
      emitter->AppendToOutgoingBuffer("CAP * LS :\r\n");
    else
      emitter->AppendToOutgoingBuffer("CAP " + emitter->GetNickname() +
                                      " LS :\r\n");
  }
  return 0;
}
