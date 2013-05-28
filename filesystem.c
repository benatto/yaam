#include <sys/stat.h>
#include <sys/mount.h>
#include <blkid/blkid.h>
#include <string.h>

#include "filesystem.h"

int mountdevice(const char *devnode){
	/*Using device's MBR information to retrieve partitions and its types
	 * to mount it correctly*/
	blkid_probe pr; /*BLKID probe struct*/
	blkid_partlist pl; /*BLKID partitions list*/
	int nparts, i;
	char *pname = (char*)malloc(32 * sizeof(char));
	char *mountpoint = (char*)malloc(128 * sizeof(char));

	pr = blkid_new_probe_from_filename(devnode); /*Starts blkid probe*/

	if (!pr){
		fprintf(stderr, "Error on opening device: %s - %s\n", devnode, strerror(errno));
		return -1;
	}

	if (!pname || !mountpoint){
		fprintf(stderr, "Not able to allocate memory, will not mount anything\n");
		return -1;
	}

	pl = blkid_probe_get_partitions(pr); /*Getting partition list*/
	nparts = blkid_partlist_numof_partitions(pl); /*Getting the number of partitions*/

	for (i = 0; i < nparts; i++){
		char *pnametmp; /*temp variable to be tokenized by udev_get_partition*/
		char *ptype, *cmd;
		int t_part, partno, mstatus;
		struct stat st = {0};
		blkid_partition part = blkid_partlist_get_partition(pl, i);

		t_part = blkid_partition_get_type(part);
		partno = blkid_partition_get_partno(part);

		fprintf(stdout, "Partition: %d, type: 0x%x\n", partno, t_part);
		
		sprintf(pname, "%s%d", devnode, partno);

		pnametmp = (char*)malloc(strlen(pname) * sizeof(char));
		
		if (!pnametmp){
			fprintf(stderr, "Could not allocate memory, will not mount anything\n");
			return -1;
		}

		strcpy(pnametmp, pname);
		
		sprintf(mountpoint, "/media/USBSTICK%s", udev_get_partition(pnametmp));

		/*Freeing temporary variable*/
		free(pnametmp);

		fprintf(stdout, "Preparing to mount: %s\n", pname);

		if (stat(mountpoint, &st) == -1){
			fprintf(stdout, "Creating dir: %s\n", mountpoint);
			/*create a new dir*/
			if(mkdir(mountpoint, 0777) == -1){
				fprintf(stderr, "Unable to create: %s. Error: %s\n", mountpoint, strerror(errno));
				return -1;
			}else{
				fprintf(stdout, "%s create successfully.\n", mountpoint);
			}
		}

		fprintf(stdout, "Mounting %s on %s\n", pname, mountpoint);

		ptype = getpartitiontype(t_part);

		mstatus = mount(pname, mountpoint, ptype, MS_SYNCHRONOUS, "");

		if (mstatus == -1){
			cmd = (char*)malloc(SLENGHT * sizeof(char));
			fprintf(stderr, "Error mounting [%s]: %s\n", pname, strerror(errno));
			sprintf(cmd, "DISPLAY=:0 zenity --notification --text\"Não foi possivel montar\
						  %s\" --timeout=2", pname);
			
			if(fork() == 0)
				system(cmd);
			else
				wait(NULL);

			free(cmd);
			
			return -1;
		}else{
			int ret;
			char *cmd = (char*)malloc(SLENGHT * sizeof(char));
			sprintf(cmd, "DISPLAY=:0 zenity --notification --text=\"Dispositivo USB(%s)\
						 encontrado e montado(%s)\" --timeout=2", pname, mountpoint);

			system(cmd);
			
			sprintf(cmd, "DISPLAY=:0 pcmanfm %s", mountpoint);

			if(fork() == 0){
				ret = system(cmd);
				exit(EXIT_SUCCESS);
			}else{
				fprintf(stdout, "[%s] mounted successfully on %s\n", pname, mountpoint);
			}

			free(cmd);
		}
	}

	free(pname);
	free(mountpoint);
	blkid_free_probe(pr);

	return 0;
}

int umountdevice(const char *path){
	char *mountpoint;

	mountpoint = getmountpoint(path);

	if(umount2(mountpoint, MNT_FORCE) == 0){
		if(rmdir(mountpoint) == 0){
			fprintf(stdout, "%s removed successfully\n", mountpoint);
			return 0;
		}else{
			fprintf(stderr, "Could not remove %s: %s\n", mountpoint, strerror(errno));
		}
	}else{
		fprintf(stderr, "%s could not be unmounted: %s\n", path, strerror(errno));
	}
}

char *getpartitiontype(int tc){
	switch(tc){
		case 0x83:
			printf("ext4\n");
			return "ext4";
			break;
		case 0xb:
			printf("vfat\n");
			return "vfat";
			break;
		case 0xc:
			printf("vfat\n");
			return "vfat";
			break;
		case 0x7:
			printf("ntfs\n");
			return "ntfs";
			break;
		default:
			printf("0x%x invalid\n", tc);
			return NULL;
			break;
	}
}

char *getmountpoint(const char *devnode){
	char *pnametmp, *mountpoint;
	pnametmp = (char*)malloc(strlen(devnode) * sizeof(char));
	mountpoint = (char*)malloc(SLENGHT * sizeof(char));
		
	if (!pnametmp || !mountpoint){
		fprintf(stderr, "Could not allocate memory, will not mount anything\n");
		return NULL;
	}

	strcpy(pnametmp, devnode);
		
	sprintf(mountpoint, "/media/USBSTICK%s", udev_get_partition(pnametmp));

	/*Freeing temporary variable*/
	free(pnametmp);

	return mountpoint;

}
