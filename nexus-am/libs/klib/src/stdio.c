#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *outp;
  int width;
  int flags;
  //const char *s;
  //int len_s;
  char nums[66];
  char *ss=nums;
  //int num;

  for(outp=out;*fmt;fmt++){
    if(*fmt!='%'){
      *outp++=*fmt;
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
      //char nums[20];
      for(;*fmt>='0'&&*fmt<='9';fmt++){
        //nums[i]=*fmt;
        width=width*10+*fmt-'0';
      }
      //width=atoi(nums);
    }
    else if(*fmt=='*'){
      fmt++;
      width=va_arg(ap,int);
      if(width<0){
        width=-width;
        flags|=16;
      }
    }
    //int base=10;
    switch(*fmt){
      case 'd':break;
      case 's':{
        char *s=va_arg(ap,char *);
        int len_s=strlen(s);
        if(!(flags&16)){
          while(len_s<width--){
            *outp++=' ';
          }
        }
        for(int i=0;i<len_s;i++){
          *outp++=*s++;
        }
        while(len_s<width--){
          *outp++=' ';
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
        *outp++='-';
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
          *outp++='0';
        }
      }
      else if(flags&8){
        while(num--){
          *outp++=' ';
        }
      }
    }
    while(count--){
      *outp++=*--ss;
    }
  } 
  *outp='\0';
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  int v;
  va_start(args,fmt);
  v=vsprintf(out,fmt,args);
  va_end(args);
  return v;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
