#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *outp;
  int width=0;
  int flags=0;
  for(outp=out;*fmt;fmt++){
    if(*fmt!='%'){
      *outp++=*fmt;
      continue;
    }
    int temp=1;
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
        //int len;
        char *s=va_arg(ap,char *);
        int len=strlen(s);
        if(!(flags&16)){
          while(len<width--){
            *outp++=' ';
          }
        }
        for(int i=0;i<len;i++){
          *outp++=*s++;
        }
        while(len<width--){
          *outp++=' ';
        }
        continue;
      }
    }
    int num=va_arg(ap,int);
    char nums[100];
    int count=0;
    char *ss=nums;
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
      //char point;
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
    while(ss--){
      *outp++=*ss;
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
