
#include "Nick.hpp"
#include "../ExecutableCommand.hpp"
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
  if (raw->Arguments().size() == 0) {
    // TODO: No parameters check
    return 1;
  }
  const std::string& name = raw->Argument(0);
  if (ctx->MatchUserByNickname(name)) {
    // TODO: Unavailable check
    return 2;
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
