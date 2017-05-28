/*
 * The storage manager is the lowest layer for dealing with data. The storage manager implements the file system/OS api's to bring data from disk and to write data back to disk. The storage manager only understand the physical view of the data. The storage manager should only be invoked by the buffer manager.
 */

/*
 * Fill a buffer with the corresponding page of a relation's file
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define PAGE_SIZE 8192

bool get_page_from_disk(char *buffer, char *rel_name,int page) {
    //TODO:This should be saved in some global state instead of looking it up for every call
    char path[100];
    path[0] = '/';
    char *env = getenv("KDB_HOME");
    strcat(path, env);
    strcat(path, "/");
    strcat(path, rel_name);

    FILE *fd = fopen(path, "r");
    if (!fd)
        return false;
    if (page > 0) {
        //seek in the file first
        fseek(fd, 8192 * page, SEEK_SET);
    }
    int read_bytes = fread(buffer, sizeof(char), 8192, fd);
    if (read_bytes <= 0)
        return false;
    return true;
}

