#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define PATH "/home/wz/c/myc/"

int main(void)
{
    DIR *dir = opendir(PATH);
    if (!dir) {
        perror("opendir");
        return 1;
    }

    struct dirent *drt;
    char out[2048] = {0};
    size_t off = 0;

    while ((drt = readdir(dir)) != NULL) {
        if (strcmp(drt->d_name, ".") == 0 || strcmp(drt->d_name, "..") == 0)
            continue;

        if (drt->d_type == DT_DIR || drt->d_type == DT_REG) {
            int n = snprintf(out + off, sizeof(out) - off,
                             "%s   ", drt->d_name);
            if (n < 0 || (size_t)n >= sizeof(out) - off) {
                fprintf(stderr, "out buffer full\n");
                break;
            }
            off += (size_t)n;
        }
    }

    printf("%s\n", out);

    closedir(dir);
    return 0;
}
