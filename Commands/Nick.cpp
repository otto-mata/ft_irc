
#include "Nick.hpp"
#include "../ExecutableCommand.hpp"
#include "../Replies/Replies.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Nick::Nick(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}
int
Commands::Nick::ValidateInput(void)
{
  if (ctx->IsPasswordProtected() && !emitter->HasSentValidPassword())
    return 1;
  if (!raw->HasArguments()) {
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  }
  const std::string& name = raw->Argument(0);
  if (ctx->MatchUserByNickname(name)) {
    return Replies::ERR_NICKNAMEINUSE(emitter, name);
  }
  return 0;
}

int
Commands::Nick::Execute(void)
{
  const std::string& name = raw->Argument(0);
  emitter->SetNickname(name);
  emitter->CompletedRegistrationRoutine(ctx->Hostname());
  return 0;
}
