#include "Whois.hpp"
#include <Channel.hpp>
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Whois::Whois(Core::User *Emitter,
                       Core::Server *Context,
                       CommandParser::MessageCommand *Raw)
    : ExecutableCommand(Emitter, Context, Raw) {
}

int
Commands::Whois::ValidateInput(void) {
    if (!emitter->FullyRegistered() || (ctx->IsPasswordProtected() && !emitter->HasSentValidPassword()))
        return 1;
    if (!raw->HasArguments())
        return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
    if (!SetTargetUserFromContext(raw->Argument(0)))
        return Replies::ERR_NOSUCHNICK(emitter, raw->Argument(0));
    return 0;
}

int
Commands::Whois::Execute(void) {
    Replies::RPL_WHOISUSER(emitter, targetUser);
    Replies::RPL_ENDOFWHOIS(emitter, targetUser);
    return 0;
}
