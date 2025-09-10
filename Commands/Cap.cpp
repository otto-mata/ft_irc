
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
  if (!raw->HasArguments() || raw->Arguments().size() == 0) {
    Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
    return 461; //! Invalid arguments count (not enough)
  }
  const char* subs[] = {
    "LS", "LIST", "REQ", "ACK", "NAK", "END", "NEW", "DEL",
  };
  bool match = false;
  for (int i = 0; i < 8; i++) {
    if (raw->Argument(0) == std::string(subs[i])) {
      match = true;
      break;
    }
  }
  if (!match)
    return Replies::ERR_UNKNOWNCOMMAND(
      emitter, std::string(raw->Name() + " " + raw->Argument(0)));
  return 0;
}

int
Commands::Cap::Execute(void)
{
  if (raw->Argument(0) != "END")
    emitter->PendingCapabilitiesNegotiation();
  else {
    emitter->FinishCapabilitiesNegotiation();
    emitter->CompletedRegistrationRoutine(ctx->Hostname());
  }
  if (raw->Argument(0) == "LS") {
    if (emitter->GetNickname().size() == 0)
      emitter->AppendToOutgoingBuffer(":" + ctx->Hostname() +
                                      " CAP * LS :\r\n");
    else
      emitter->AppendToOutgoingBuffer(":" + ctx->Hostname() + " CAP " +
                                      emitter->GetNickname() + " LS :\r\n");
  }
  return 0;
}
