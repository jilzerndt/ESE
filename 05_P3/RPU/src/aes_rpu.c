/****************************************************************************************
 * @file
 * @brief RPU-based AES encryption
 *
 * @note
 *
 * --------------------------------------------------------------------------------------
 * @author  Flavio Felder, felf@zhaw.ch
 * @date	04.09.2023
 ***************************************************************************************/

/****************************************************************************************
 * Includes
 ***************************************************************************************/
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <linux/rpmsg.h>
#include "aes_rpu.h"

/****************************************************************************************
 * Defines
 ***************************************************************************************/
#define RPMSG_BUS_SYS "/sys/bus/rpmsg"

#define PR_DBG(fmt, args ...) printf("%s():%u "fmt, __func__, __LINE__, ##args)
#define SHUTDOWN_MSG    0xEF56A55A

#define KEY_SIZE		(32)
#define IV_SIZE			(16)

#define SLEEP_INTERVAL_MS 30
#define LOOKUP_CHANNEL_ATTEMPTS 10

/****************************************************************************************
 * Typedefs
 ***************************************************************************************/
typedef struct _aes_datatype {
	unsigned char dec;
	unsigned char key[KEY_SIZE];
	unsigned char iv[IV_SIZE];
	unsigned char text_length;
	unsigned char text[];	//flexible array members
} aes_datatype;


static int charfd_glob, fd_glob;

/****************************************************************************************
 * Local Functions
 ***************************************************************************************/

/*
 * Functions for rpmsg inter processor communication from linux userspace to
 * a remote software context.
 */
void send_shutdown(int fd)
{
	int sdm = SHUTDOWN_MSG;

	if (write(fd, &sdm, sizeof(int)) < 0) perror("write SHUTDOWN_MSG\n");
}

int rpmsg_create_ept(int rpfd, struct rpmsg_endpoint_info *eptinfo)
{
	int ret;

	ret = ioctl(rpfd, RPMSG_CREATE_EPT_IOCTL, eptinfo);
	if (ret) perror("Failed to create endpoint.\n");
	return ret;
}

static char *get_rpmsg_ept_dev_name(const char *rpmsg_char_name,
				    const char *ept_name,
				    char *ept_dev_name)
{
	char sys_rpmsg_ept_name_path[64];
	char svc_name[64];
	char *sys_rpmsg_path = "/sys/class/rpmsg";
	FILE *fp;
	int i;
	int ept_name_len;

	for (i = 0; i < 128; i++) {
		sprintf(sys_rpmsg_ept_name_path, "%s/%s/rpmsg%d/name",
			sys_rpmsg_path, rpmsg_char_name, i);
		printf("checking %s\n", sys_rpmsg_ept_name_path);
		if (access(sys_rpmsg_ept_name_path, F_OK) < 0) continue;
		fp = fopen(sys_rpmsg_ept_name_path, "r");
		if (!fp) {
			printf("failed to open %s\n", sys_rpmsg_ept_name_path);
			break;
		}
		fgets(svc_name, sizeof(svc_name), fp);
		fclose(fp);
		printf("svc_name: %s.\n",svc_name);
		ept_name_len = strlen(ept_name);
		if (ept_name_len > sizeof(svc_name)) {
			ept_name_len = sizeof(svc_name);
		}
		if (!strncmp(svc_name, ept_name, ept_name_len)) {
			sprintf(ept_dev_name, "rpmsg%d", i);
			return ept_dev_name;
		}
	}

	printf("Not able to RPMsg endpoint file for %s:%s.\n",
	       rpmsg_char_name, ept_name);
	return NULL;
}

static int bind_rpmsg_chrdev(const char *rpmsg_dev_name)
{
	char fpath[256];
	char *rpmsg_chdrv = "rpmsg_chrdev";
	int fd;
	int ret;

	/* rpmsg dev overrides path */
	sprintf(fpath, "%s/devices/%s/driver_override",
		RPMSG_BUS_SYS, rpmsg_dev_name);
	PR_DBG("open %s\n", fpath);
	fd = open(fpath, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open %s, %s\n",
			fpath, strerror(errno));
		return -EINVAL;
	}
	ret = write(fd, rpmsg_chdrv, strlen(rpmsg_chdrv) + 1);
	if (ret < 0) {
		fprintf(stderr, "Failed to write %s to %s, %s\n",
			rpmsg_chdrv, fpath, strerror(errno));
		return -EINVAL;
	}
	close(fd);

	/* bind the rpmsg device to rpmsg char driver */
	sprintf(fpath, "%s/drivers/%s/bind", RPMSG_BUS_SYS, rpmsg_chdrv);
	fd = open(fpath, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open %s, %s\n",
			fpath, strerror(errno));
		return -EINVAL;
	}
	PR_DBG("write %s to %s\n", rpmsg_dev_name, fpath);
	ret = write(fd, rpmsg_dev_name, strlen(rpmsg_dev_name) + 1);
	if (ret < 0) {
		fprintf(stderr, "Failed to write %s to %s, %s\n",
			rpmsg_dev_name, fpath, strerror(errno));
		return -EINVAL;
	}
	close(fd);
	return 0;
}

