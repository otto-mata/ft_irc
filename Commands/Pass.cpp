
#include "Pass.hpp"
#include "../ExecutableCommand.hpp"
#include "../Replies/Replies.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Pass::Pass(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Pass::ValidateInput(void)
{
  if (raw->Arguments().empty())
    return Replies::SendReply461ToUserForCommand(emitter, raw->Name());
  if (emitter->HasSentValidPassword())
    return Replies::SendReply462ToUser(emitter);
  return 0;
}

int
Commands::Pass::Execute(void)
{
  emitter->SetPasswordReceived(true);
  emitter->SetPasswordValid(ctx->TryPassword(raw->Argument(0)));
  return 0;
}
