#ifndef MCASTAT_MCA_REGION_H
#define MCASTAT_MCA_REGION_H

#define N_LOCATIONS_IN_REGION (1024)

#include <stdint.h>

typedef union location location_t;

typedef struct region_header {
    union location {
        struct {
            uint8_t offset[3];
            uint8_t n_sectors;
        };
        uint32_t defined;
    } *locations;
} region_header_t;

region_header_t* read_region_header(const char *file);

#endif //MCASTAT_MCA_REGION_H
