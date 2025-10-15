#ifndef PRIVMSG_COMMAND_HPP
#define PRIVMSG_COMMAND_HPP
#include <ExecutableCommand.hpp>

namespace Commands {
    class Privmsg : public ExecutableCommand {
    public:
        Privmsg(Core::User *Emitter, Core::Server *Context, CommandParser::MessageCommand *Raw);

        int ValidateInput(void);

        int Execute(void);
    };
}
#endif
