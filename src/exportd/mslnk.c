/*
 Copyright (c) 2010 Fizians SAS. <http://www.fizians.com>
 This file is part of Rozofs.

 Rozofs is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published
 by the Free Software Foundation; either version 3 of the License,
 or (at your option) any later version.

 Rozofs is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see
 <http://www.gnu.org/licenses/>.
 */

#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <rozofs/common/profile.h>
#include <rozofs/rpc/epproto.h>

#include "mslnk.h"

DECLARE_PROFILING(epp_profiler_t);

int mslnk_open(mslnk_t *mslnk, const char *path) {
    START_PROFILING(mslnk_open);
    mslnk->fdattrs = open(path, O_RDWR | O_NOATIME, S_IRWXU);
    STOP_PROFILING(mslnk_open);
    return mslnk->fdattrs < 0 ? -1 : 0;
}

void mslnk_close(mslnk_t *mslnk) {
    START_PROFILING(mslnk_close);
    close(mslnk->fdattrs);
    STOP_PROFILING(mslnk_close);
}

int mslnk_read_attributes(mslnk_t *mslnk, mattr_t *attrs) {
    int status;

    START_PROFILING(mslnk_read_attributes);

    status = pread(mslnk->fdattrs, attrs, sizeof(mattr_t), 0)
            == sizeof(mattr_t) ? 0 : -1;

    STOP_PROFILING(mslnk_read_attributes);
    return status;
}

int mslnk_write_attributes(mslnk_t *mslnk, mattr_t *attrs) {
    int status;

    START_PROFILING(mslnk_write_attributes);

    status = pwrite(mslnk->fdattrs, attrs, sizeof(mattr_t), 0)
            == sizeof(mattr_t) ? 0 : -1;

    STOP_PROFILING(mslnk_write_attributes);
    return status;
}

int mslnk_read_link(mslnk_t *mslnk, char *link) {
    char link_name[ROZOFS_PATH_MAX];

    START_PROFILING(mslnk_read_link);

    if (pread(mslnk->fdattrs, link_name, ROZOFS_PATH_MAX, sizeof(mattr_t))
            != ROZOFS_PATH_MAX)
        return -1;
    strcpy(link, link_name);

    STOP_PROFILING(mslnk_read_link);
    return 0;
}

int mslnk_write_link(mslnk_t *mslnk, char *link) {
    int status;
    char link_name[ROZOFS_PATH_MAX];

    START_PROFILING(mslnk_write_link);

    memset(link_name, 0, ROZOFS_PATH_MAX);
    strcpy(link_name, link);

    status = pwrite(mslnk->fdattrs, link_name, ROZOFS_PATH_MAX,
            sizeof(mattr_t))
            == ROZOFS_PATH_MAX ? 0 : -1;

    STOP_PROFILING(mslnk_write_link);

    return status;
}