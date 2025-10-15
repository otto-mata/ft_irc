#include "User.hpp"
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::User::User(Core::User *Emitter,
                     Core::Server *Context,
                     CommandParser::MessageCommand *Raw)
    : ExecutableCommand(Emitter, Context, Raw) {
}

int
Commands::User::ValidateInput(void) {
    if (ctx->IsPasswordProtected() && !emitter->HasSentValidPassword())
        return 1;
    if (!raw->HasArguments() || raw->Arguments().size() != 3 || !raw->HasTrailing())
        return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
    if (emitter->FullyRegistered())
        return Replies::ERR_ALREADYREGISTRED(emitter);
    return 0;
}

int
Commands::User::Execute(void) {
    emitter->SetUsername(raw->Argument(0));
    emitter->SetRealName(raw->Trailing());
    emitter->CompletedRegistrationRoutine(ctx->Hostname());
    return 0;
}
