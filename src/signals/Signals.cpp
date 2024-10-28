#include "Signals.hpp"

//this file handles default signals, for example making sure the app is terminated properly
#include <csignal>
#include <cstdlib>
#include <string>


#include "../log/Logger.hpp"
#include "../transactions/TransactionManager.hpp"
#include "../messages/MessageHandler.hpp"
#include "../server/Server.hpp"
#include "../gossip/GossipServer.hpp"


void sig_handler(int sig) {
    std::string signalName = "Unknown";
    switch (sig) {
        case SIGSEGV:
            signalName = "SIGSEGV";
            break;
        case SIGABRT:
            signalName = "SIGABRT";
            break;
        case SIGFPE:
            signalName = "SIGFPE";
            break;
        case SIGILL:
            signalName = "SIGILL";
            break;
        case SIGINT:
            signalName = "SIGINT";
            break;
        case SIGTERM:
            signalName = "SIGTERM";
            break;
    }
    log(LOG_INFO, "Received signal " + signalName + ". Exiting.");

    std::signal(sig, SIG_DFL);

    try {
        stopServer();
        stopTransactionHandling();
        stopMessageExchangeQueue();
        stopGossipServer();
    } catch (const std::exception& e) {
        log(LOG_ERROR, std::string("Exception during cleanup: ") + e.what());
    } catch (...) {
        log(LOG_ERROR, "Unknown exception during cleanup.");
    }
    
    log(LOG_INFO, "Exiting...");
    _exit(1); // Exit immediately, maybe not all resources are freed and not the best way, but at this point all main cleanup is ready, we are just terminating the process, so we dont care, the os will clean up the mess
}