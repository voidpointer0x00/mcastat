#include "mca_region.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static region_header_t region_header;

void resolve_linux_home(const char *filename, char *dest)
{
#ifdef __linux__
    if (*filename == '~') {
        sprintf(dest, "%s%s", getenv("HOME"), filename + 1);
    } else {
        sprintf(dest, "%s", filename);
    }
#else
    sprintf(dest, "%s", filename);
#endif
}

region_header_t *read_region_header(const char *file)
{
    char resolved_path[8192];
    resolve_linux_home(file, resolved_path);
    FILE* region_file = fopen(resolved_path, "r");
    if (errno) {
        fprintf(stderr, "%s %s\n", file, strerror(errno));
        exit(EXIT_FAILURE);
    }
    region_header.locations = calloc(N_LOCATIONS_IN_REGION, sizeof(location_t));
    size_t ret = fread(region_header.locations, sizeof(location_t), N_LOCATIONS_IN_REGION, region_file);
    fclose(region_file);

    if (ret != N_LOCATIONS_IN_REGION) {
        fprintf(stderr, "fread() failed: read %zu, expected %u\n", ret, N_LOCATIONS_IN_REGION);
        exit(EXIT_FAILURE);
    }

    return &region_header;
}