static int get_rpmsg_chrdev_fd(const char *rpmsg_dev_name,
			       char *rpmsg_ctrl_name)
{
	char dpath[2*NAME_MAX];
	DIR *dir;
	struct dirent *ent;
	int fd;

	sprintf(dpath, "%s/devices/%s/rpmsg", RPMSG_BUS_SYS, rpmsg_dev_name);
	PR_DBG("opendir %s\n", dpath);
	dir = opendir(dpath);
	if (dir == NULL) {
		fprintf(stderr, "opendir %s, %s\n", dpath, strerror(errno));
		return -EINVAL;
	}
	while ((ent = readdir(dir)) != NULL) {
		if (!strncmp(ent->d_name, "rpmsg_ctrl", 10)) {
			sprintf(dpath, "/dev/%s", ent->d_name);
			closedir(dir);
			PR_DBG("open %s\n", dpath);
			fd = open(dpath, O_RDWR | O_NONBLOCK);
			if (fd < 0) {
				fprintf(stderr, "open %s, %s\n",
					dpath, strerror(errno));
				return fd;
			}
			sprintf(rpmsg_ctrl_name, "%s", ent->d_name);
			return fd;
		}
	}

	fprintf(stderr, "No rpmsg_ctrl file found in %s\n", dpath);
	closedir(dir);
	return -EINVAL;
}

static void set_src_dst(char *out, struct rpmsg_endpoint_info *pep)
{
	long dst = 0;
	char *lastdot = strrchr(out, '.');

	if (lastdot == NULL) return;
	dst = strtol(lastdot + 1, NULL, 10);
	if ((errno == ERANGE && (dst == LONG_MAX || dst == LONG_MIN))
	    || (errno != 0 && dst == 0)) {
		return;
	}
	pep->dst = (unsigned int)dst;
}

/*
 * return the first dirent matching rpmsg-openamp-demo-channel
 * in /sys/bus/rpmsg/devices/ E.g.:
 *	virtio0.rpmsg-openamp-demo-channel.-1.1024
 */
static int lookup_channel(char *out, struct rpmsg_endpoint_info *pep) {
	char dpath[] = RPMSG_BUS_SYS "/devices";
	struct dirent *ent;
	DIR *dir;

	// the virtio device needs a few ms for startup.
	// this loop polls the device and continues as soon as is is available
	for (int i = 0; i < LOOKUP_CHANNEL_ATTEMPTS; i++) {
		dir = opendir(dpath);
	if (dir == NULL) {
		fprintf(stderr, "opendir %s, %s\n", dpath, strerror(errno));
		return -1;
	}
	while ((ent = readdir(dir)) != NULL) {
		if (strstr(ent->d_name, pep->name)) {
			strncpy(out, ent->d_name, NAME_MAX);
			set_src_dst(out, pep);
			PR_DBG("using dev file: %s\n", out);
			closedir(dir);
			return 0;
		}
	}
	closedir(dir);
		printf("lookup channel attempt %d.\n", i);
		usleep(SLEEP_INTERVAL_MS * 1000);
	}
	fprintf(stderr, "No dev file for %s in %s\n", pep->name, dpath);
	return -1;
}


/****************************************************************************************
 * @brief engine-based AES encryption
 * @param dec[in]		Set 1 for decryption
 * @param key[in]		Key
 * @param iv[in]		Initialization vector
 * @param buf[in/out]	Plain/Cipher text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
static void AES_RPU_xcrypt_buffer(int dec, const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length) {
	/* AES encryption/decryption */
	aes_datatype *aes_data;
	size_t data_size = sizeof(*aes_data)+length;
	aes_data = (aes_datatype *) malloc(data_size);

	aes_data->dec = dec;
	aes_data->text_length = length;
	memcpy(aes_data->key, key, KEY_SIZE);
	memcpy(aes_data->iv, iv, IV_SIZE);
	memcpy(aes_data->text, buf, length);

	printf("write rpmsg: %lu bytes\n", data_size);
	ssize_t rc = write(fd_glob, aes_data, data_size);
	if (rc < 0)	fprintf(stderr, "write,errno = %ld, %d\n", rc, errno);

	printf("read results: %lu bytes\n", length);
	do {
		rc = read(fd_glob, buf, length);
	} while (rc < (int)length);
}


/****************************************************************************************
 * Global Functions
 ***************************************************************************************/

