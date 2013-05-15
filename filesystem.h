#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

/*Mounts a device
 * Param1: device node
 * Param2: mounting point*/
int mountdevice(const char*, const char*);
/*Removes a not mounted more dir
 * Param1: directory*/;
int umountdevice(const char*);
