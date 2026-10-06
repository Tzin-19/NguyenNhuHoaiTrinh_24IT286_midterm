#ifndef DISPLAY_H
#define DISPLAY_H

#include "file_info.h"
#include "flags.h"

void display_files(FileInfo **files, int count, const Options *opts, const char *path_prefix);
void format_permissions(mode_t mode, char *str);
void format_human_size(off_t size, char *buf, size_t buf_size);

#endif // DISPLAY_H