/****************************************************************************************
 * @brief RPU-based AES encryption
 * @param key[in]		Key
 * @param iv[in]		Initialization vector
 * @param buf[in/out]	Plain text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
void AES_RPU_encrypt_buffer(const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length) {
	AES_RPU_xcrypt_buffer(0, key, iv, buf, length);
}

/****************************************************************************************
 * @brief RPU-based AES decryption
 * @param key[in]		Key
 * @param iv[in]		Initialization vector
 * @param buf[in/out]	Cipher text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
void AES_RPU_decrypt_buffer(const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length) {
	AES_RPU_xcrypt_buffer(1, key, iv, buf, length);
}

/****************************************************************************************
 * @brief Start RPU-firmware for AES encryption
 * @param firmware[in]	Firmware name as string
 ***************************************************************************************/
int AES_RPU_start(char *firmware) {
    FILE *fp;

	fp = fopen("/sys/class/remoteproc/remoteproc0/firmware", "w");
	if (fp == NULL) {
		printf("Error selecting firmware %s\n", firmware);
		return -1;
	}
    fprintf(fp, "%s", firmware);
    fclose(fp);

	// TODO BEGIN STUDENTS
    // start the firmware...

	fp = fopen("/sys/class/remoteproc/remoteproc0/state", "w");
	if (fp == NULL) {
		printf("Error starting firmware %s\n", firmware);
		return -1;
	}
	fprintf(fp, "start");
	fclose(fp);

	// TODO END STUDENTS

	charfd_glob = -1;
	int ret;
	char rpmsg_dev[NAME_MAX] = "virtio0.rpmsg-openamp-demo-channel.-1.0";
	char rpmsg_char_name[16];
	char fpath[2*NAME_MAX];
	struct rpmsg_endpoint_info eptinfo = {
		.name = "rpmsg-openamp-demo-channel", .src = 0, .dst = 0
	};
	char ept_dev_name[16];
	char ept_dev_path[32];

	printf("\r\n Establish rpmsg channel \r\n");

	/* Load rpmsg_char driver */
	printf("\r\nMaster>probe rpmsg_char\r\n");
	ret = system("set -x; lsmod; modprobe rpmsg_char");
	if (ret < 0) {
		perror("Failed to load rpmsg_char driver.\n");
		return -1;
	}

	printf("looking for channel...\n");
	int chan_state = lookup_channel(rpmsg_dev, &eptinfo);

	if (0 > chan_state) {
		perror("No rpmsg device found.\n");
		return -1;
	} 
	printf("channel found: %s\n", rpmsg_dev);

	sprintf(fpath, RPMSG_BUS_SYS "/devices/%s", rpmsg_dev);
	if (access(fpath, F_OK)) {
		fprintf(stderr, "access(%s): %s\n", fpath, strerror(errno));
		return -1;
	}
	ret = bind_rpmsg_chrdev(rpmsg_dev);
	if (ret < 0) return -1;
	charfd_glob = get_rpmsg_chrdev_fd(rpmsg_dev, rpmsg_char_name);
	if (charfd_glob < 0) return -1;

	/* Create endpoint from rpmsg char driver */
	PR_DBG("rpmsg_create_ept: %s[src=%#x,dst=%#x]\n",
		eptinfo.name, eptinfo.src, eptinfo.dst);
	ret = rpmsg_create_ept(charfd_glob, &eptinfo);
	if (ret) {
		fprintf(stderr, "rpmsg_create_ept %s\n", strerror(errno));
		return -1;
	}
	if (!get_rpmsg_ept_dev_name(rpmsg_char_name, eptinfo.name, ept_dev_name)) {
		return -1;
	}
	sprintf(ept_dev_path, "/dev/%s", ept_dev_name);

	printf("open %s\n", ept_dev_path);
	fd_glob = open(ept_dev_path, O_RDWR | O_NONBLOCK);
	if (fd_glob < 0) {
		perror(ept_dev_path);
		close(charfd_glob);
		return -1;
	}

	return 0;
}

/****************************************************************************************
 * @brief Stop RPU-firmware for AES encryption
 * @param firmware[in]	Firmware name as string
 ***************************************************************************************/
int AES_RPU_stop(char *firmware) {
	printf("\r\n Quitting application .. \r\n");
	send_shutdown(fd_glob);
	close(fd_glob);
	if (charfd_glob >= 0) {
		close(charfd_glob);
	}

	sleep(2); // wait for shutdown completion
	printf(" AES RPU application end \r\n");

	// TODO BEGIN STUDENTS
	// stop the firmware

	FILE *fp = fopen("/sys/class/remoteproc/remoteproc0/state", "w");
	if (fp == NULL) {
		printf("Error stopping firmware %s\n", firmware);
		return -1;
	}
	fprintf(fp, "stop");
	fclose(fp);

	// TODO END STUDENTS

	return 0;
}
