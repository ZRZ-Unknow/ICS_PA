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
//this is for the diff-dug
/*#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

const char lower_dict[] = "0123456789abcdef";
const char upper_dict[] = "0123456789ABCDEF";

static inline int _printc(char **out, char c) {
  if (out) {
    **out = c;
    (*out)++;
  } else {
    _putc(c);
  }
  return 1;
}

static inline int _prints(char **out, const char *s) {
  int cnt = 0;
  while (*s != '\0') {
    cnt += _printc(out, *(s++));
  }
  return cnt;
}

static inline int _printi(char **out, int num, int base, int use_upper, int sign) {
  assert(base <= 16 && base >= 2);

  const int buf_sz = 51;
  char buf[buf_sz];
  buf[buf_sz - 1] = '\0';
  const char *dict = use_upper ? upper_dict : lower_dict;

  if (num == 0) {
    return _printc(out, dict[0]);
  }

  char *p = &buf[buf_sz - 1];
  if (!sign) {
    unsigned int n = *(unsigned int *)&num;
    while (n != 0) {
      p--;
      *p = dict[n % base];
      n /= base;
    }
  } else {
    int is_neg = 0;
    if (num < 0) {
      num = -num;
      is_neg = 1;
    }

    while (num != 0) {
      p--;
      *p = dict[num % base];
      num /= base;
    }

    if (is_neg) {
      p--;
      *p = '-';
    }
  }

  return _prints(out, p);
}

static inline int _print(char **out, const char *fmt, va_list ap) {
  const char *p = fmt;
  int cnt = 0;
  while (*p) {
    if (*p == '%') {
      p++;
      switch (*p) {
        case 'd':
          cnt += _printi(out, va_arg(ap, int), 10, 0, 1);
          break;
        case 'u':
          cnt += _printi(out, va_arg(ap, int), 10, 0, 0);
          break;
        case 'x':
          cnt += _printi(out, va_arg(ap, int), 16, 0, 0);
          break;
        case 'X':
          cnt += _printi(out, va_arg(ap, int), 16, 1, 0);
          break;
        case 's':
          cnt += _prints(out, va_arg(ap, char *));
          break;
        default:
          break;
      }
    } else {
      cnt += _printc(out, *p);
    }
    p++;
  }
  if (out)
    **out = '\0';
  return cnt;
}

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int cnt = _print(NULL, fmt, ap);
  va_end(ap);
  return cnt;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return _print(&out, fmt, ap);
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int cnt = _print(&out, fmt, ap);
  va_end(ap);
  return cnt;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
*/