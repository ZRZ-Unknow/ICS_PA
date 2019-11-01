#include <am.h>
#include <amdev.h>
#include <nemu.h>
#include <klib.h>
#define SCREEN_PORT 0x100
static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      uint32_t vr=inl(SCREEN_PORT);
      info->width = vr >> 16;
      info->height = vr & 0xffff;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
      int x=ctl->x,y=ctl->y,w=ctl->w,h=ctl->h;
      uint32_t *pixels=ctl->pixels;
      int W=screen_width(),H=screen_height();
      int cp_bytes =sizeof(uint32_t)*(w<W-x?w:W-x);
      for(int i=0;i<h&&y+i<H;i++){
        memcpy(&fb[(y+i)*W+x],pixels,cp_bytes);
        pixels+=w;
      }
      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
  /*int i;
  int size=screen_width()*screen_height();
  uint32_t *fb=(uint32_t *)(uintptr_t)FB_ADDR;
  for(i=0;i<size;i++){
    fb[i]=i;
  }
  draw_sync();*/
}
