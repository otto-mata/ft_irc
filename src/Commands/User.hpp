#ifndef USER_COMMAND_HPP
#define USER_COMMAND_HPP
#include <ExecutableCommand.hpp>

namespace Commands {
    class User : public ExecutableCommand {
    public:
        User(Core::User *Emitter, Core::Server *Context, CommandParser::MessageCommand *Raw);

        int ValidateInput(void);

        int Execute(void);
    };
}
#endif
