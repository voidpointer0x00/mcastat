#include "mcastat.h"
#include "mca_region.h"
#include <stdlib.h>
#include <stdio.h>

struct mcastat_state mcastat_state = {.flags = 0};

static char *chunks_format[] = {
        "Chunks: %hu\n",
        "%hu\n"
};
static char *sectors_format[] = {
        "Sectors: %zu\n",
        "%zu\n"
};
static char *correlation_format[] = {
        "Correlation: %f\n",
        "%f\n"
};

int mcastat()
{
    region_header_t *header = read_region_header(mcastat_state.filename);
    uint16_t chunks_defined = 0;
    size_t sectors_used = 0;

    for (int index = 0; index < N_LOCATIONS_IN_REGION; index++) {
        if (!header->locations[index].defined)
            continue;
        chunks_defined++;
        sectors_used += header->locations[index].n_sectors;
    }

    if (!mcastat_state.flags || mcastat_state.chunk)
        printf(chunks_format[mcastat_state.quiet], chunks_defined);
    if (!mcastat_state.flags || mcastat_state.sector)
        printf(sectors_format[mcastat_state.quiet], sectors_used);
    if (!mcastat_state.flags || mcastat_state.correlation)
        printf(correlation_format[mcastat_state.quiet], (double) sectors_used / chunks_defined);

    return EXIT_SUCCESS;
}
