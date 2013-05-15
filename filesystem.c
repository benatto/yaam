#include <sys/stat.h>
#include <sys/mount.h>

#include "filesystem.h"

int mountdevice(const char *devnode, const char *mountpoint){
	int mstatus;
	struct stat st = {0};

	fprintf(stdout, "Checking for directory existence: %s\n", mountpoint);
	/*checking for directory existence*/
	if (stat(mountpoint, &st) == -1){
		fprintf(stdout, "Creating dir: %s\n", mountpoint);
		/*create a new dir*/
		if(mkdir(mountpoint, 0666) == -1){
			fprintf(stderr, "Unable to create: %s. Error: %s\n", mountpoint, strerror(errno));
			return -1;
		}
	}

	fprintf(stdout, "Mounting %s on %s\n", devnode, mountpoint);

	mstatus = mount(devnode, mountpoint, "ext4", MS_MGC_VAL, "");

	if (mstatus == -1){
		fprintf(stderr, "Error mounting [%s]: %s\n", devnode, strerror(errno));
		return -1;
	}else{
		fprintf(stdout, "[%s} mounted successfully on %s\n", devnode, mountpoint);
		return 0;
	}
}

int umountdevice(const char *path){
	if(umount(path) == 0){
		if(rmdir(path) == 0){
			fprintf(stdout, "%s removed successfully\n", path);
			return 0;
		}else{
			fprintf(stderr, "Could not remove %s: %s\n", path, strerror(errno));
		}
	}else{
		fprintf(stderr, "%s could not be unmounted: %s\n", strerror(errno));
	}
}
