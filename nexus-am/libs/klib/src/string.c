#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len=0;
  while((*s++)!='\0'){
    len++;
  }
  return len;
}

char *strcpy(char* dst,const char* src) {
  return strncpy(dst,src,strlen(src));
}

char* strncpy(char* dst, const char* src, size_t n) {
  if(n>strlen(src)){
    n=strlen(src);
  }
  char *res=dst;
  while(n){
    if((*res=*src)!=0){
      src++;
    }
    res++;
    n--;
  }
  return res;
}

char* strcat(char* dst, const char* src) {
  char *temp=dst;
  while(*dst){
    dst++;
  }
  while((*dst++=*src++)!='\0');
  return temp; 
}

int strcmp(const char* s1, const char* s2) {
  int t=0;
  while((t=(*s1-*s2))==0 && *s1 && *s2){
    s1++;s2++;
  }
  if(t>0){
    return 1;
  }
  else if(t<0){
    return -1;
  }
  else{
    return 0;
  }
  /*while((t=(*s1-*s2))==0 && *s1 && *s2){
    s1++;s2++;
  }*/
  printf("t:%d\n",t);
  return t;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  assert(s1!=NULL && s2!=NULL);
  while(n--){
    if(*s1==0 ||*s1!=*s2){
      return *s1-*s2;
    }
    s1++;s2++;
  }
  return 0;
}

void* memset(void* v,int c,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n){
  return 0;
}

#endif
