#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

#include "6809dasm.h"

#ifndef FALSE
#define FALSE (0!=0)
#define TRUE (0==0)
#endif

int load_tags(char *binname) {
  // procedure will remove the .bin or whatever extension
  int addr, rc, flags;
  FILE *blob;
  char *fn = malloc(strlen(binname)+16);
  char *s = strrchr(binname, '/'), *ext;
  strcpy(fn, binname);
  if (s) {
    *s++ = '\0';
    ext = strrchr(s, '.');
    *--s = '/';
  } else {
    ext = strrchr(fn, '.');
  }
  if (ext) *ext = '\0';
  strcat(fn, ".tags");
  blob = fopen(fn, "r"); // might do a binary blob later but for now ... this is unix after all...
  if (blob == NULL) {
    fprintf(stderr, "load tags: %s\n", strerror(errno));
    return(FALSE);
  }
  for (addr = 0; addr < 0x10000; addr++) {
    rc = fscanf(blob, "%d\n", &flags);
    if (rc != 1) {
      fprintf(stderr, "Input tags file was corrupt - bad entry #%d\n", addr);
      return(FALSE);
    }
    F_[addr] = flags;
  }
  fclose(blob);
  return TRUE;
}

int save_tags(char *binname) {
  // procedure will remove the .bin or whatever extension
  int addr;
  FILE *blob;
  char *fn = malloc(strlen(binname)+16);
  char *s = strrchr(binname, '/'), *ext;
  strcpy(fn, binname);
  if (s) {
    *s++ = '\0';
    ext = strrchr(s, '.');
    *--s = '/';
  } else {
    ext = strrchr(fn, '.');
  }
  if (ext) *ext = '\0';
  strcat(fn, ".tags");
  blob = fopen(fn, "w"); // might do a binary blob later but for now ... this is unix after all...
  if (blob == NULL) {
    fprintf(stderr, "load tags: %s\n", strerror(errno));
    return(FALSE);
  }
  for (addr = 0; addr < 0x10000; addr++) {
    fprintf(blob, "%d\n", F_[addr]);
  }
  fclose(blob);
  return TRUE;
}

