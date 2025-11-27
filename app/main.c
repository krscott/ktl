#include "ktl.h"
#include "kcli.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

struct opts
{
    double a;
    double b;
    bool verbose;
};

static struct opts opts_parse(int const argc, char const *const *const argv)
{
    struct opts opts = {0};

    KCLI_PARSE(
        argc,
        argv,
        {
            .pos_name = "a",
            .ptr_double = &opts.a,
            .help = "First number",
        },
        {
            .pos_name = "b",
            .ptr_double = &opts.b,
            .help = "Second number",
        },
        {
            .short_name = 'v',
            .long_name = "verbose",
            .ptr_flag = &opts.verbose,
            .help = "Enable extra logging",
        },
    );

    return opts;
}

int main(int const argc, char const *const *const argv)
{
    struct opts opts = opts_parse(argc, argv);

    if (opts.verbose)
    {
        fprintf(stderr, "ktl: Adding %f + %f...\n", opts.a, opts.b);
    }

    double const sum = ktl_sum(opts.a, opts.b);

    printf("%f\n", sum);

    return isnan(sum) ? 1 : 0;
}
