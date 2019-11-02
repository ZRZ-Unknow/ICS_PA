#include <am.h>
#include <amdev.h>
#include <nemu.h>
#include <klib.h>
#define SCREEN_PORT 0x100
#define W 400
#define H 300

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      //uint32_t vr=inl(SCREEN_PORT);
      info->width = W;
      info->height = H;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
      uint32_t *fb=(uint32_t *)(uintptr_t)FB_ADDR;

      int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
      uint32_t *pixels = ctl->pixels;
      int cp_bytes = sizeof(uint32_t) * (w<W-x?w:W-x);
      for (int j = 0; j < h && y + j < H; j ++) {
        memcpy(&fb[(y + j) * W + x], pixels, cp_bytes);
        pixels += w;
      }

//      uint32_t *fb=(uint32_t *)(uintptr_t)FB_ADDR;
      //for(int i=0;i<ctl->h;i++){
      //  memcpy(&fb[(i+ctl->y)*screen_width()+ctl->x],&ctl->pixels[i*ctl->w],ctl->w*4);
      //}
      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
  int i;
  int size=screen_width()*screen_height();
  uint32_t *fb=(uint32_t *)(uintptr_t)FB_ADDR;
  for(i=0;i<size;i++){
    fb[i]=i;
  }
  draw_sync();
}
