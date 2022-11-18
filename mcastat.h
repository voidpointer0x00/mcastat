#ifndef MCASTAT_H
#define MCASTAT_H

extern struct mcastat_state {
    char *filename;
    union {
        struct {
            unsigned char chunk:1;
            unsigned char sector:1;
            unsigned char correlation:1;
            unsigned char quiet:1;
        };
        unsigned int flags;
    };
} mcastat_state;

/**
 * mcastat reads the given .mca file and displays
 *  status based on selected mcastat_state.flags.
 */
int mcastat();

#endif //MCASTAT_H
