#include <cstdio>

#include <tachyon/tachyon.h>

int main(int argc, char **argv)
{
    using namespace tachyon;

    try {
        return 0;
    } catch (Exception &ex) {
        fprintf(stderr, "unhandled exception: %s\n", ex.what());
        return -1;
    }
}
