#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>

#if defined(__linux__)
#include <sys/sysmacros.h>
#endif

// Tự định nghĩa macro major/minor nếu biên dịch trên môi trường thiếu
#ifndef major
#define major(dev) ((int)(((dev) >> 8) & 0xff))
#endif
#ifndef minor
#define minor(dev) ((int)((dev) & 0xff))
#endif

#include "display.h"

// Chuyển st_mode thành chuỗi quyền hạn 10 ký tự (-rwxr-xr-x)
void format_permissions(mode_t mode, char *str)
{
    if (S_ISREG(mode))
        str[0] = '-';
    else if (S_ISDIR(mode))
        str[0] = 'd';
    else if (S_ISLNK(mode))
        str[0] = 'l';
    else if (S_ISCHR(mode))
        str[0] = 'c';
    else if (S_ISBLK(mode))
        str[0] = 'b';
    else if (S_ISFIFO(mode))
        str[0] = 'p';
    else if (S_ISSOCK(mode))
        str[0] = 's';
    else
        str[0] = '?';

    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    if (mode & S_ISUID)
        str[3] = (mode & S_IXUSR) ? 's' : 'S';
    else
        str[3] = (mode & S_IXUSR) ? 'x' : '-';

    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    if (mode & S_ISGID)
        str[6] = (mode & S_IXGRP) ? 's' : 'S';
    else
        str[6] = (mode & S_IXGRP) ? 'x' : '-';

    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    if (mode & S_ISVTX)
        str[9] = (mode & S_IXOTH) ? 't' : 'T';
    else
        str[9] = (mode & S_IXOTH) ? 'x' : '-';

    str[10] = '\0';
}

// Xử lý cờ -h (Kích thước dễ đọc KB, MB, GB)
void format_human_size(off_t size, char *buf, size_t buf_size)
{
    const char *units[] = {"B", "K", "M", "G", "T", "P"};
    double dsize = (double)size;
    int unit_idx = 0;

    while (dsize >= 1024.0 && unit_idx < 5)
    {
        dsize /= 1024.0;
        unit_idx++;
    }

    if (unit_idx == 0)
    {
        snprintf(buf, buf_size, "%lldB", (long long)size);
    }
    else if (dsize < 10.0)
    {
        snprintf(buf, buf_size, "%.1f%s", dsize, units[unit_idx]);
    }
    else
    {
        snprintf(buf, buf_size, "%.0f%s", dsize, units[unit_idx]);
    }
}

static void print_name(const char *name, const Options *opts)
{
    for (int i = 0; name[i] != '\0'; i++)
    {
        unsigned char c = (unsigned char)name[i];
        if (!isprint(c) && !opts->flag_w)
        {
            putchar('?'); // Cờ -q hoặc mặc định
        }
        else
        {
            putchar(c);
        }
    }
}

// Xử lý cờ -F
static void print_suffix(mode_t mode, const Options *opts)
{
    if (!opts->flag_F)
        return;
    if (S_ISDIR(mode))
        putchar('/');
    else if (S_ISLNK(mode))
        putchar('@');
    else if (S_ISSOCK(mode))
        putchar('=');
    else if (S_ISFIFO(mode))
        putchar('|');
    else if (mode & (S_IXUSR | S_IXGRP | S_IXOTH))
        putchar('*');
}

void display_files(FileInfo **files, int count, const Options *opts, const char *path_prefix)
{
    if (count == 0)
        return;

    if (path_prefix && strlen(path_prefix) > 0)
    {
        printf("%s:\n", path_prefix);
    }

    // In tổng số blocks nếu dùng -l, -n hoặc -s
    if (opts->flag_l || opts->flag_n || opts->flag_s)
    {
        long long total_blocks = 0;
        for (int i = 0; i < count; i++)
        {
            total_blocks += files[i]->st.st_blocks;
        }
        if (opts->flag_k)
        {
            printf("total %lld\n", (total_blocks + 1) / 2);
        }
        else
        {
            printf("total %lld\n", total_blocks);
        }
    }

    for (int i = 0; i < count; i++)
    {
        FileInfo *f = files[i];

        if (opts->flag_i)
            printf("%llu ", (unsigned long long)f->st.st_ino);
        if (opts->flag_s)
        {
            long long blocks = opts->flag_k ? (f->st.st_blocks + 1) / 2 : f->st.st_blocks;
            printf("%lld ", blocks);
        }

        // Định dạng danh sách dài (-l hoặc -n)
        if (opts->flag_l || opts->flag_n)
        {
            char perm[11];
            format_permissions(f->st.st_mode, perm);
            printf("%s %2u ", perm, (unsigned int)f->st.st_nlink);

            if (opts->flag_n)
            {
                printf("%4u %4u ", (unsigned int)f->st.st_uid, (unsigned int)f->st.st_gid);
            }
            else
            {
                printf("%s %s ", f->owner_name ? f->owner_name : "UNKNOWN",
                       f->group_name ? f->group_name : "UNKNOWN");
            }

            if (S_ISCHR(f->st.st_mode) || S_ISBLK(f->st.st_mode))
            {
                printf("%3u, %3u ", major(f->st.st_rdev), minor(f->st.st_rdev));
            }
            else if (opts->flag_h)
            {
                char hbuf[16];
                format_human_size(f->st.st_size, hbuf, sizeof(hbuf));
                printf("%5s ", hbuf);
            }
            else
            {
                printf("%8lld ", (long long)f->st.st_size);
            }

            struct tm *tm_info = localtime(&(f->active_time));
            char time_str[32];
            strftime(time_str, sizeof(time_str), "%b %e %H:%M", tm_info);
            printf("%s ", time_str);

            print_name(f->name, opts);
            print_suffix(f->st.st_mode, opts);

            if (S_ISLNK(f->st.st_mode) && f->link_target)
            {
                printf(" -> %s", f->link_target);
            }
            putchar('\n');
        }
        else
        {
            print_name(f->name, opts);
            print_suffix(f->st.st_mode, opts);
            putchar('\n');
        }
    }
}