#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "define.hpp"
#include "engine.hpp"

// Check target is not 32bit
#ifdef OS_32BIT
    #error Engine is not should works under 32bit system.
#endif