#include <libudev.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <errno.h>

#include "udev.h"
#include "filesystem.h"
#include "signalhandler.h"


int main(int argc, char **argv){
		struct udev_info *u = create_udev_info();
		char *mountpath, *devname;
	
		if(!u)
			exit(1);

		if (!udev_monitor_start(u))
			exit(1);

		signal(SIGINT, signal_handler_callback);

		mountpath = (char*)malloc(SLENGHT * sizeof(char));
		devname = (char*)malloc(SLENGHT * sizeof(char));


		/*main loop*/
		while(1){
			fd_set fds;
			struct timeval tv;
			int ret;

			FD_ZERO(&fds);
			FD_SET(u->fd, &fds);

			tv.tv_sec = 0;
			tv.tv_usec = 0;

			ret = select(u->fd + 1, &fds, NULL, NULL, &tv);

			if (ret > 0 && FD_ISSET(u->fd, &fds)){
								u->dev = udev_monitor_receive_device(u->mon);

				udev_device_get_devnode(u->dev);

				/*For mount devices, yaamd should expect for the whole disk
				 * and send this information for libblkid, otherwise there's
				 * no way to find out the partitions type of an umounted device.
				 *For umount devices, yammd should expect by each partition,
				 * or it will try to mount root device returning error on umounting*/

				if(strcmp(udev_device_get_devtype(u->dev), "disk") == 0
						  && strcmp(udev_device_get_action(u->dev), "add") == 0){
						int mret;
						char *cmd;
						char *devtmp; /*needed to be tokenized, otherwise original value gets lost*/
						
						fprintf(stdout, "\n Select():\n");
						fprintf(stdout, "Action: %s, Disk: %s\n", udev_device_get_action(u->dev),
																	   udev_device_get_devnode(u->dev));
						

						devname = udev_device_get_devnode(u->dev);

						devtmp = (char*)malloc((strlen(devname) + 1) * sizeof(char));

						strcpy(devtmp, devname);

						sprintf(mountpath, "/media/USBSTICK%s", udev_get_partition(devtmp));

						free(devtmp);
					
						mountdevice(devname) == 0 ? fprintf(stdout, "%s mounted!!!\n", devname) :
							   						fprintf(stderr, "Could not mount %s\n", devname);
				}else if(strcmp(udev_device_get_devtype(u->dev), "partition") == 0
						 && strcmp(udev_device_get_action(u->dev), "remove") == 0){
						system("DISPLAY=:0 zenity --notification --text=\"Dispositivo USB removido\" --timeout=2");
						umountdevice(udev_device_get_devnode(u->dev));
				}
			}

			usleep(250*1000);
			fflush(stdout);
		}

		return 0;

}
