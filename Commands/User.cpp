
#include "User.hpp"
#include "../ExecutableCommand.hpp"
#include "../Replies/Replies.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::User::User(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::User::ValidateInput(void)
{
  if (raw->Arguments().size() != 3 || !raw->HasTrailing())
    return Replies::SendReply461ToUserForCommand(emitter, raw->Name());
  return 0;
}

int
Commands::User::Execute(void)
{
  TRACE_CALL
  emitter->SetUsername(raw->Argument(0));
  emitter->SetRealName(raw->Trailing());
  emitter->CompletedRegistrationRoutine(ctx->Hostname());
  return 0;
}
