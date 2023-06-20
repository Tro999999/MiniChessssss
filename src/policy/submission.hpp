#ifndef __SUBMISSION_H__
#define __SUBMISSION_H__

#include <string>
#include <cstdlib>
#include <vector>
#include <utility>
#include "../state/state.hpp"

#include "../config.hpp"

class submission{
    public:
        static int Submission(State* node, int depth, int Alpha, int Beta, int myself);
        static Move get_move(State *state, int depth);
};

#endif