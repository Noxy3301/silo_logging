#pragma once

#include <vector>

#include "logger.h"

void worker_th(int thID, char &ready, const bool &start, const bool &quit, std::atomic<Logger*> *logp);