#ifndef JOIN_COMMAND_HPP
#define JOIN_COMMAND_HPP
#include <ExecutableCommand.hpp>

namespace Commands {
    class Join : public ExecutableCommand {
    public:
        Join(Core::User *Emitter, Core::Server *Context, CommandParser::MessageCommand *Raw);

        int ValidateInput(void);

        int Execute(void);
    };
}
#endif
