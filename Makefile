NAME=ircserv
CXX=c++
DEPS=-MMD -MP
DEBUGF=-g -O0
FSANF=-fsanitize=address
CXXFLAGS=-Wall -Werror -Wextra -std=c++98 -Iincludes/ $(DEPS)

SRC := ./src/Commands/User.cpp \
	./src/Commands/Cap.cpp \
	./src/Commands/Invite.cpp \
	./src/Commands/Join.cpp \
	./src/Commands/Kick.cpp \
	./src/Commands/List.cpp \
	./src/Commands/Mode.cpp \
	./src/Commands/Nick.cpp \
	./src/Commands/Part.cpp \
	./src/Commands/Pass.cpp \
	./src/Commands/Ping.cpp \
	./src/Commands/Privmsg.cpp \
	./src/Commands/Quit.cpp \
	./src/Commands/Topic.cpp \
	./src/Commands/Pong.cpp \
	./src/Commands/Whois.cpp \
	./src/Commands/Whowas.cpp \
	./src/Commands/ExecutableCommand.cpp \
	./src/Common/Algo/EscapeSequence.cpp \
	./src/Common/Algo/SaferStoul.cpp \
	./src/Common/Algo/Split.cpp \
	./src/Core/Args/Args.cpp \
	./src/Core/Channel/Channel.cpp \
	./src/Core/Channel/ChannelPredicates.cpp \
	./src/Core/Server/Server.cpp \
	./src/Core/Server/ServerBroadcast.cpp \
	./src/Core/Server/ServerServices.cpp \
	./src/Core/Server/ServerStart.cpp \
	./src/Core/User/User.cpp \
	./src/Core/User/UserPredicates.cpp \
	./src/Logging/Logger.cpp \
	./src/Logging/Colors.cpp \
	./src/Parsing/MessageCommand.cpp \
	./src/Parsing/Token.cpp \
	./src/Parsing/Scanner.cpp \
	./src/Replies/Replies.cpp \
	./src/main.cpp

OBJDIR=./build
OBJ=$(addprefix $(OBJDIR)/,$(patsubst ./src/%,%,$(SRC:.cpp=.o)))

ifeq ($(DEBUG), 1)
CXXFLAGS += $(DEBUGF)
endif
ifeq ($(FSAN), 1)
CXXFLAGS += $(FSANF)
endif


all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@printf "\t'$@' compiled ($(CXX), $(CXXFLAGS), [$^])\n"

$(OBJDIR)/%.o: ./src/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<
	@printf "\t'$@' compiled\n"

clean:
	@rm -rf $(OBJDIR)
	@printf " > Removed '$(OBJDIR)'\n"

fclean: clean
	@rm -f $(NAME)
	@printf " > Removed '$(NAME)'\n"

re: fclean all

default: $(NAME)

-include $(OBJ:.o=.d)

.PHONY: all clean fclean re
