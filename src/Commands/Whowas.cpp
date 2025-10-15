#include "Whowas.hpp"
#include <Channel.hpp>
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Whowas::Whowas(Core::User *Emitter,
                         Core::Server *Context,
                         CommandParser::MessageCommand *Raw)
    : ExecutableCommand(Emitter, Context, Raw) {
}

int
Commands::Whowas::ValidateInput(void) {
    if (!emitter->FullyRegistered() || (ctx->IsPasswordProtected() && !emitter->HasSentValidPassword()))
        return 1;
    if (!raw->HasArguments())
        return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
    if (!SetTargetUser(ctx->GetLatestUserWithNickname(raw->Argument(0))))
        return Replies::ERR_WASNOSUCHNICK(emitter, raw->Argument(0));
    return 0;
}

int
Commands::Whowas::Execute(void) {
    Replies::RPL_WHOWASUSER(emitter, targetUser);
    Replies::RPL_ENDOFWHOWAS(emitter, targetUser);
    return 0;
}
