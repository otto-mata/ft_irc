
#include "Pass.hpp"
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Pass::Pass(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Pass::ValidateInput(void)
{
  if (!raw->HasArguments())
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (emitter->HasSentValidPassword())
    return Replies::ERR_ALREADYREGISTRED(emitter);
  return 0;
}

int
Commands::Pass::Execute(void)
{
  emitter->SetPasswordReceived(true);
  if (!emitter->SetPasswordValid(ctx->TryPassword(raw->Argument(0))))
    return Replies::ERR_PASSWDMISMATCH(emitter);
  return 0;
}
