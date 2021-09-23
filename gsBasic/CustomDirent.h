/* 
 * File:   CustomDirent.h
 * Author: Bob
 *
 * Created on November 10, 2015, 9:05 PM
 */

#ifndef CUSTOMDIRENT_H
#define	CUSTOMDIRENT_H

#ifdef	__cplusplus
extern "C" {
#endif

struct dirent
{
    char *d_name;
    unsigned char d_type;
};

/*
 * File types
 */
#define	DT_REG 0
#define	DT_DIR 1

struct Dir;
typedef struct Dir Dir_t;

int closedir(Dir_t *);
Dir_t *opendir(const char *);
struct dirent *readdir(Dir_t *);
int readdir_r(Dir_t *, struct dirent *, struct dirent **);
void rewinddir(Dir_t *);

const char *CurrentDirectoryName();

#ifdef	__cplusplus
}
#endif

#endif	/* CUSTOMDIRENT_H */

