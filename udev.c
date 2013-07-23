#include "udev.h"

#define DELIM "/"


struct udev_info *create_udev_info(){
		struct udev_info *udev_i = (struct udev_info*)malloc(sizeof(struct udev_info));

		if (!udev_i){
				printf("Not able to allocate udev_info struct\n");
				return NULL;
		}

		udev_i->udev = udev_new(); /*Creating new udev struct*/

		if (!udev_i->udev){
				printf("Not able to create udev struct\n");
				return NULL;
		}
			
		return udev_i;
}

int udev_monitor_start(struct udev_info *u){
	u->mon = udev_monitor_new_from_netlink(u->udev, "udev");

	if (!u->mon)
			return 1;

	udev_monitor_filter_add_match_subsystem_devtype(u->mon, "block", NULL);

	udev_monitor_enable_receiving(u->mon);

	u->fd = udev_monitor_get_fd(u->mon);

	return 1;
}

char *udev_get_partition(const char *devname){
	const char delim[2] = "/";
	char *tmp = (char*)malloc((strlen(devname) + 1 ) * sizeof(char));

	/*Copying the original value to a temp var
	 * as strtok will destroy the value from string and
	 * devname value pointed here could be used outside this function
	 * for any other reason */
	if (!strcpy(tmp, devname))
		return NULL;

	char *token = strtok(tmp, delim);
	char *part;

	while (token != NULL){
		token = strtok(NULL, delim);

		if(token)
			part = token;
	}

	/*Freeing temporary variable*/
	free(tmp);

	return part;
}
