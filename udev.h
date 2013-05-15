#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <libudev.h>

struct udev_info{
		struct udev *udev; /*udev API struct*/
		struct udev_enumerate *enumerate; /*udev enum struct*/
		struct udev_list_entry *devices, *dev_list_entry; /*device list entry*/
		struct udev_device *dev; /*udev device*/
		struct udev_monitor *mon; /*udev monitor struct*/
		int fd; /*file descriptor*/
};

struct udev_info *create_udev_info();

int udev_monitor_start();

char *udev_get_partition(const char*);
