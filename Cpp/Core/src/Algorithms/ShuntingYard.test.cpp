#include "Core/Algorithms/ShuntingYard.h"

namespace ShuntingYard {
    namespace Tests {
        bool Run() {
            if (Evaluate<int>("1 + 1") != 2) return false;
            if (Evaluate<int>("1+1") != 2) return false;
            if (Evaluate<int>("2 * (3 + 4)") != 14) return false;
            if (Evaluate<int>("1 + 2 * 3") != 7) return false;
            if (Evaluate<int>("1+(2*(3+4))") != 15) return false;
            if (Evaluate<int>("((1 +2) * 3)+4") != 13) return false;
            if (Evaluate<int>("(1 + 2 + 3) * (4 + 5 + 6)") != 90) return false;

            return true;
        }
    }
}