#ifndef __FS_H__
#define __FS_H__

#include "common.h"

#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif

#endif
int32_t fs_open(const char* pathname,int flags,int mode);
int32_t fs_read(int fd,void*buf,size_t len);
int fs_close(int fd);
uint32_t fs_lseek(int fd,size_t offset,int whence);
int32_t fs_write(int fd,void *buf,size_t len);