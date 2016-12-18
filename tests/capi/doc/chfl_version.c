#include <chemfiles.h>

#include <string.h>
#include <assert.h>

int main() {
    // [example]
    const char* version = chfl_version();
    assert(strcmp(version, CHEMFILES_VERSION) == 0);
    // [example]
    return 0;
}
