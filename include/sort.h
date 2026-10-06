#ifndef SORT_H
#define SORT_H

#include "file_info.h"
#include "flags.h"

void sort_files(FileInfo **files, int count, const Options *opts);

#endif // SORT_H