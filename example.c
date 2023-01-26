#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "module/sample.h"

int main() {
    // open the module
    int fd = open(MODULE_DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error: Could not open module: %s\n", MODULE_DEVICE_PATH);
        return -1;
    }
    
    size_t val = 3;
    printf("Val (before): %zd\n", val);
    // call into module to increment 'val'
    int ret = ioctl(fd, MODULE_IOCTL_CMD_INC, (unsigned long)&val);
    printf("Val (after):  %zd\n", val);

    close(fd);
}
