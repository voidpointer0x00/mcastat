#include <argp.h>
#include "mcastat.h"

const char *argp_program_version = "mcastat 1.0.0";
const char *argp_program_bug_address = "<void.pointer@ya.ru>";

enum key_options {
    N_CHUNK_OPT = 'c',
    N_SECTOR_OPT = 's',
    SECTOR_PER_CHUNK = 'p',
    QUIET = 'q',
};

const char args_doc[] = "r.X.Z.mca";
const char doc[] = "Display information about MCA region. If multiple options provided, the mcastat"
                   "displays them in the following order: chunk->sector->correlation, one per line.";

static struct argp_option options[] = {
        {"chunk", N_CHUNK_OPT, 0, 0, "prints total number of chunk"},
        {"sector", N_SECTOR_OPT, 0, 0, "prints total number of sectors"},
        {"correlation", SECTOR_PER_CHUNK, 0, 0, "prints correlation between chunk and sector"},
        {"quiet", QUIET, 0, 0, "with this option the program only prints numbers"},
        {} /* fixed weird bug when unknown option appears in usage & help messages */
};

error_t parse_opt(int key, char *arg, struct argp_state *state);

const static struct argp argp = {
    .options = options,
    .parser = &parse_opt,
    .args_doc = args_doc,
    .doc = doc,
    .children = 0,
    .help_filter = 0,
    .argp_domain = 0
};

int main(int argc, char **argv)
{
    error_t err = argp_parse(&argp, argc, argv, 0, 0, 0);
    if (err)
        return err;
    return mcastat();
}

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    switch (key) {
        case N_CHUNK_OPT:
            mcastat_state.chunk = 0b1;
            break;
        case N_SECTOR_OPT:
            mcastat_state.sector = 0b1;
            break;
        case SECTOR_PER_CHUNK:
            mcastat_state.correlation = 0b1;
            break;
        case QUIET:
            mcastat_state.quiet = 0b1;
            break;
        case ARGP_KEY_ARG:
            mcastat_state.filename = arg;
            break;
        case ARGP_KEY_INIT:
        case ARGP_KEY_NO_ARGS:
            break;
        case ARGP_KEY_END:
            if (!mcastat_state.filename)
                argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
