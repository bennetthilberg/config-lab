#include "emp/config/config.hpp"

EMP_BUILD_CONFIG(MyConfig,
                 VALUE(SEED, int, 10, "What value should the random seed be?"),
                 VALUE(START_POP_SIZE, int, 10,
                       "How many organisms should we start with?"));