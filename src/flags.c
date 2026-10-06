#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "flags.h"

void init_options(Options *opts)
{
    opts->flag_A = 0;
    opts->flag_a = 0;
    opts->flag_c = 0;
    opts->flag_d = 0;
    opts->flag_F = 0;
    opts->flag_f = 0;
    opts->flag_h = 0;
    opts->flag_i = 0;
    opts->flag_k = 0;
    opts->flag_l = 0;
    opts->flag_n = 0;
    opts->flag_q = 0;
    opts->flag_R = 0;
    opts->flag_r = 0;
    opts->flag_S = 0;
    opts->flag_s = 0;
    opts->flag_t = 0;
    opts->flag_u = 0;
    opts->flag_w = 0;
}

int parse_flags(int argc, char *argv[], Options *opts, int *opt_ind)
{
    init_options(opts);
    int opt;

    // Tắt thông báo lỗi mặc định của getopt
    opterr = 0;

    while ((opt = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1)
    {
        switch (opt)
        {
        case 'A':
            opts->flag_A = 1;
            break;
        case 'a':
            opts->flag_a = 1;
            break;
        case 'c':
            opts->flag_c = 1;
            opts->flag_u = 0;
            break; // -c đè -u
        case 'd':
            opts->flag_d = 1;
            opts->flag_R = 0;
            break; // -d đè -R
        case 'F':
            opts->flag_F = 1;
            break;
        case 'f':
            opts->flag_f = 1;
            opts->flag_a = 1;
            break; // -f bật luôn -a và không sắp xếp
        case 'h':
            opts->flag_h = 1;
            opts->flag_k = 0;
            break; // -h đè -k
        case 'i':
            opts->flag_i = 1;
            break;
        case 'k':
            if (!opts->flag_h)
                opts->flag_k = 1;
            break;
        case 'l':
            opts->flag_l = 1;
            opts->flag_n = 0;
            break; // -l đè -n
        case 'n':
            opts->flag_n = 1;
            opts->flag_l = 0;
            break; // -n đè -l
        case 'q':
            opts->flag_q = 1;
            opts->flag_w = 0;
            break; // -q đè -w
        case 'R':
            if (!opts->flag_d)
                opts->flag_R = 1;
            break;
        case 'r':
            opts->flag_r = 1;
            break;
        case 'S':
            opts->flag_S = 1;
            break;
        case 's':
            opts->flag_s = 1;
            break;
        case 't':
            opts->flag_t = 1;
            break;
        case 'u':
            opts->flag_u = 1;
            opts->flag_c = 0;
            break; // -u đè -c
        case 'w':
            opts->flag_w = 1;
            opts->flag_q = 0;
            break; // -w đè -q
        default:
            fprintf(stderr, "ls: illegal option -- %c\n", optopt);
            return -1;
        }
    }
    *opt_ind = optind;
    return 0;
}