#ifndef FUSE_H
#define FUSE_H

#include <stddef.h> /* size_t definition */

enum
{
    E_BAD_SOURCE = 1,
    E_BAD_DESTINATION,
    E_NO_MEMORY
};
#ifdef __cplusplus
extern "C"
{
#endif
    int fuse(char const **filenames, int num_files, char const *output);
    int unfuse(char const *filename);
#ifdef __cplusplus
}
#endif

#endif

/*
+-------------+--------+-----------+-------------+--------+-----------+....
|filename1    |filesize|data ......|filename1    |filesize|data ......|....
|\0 terminated| int    |           |\0 terminated| int    |           |....
+-------------+--------+-----------+-------------+--------+-----------+....
*/
