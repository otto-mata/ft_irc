#ifndef PONG_COMMAND_HPP
#define PONG_COMMAND_HPP
#include <ExecutableCommand.hpp>

namespace Commands {
    class Pong : public ExecutableCommand {
    public:
        Pong(Core::User *Emitter, Core::Server *Context, CommandParser::MessageCommand *Raw);

        int ValidateInput(void);

        int Execute(void);
    };
}
#endif
