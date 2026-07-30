#include "init_dev.h"

int main(void) {
    if (init_dev() != 0) {
        // handle error
    }
    // rest of startup
    return 0;
}
