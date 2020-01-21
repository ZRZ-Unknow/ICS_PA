#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  va_list args;
  va_start(args,fmt);
  char out[1000];
  int out_len=vsprintf(out,fmt,args);
  va_end(args);
  for(int i=0;i<out_len;i++){
    _putc(out[i]);
  }
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *outp;int out_len=0;
  int width;
  int flags;
  char nums[1000];
  char *ss=nums;

  for(outp=out;*fmt;fmt++){
    if(*fmt!='%'){
      *outp++=*fmt;
      out_len++;
      continue;
    }
    int temp=1;
    flags=0;
    while(temp==1){
      fmt++;
      switch(*fmt){
        case '0':flags|=1;break;
        case '+':flags|=4;break;
        case ' ':flags|=8;break;
        case '-':flags|=16;break;
        case '#':flags|=32;break;
        default:temp=0;
      }
    }
    width=0;
    if(*fmt>='0'&&*fmt<='9'){
      for(;*fmt>='0'&&*fmt<='9';fmt++){
        width=width*10+*fmt-'0';
      }
    }
    else if(*fmt=='*'){
      fmt++;
      width=va_arg(ap,int);
      if(width<0){
        width=-width;
        flags|=16;
      }
    }
    switch(*fmt){
      case 'd':break;
      case 's':{
        char *s=va_arg(ap,char *);
        int len_s=strlen(s);
        if(!(flags&16)){
          while(len_s<width--){
            *outp++=' ';out_len++;
          }
        }
        for(int i=0;i<len_s;i++){
          *outp++=*s++;out_len++;
        }
        while(len_s<width--){
          *outp++=' ';out_len++;
        }
        continue;
      }
    }

    int num=va_arg(ap,int);
    int count=0;
    if(num==0){
      *ss++='0';
      count+=1;
    }
    else{
      if(num<0){
        *outp++='-';out_len++;
        num=-num;
      }
      while(num){
        *ss++=num%10+'0';
        num=num/10;
        count+=1;
      }
    }
    if(count<width){
      num=width-count;
      if(flags&1){
        while(num--){
          *outp++='0';out_len++;
        }
      }
      else if(flags&8){
        while(num--){
          *outp++=' ';out_len++;
        }
      }
    }
    while(count--){
      *outp++=*--ss;out_len++;
    }
  } 
  *outp='\0';
  return out_len;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args,fmt);
  int out_len=vsprintf(out,fmt,args);
  va_end(args);
  return out_len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif