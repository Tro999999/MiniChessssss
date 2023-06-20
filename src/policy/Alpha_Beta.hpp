#ifndef __ALPHA_BETA_H__
#define __ALPHA_BETA_H__

#include <string>
#include <cstdlib>
#include <vector>
#include <utility>
#include "../state/state.hpp"

#include "../config.hpp"

class Alpha_Beta{
    public:
        static int alpha_beta(State* node, int depth, int Alpha, int Beta, int myself);
        static Move get_move(State *state, int depth);
};

#endif