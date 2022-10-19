#include <vector>

#include "include/cache_line_size.h"
#include "include/result.h"
#include "include/common.h"

alignas(CACHE_LINE_SIZE) std::vector<ResultLog> SiloResult;
void initResult() { SiloResult.resize(THREAD_NUM); }