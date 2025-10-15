#include "Server.hpp"
#include <Logger.hpp>
#include "Args.hpp"
#include <signal.h>
#include <iostream>

Logging::Engine logger("Main");

void
control(__attribute_maybe_unused__ int n) {
    logger.info("CTRL-C received");
    Core::Server::StopServer();
}

int
main(int argc, char **argv) {
    Core::Arguments args(argc, argv);
    if (args.parseArgs())
        return (1);
    try {
        logger.debug("Starting server...");
        Core::Server srv(args.port, args.password);
        logger.debug("Setting up signal interceptors...");
        signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, control);
        srv.Start();
    } catch (const std::runtime_error &e) {
        logger.fatal(e.what());
    }
    return (0);
}
