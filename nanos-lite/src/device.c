#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  char *b=(char*)buf;
  for(int i=0;i<len;i++){
    _putc(*(b++));
  }
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  int key=read_key();
  //if(key & 0x8000){
    //sprintf(buf,"kd %s\n",keyname[key & ~0x8000]);
  //}
  //else{
    //if((key & ~0x8000)==_KEY_NONE){
    //  sprintf(buf,"t %d\n",uptime());
    //}
    //else 
    if(key & 0x8000){
        printf("aaaaaaaaaaaaaaaaa\n");
        sprintf(buf,"kd %s\n",keyname[key & ~0x8000]);
    }
    else if ((key & ~0x8000)!=_KEY_NONE){
      printf("ddddddd\n");
        sprintf(buf,"ku %s\n",keyname[key & ~0x8000]);
    }
    else{
      sprintf(buf,"t %d\n",uptime());
    }
  return strlen(buf);
}
static char dispinfo[128] __attribute__((used)) = {};

size_t discinfo_sizes(){
  int len=strlen(dispinfo);
  return len;
}
size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  strncpy(buf,dispinfo+offset,len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int x=(offset/4)%screen_width(); 
  int y=(offset/4)/screen_width();
  draw_rect((uint32_t*)buf,x,y,len/4,1);
  return len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  draw_sync();
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_width(),screen_height());
}
