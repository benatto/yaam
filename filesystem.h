#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

#define SLENGHT 128
#define MOUNTPREFIX "/media/USBSTICK"
#define SYSLENGHT 255

/*Mounts a device
 * Param1: device node
 */
int mountdevice(const char*);

/*Removes a not mounted more dir
 * Param1: directory
 */;
int umountdevice(const char*);

/*Returns the "must-be" filesystem type from a umounted device
 * Param1: type code 
 */
char *getpartitiontype(int);

/*Returns the mountpoint path to a given device name
 * Param1: device name
 * PS.: Note that this function doesn't free it's return
 * caller must handle this kind of thing*/
char *getmountpoint(const char*);

/*Returns 1 in case the device pointed by arg path
 * is a removable media or 0 case not
 * */
int isdevremovable(const char*);
