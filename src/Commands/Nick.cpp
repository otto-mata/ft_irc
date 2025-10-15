#include "Nick.hpp"
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

typedef std::set<Core::User *> UserSet;


Commands::Nick::Nick(Core::User *Emitter,
                     Core::Server *Context,
                     CommandParser::MessageCommand *Raw)
    : ExecutableCommand(Emitter, Context, Raw) {
}

int
Commands::Nick::ValidateInput() {
    if (ctx->IsPasswordProtected() && !emitter->HasSentValidPassword())
        return 1;
    if (!raw->HasArguments()) {
        return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
    }
    if (ctx->MatchUserByNickname(raw->Argument(0))) {
        return Replies::ERR_NICKNAMEINUSE(emitter, raw->Argument(0));
    }
    return 0;
}

int
Commands::Nick::Execute() {
    const std::string &name = raw->Argument(0);
    if (emitter->FullyRegistered()) {
        Core::ChannelMap joined = emitter->getJoinedChanels();
        UserSet to_send_to;
        const std::string broadcast(":" + emitter->FullIdentityString() + " NICK :" + name);

        if (joined.empty())
            emitter->AppendToOutgoingBuffer(broadcast);
        else
            for (Core::ChannelMap::iterator it = joined.begin(); it != joined.end(); ++it)
                to_send_to.insert(it->second->GetUsers().begin(), it->second->GetUsers().end());

        if (!to_send_to.empty())
            for (UserSet::iterator it = to_send_to.begin(); it != to_send_to.end(); ++it)
                (*it)->AppendToOutgoingBuffer(broadcast);
    }
    emitter->SetNickname(name);
    emitter->CompletedRegistrationRoutine(ctx->Hostname());
    ctx->LogNicknameChangeForUser(emitter);
    return 0;
}
