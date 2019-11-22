#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0,invalid_read, invalid_write},
  {"stdout", 0, 0, 0,invalid_read, invalid_write},
  {"stderr", 0, 0, 0,invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char* pathname,int flags,int mode){
  for(int i=0;i<NR_FILES;i++){
    if(strcmp(pathname,file_table[i].name)==0)
      return i;
  }
  panic("could not find file");
  return -1;
}

size_t fs_read(int fd,void*buf,size_t len){
  assert(fd<NR_FILES);
  size_t lens=len;
  if(file_table[fd].open_offset+len>file_table[fd].size){
    lens=file_table[fd].size-file_table[fd].open_offset;
  }
  if(file_table[fd].read==NULL){
    size_t offset=ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,lens); 
    file_table[fd].open_offset+=offset;
    return offset; 
  }
  else{
    size_t offset=file_table[fd].read(buf,file_table[fd].open_offset,lens);
    file_table[fd].open_offset+=offset;
    return offset;
  }
}
int fs_close(int fd){
  file_table[fd].open_offset=0;
  return 0;
}
size_t fs_write(int fd,void *buf,size_t len){
  assert(fd<NR_FILES);
  size_t lens=len;
  if(file_table[fd].open_offset+len>file_table[fd].size){
    lens=file_table[fd].size-file_table[fd].open_offset;
  }
  if(fd>2){
  //if(file_table[fd].write==NULL){
    size_t offset=ramdisk_write(buf,file_table[fd].disk_offset+file_table[fd].open_offset,lens); 
    file_table[fd].open_offset+=offset;
    return offset; 
  }
  else{
    size_t offset=file_table[fd].write(buf,file_table[fd].open_offset,lens);
    file_table[fd].open_offset+=offset;
    return offset;
  }

}
size_t fs_lseek(int fd,size_t offset,int whence){
  assert(fd<NR_FILES);
  switch (whence)
  {
  case SEEK_SET:
    file_table[fd].open_offset=offset;
    break;
  case SEEK_CUR:
    file_table[fd].open_offset=file_table[fd].size+offset;
    break;
  case SEEK_END:
    file_table[fd].open_offset=file_table[fd].size+offset;
    break;
  default:
    assert(0);
  }
  return file_table[fd].open_offset;
}