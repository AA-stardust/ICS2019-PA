#include "klib.h"
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  int len=0;
  while(s[len]!='\0'){
    len++;
  }
  return len;
}

char *strcpy(char* dst,const char* src) {
  return NULL;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
  return NULL;
}

int strcmp(const char* s1, const char* s2) {
  for(int i=0;i<strlen(s1)&&i<strlen(s2);i++){
    if(s1[i]<s2[i])return -1;
    if(s1[i]>s2[i])return 1;
  }
  if(strlen(s1)<strlen(s2))return -1;
  if(strlen(s1)>strlen(s2))return 1;
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

void* memset(void* v,int c,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n){
  char *r1=(char *)s1;
  char *r2=(char *)s2;
  for(int i=0;i<n;i++){
    if(r1[i]>r2[i])return 1;
    if(r1[i]<r2[i])return -1;
  }
  return 0;
}

#endif
