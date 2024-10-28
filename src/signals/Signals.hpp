#pragma once

//this file handles default signals, for example making sure the app is terminated properly
#include <csignal>
#include <cstdlib>
#include <string>
#include "../log/Logger.hpp"


void sig_handler(int sig);