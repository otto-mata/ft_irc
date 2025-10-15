#ifndef INVITE_COMMAND_HPP
#define INVITE_COMMAND_HPP
#include <ExecutableCommand.hpp>

namespace Commands {
    class Invite : public ExecutableCommand {
    public:
        Invite(Core::User *Emitter, Core::Server *Context, CommandParser::MessageCommand *Raw);

        int ValidateInput(void);

        int Execute(void);
    };
}
#endif
