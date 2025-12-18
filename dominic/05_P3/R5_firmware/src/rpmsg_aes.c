/*
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* This is a sample demonstration application that showcases usage of remoteproc
and rpmsg APIs on the remote core. This application is meant to run on the remote CPU
running baremetal code. This application receives Plain/Cipher text from the master,
encryption/decryption them and returns the result to the master core. */

#include "rpmsg_aes.h"
#include "xil_printf.h"
#include "openamp/open_amp.h"
#include "platform_info.h"
#include "aes.h"

#include "FreeRTOS.h"
#include "task.h"

#define SHUTDOWN_MSG	0xEF56A55A

#define LPRINTF(fmt, ...) xil_printf("%s():%u " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define LPERROR(fmt, ...) LPRINTF("ERROR: " fmt, ##__VA_ARGS__)

#define KEY_SIZE		(32)
#define IV_SIZE			(16)

typedef struct _aes_datatype {
	unsigned char dec;
	unsigned char key[KEY_SIZE];
	unsigned char iv[IV_SIZE];
	unsigned char text_length;
	unsigned char text[];	//flexible array members
} aes_datatype;

/* Local variables */
static struct AES_ctx ctx;
static TaskHandle_t comm_task;

static struct rpmsg_endpoint lept;
static int shutdown_req = 0;


/*-----------------------------------------------------------------------------*
 *  RPMSG callbacks setup by remoteproc_resource_init()
 *-----------------------------------------------------------------------------*/
static int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
				 uint32_t src, void *priv)
{
	aes_datatype *aes_data;

	(void)priv;
	(void)src;

	if ((*(unsigned int *)data) == SHUTDOWN_MSG) {
		ML_INFO("shutdown message is received.\r\n");
		shutdown_req = 1;
		return RPMSG_SUCCESS;
	}

	aes_data = (aes_datatype *) malloc( len );
	memcpy(aes_data, data, len);

	AES_init_ctx_iv(&ctx, aes_data->key, aes_data->iv);
	if (aes_data->dec) {
		AES_CBC_decrypt_buffer(&ctx, aes_data->text, aes_data->text_length);
	} else {
		AES_CBC_encrypt_buffer(&ctx, aes_data->text, aes_data->text_length);
	}
	// Send the result back to master.
	if (rpmsg_send(ept, aes_data->text, aes_data->text_length) < 0) {
		ML_ERR("rpmsg_send failed\r\n");
	}
	return RPMSG_SUCCESS;
}

static void rpmsg_service_unbind(struct rpmsg_endpoint *ept)
{
	(void)ept;
	ML_ERR("Endpoint is destroyed\r\n");
	shutdown_req = 1;
}

/*-----------------------------------------------------------------------------*
 *  Application
 *-----------------------------------------------------------------------------*/
int app(struct rpmsg_device *rdev, void *priv)
{
	int ret;

	ret = rpmsg_create_ept(&lept, rdev, RPMSG_SERVICE_NAME,
				   RPMSG_ADDR_ANY, RPMSG_ADDR_ANY,
				   rpmsg_endpoint_cb,
				   rpmsg_service_unbind);
	if (ret) {
		ML_ERR("Failed to create endpoint.\r\n");
		return -1;
	}

	LPRINTF("Waiting for events...\r\n");
	while(1) {
		platform_poll(priv);
		/* we got a shutdown request, exit */
		if (shutdown_req) {
			break;
		}
	}
	/*
	 * Ensure that kernel does not destroy endpoint twice
	 * by disabling NS announcement. Kernel will handle it.
	 */
	(&lept)->rdev->support_ns = 0;
	rpmsg_destroy_ept(&lept);

	return 0;
}

/*-----------------------------------------------------------------------------*
 *  Processing Task
 *-----------------------------------------------------------------------------*/
static void processing(void *unused_arg)
{
	void *platform;
	struct rpmsg_device *rpdev;

	LPRINTF("Starting application...\r\n");
	/* Initialize platform */
	if (platform_init(NULL, NULL, &platform)) {
		LPERROR("Failed to initialize platform.\r\n");
	} else {
		rpdev = platform_create_rpmsg_vdev(platform, 0,
						   VIRTIO_DEV_SLAVE,
						   NULL, NULL);
		if (!rpdev){
			ML_ERR("Failed to create rpmsg virtio device.\r\n");
		} else {
			app(rpdev, platform);
			platform_release_rpmsg_vdev(rpdev, platform);
		}
	}

	ML_INFO("Stopping application...\r\n");
	platform_cleanup(platform);

	/* Terminate this task */
	vTaskDelete(NULL);
}

/*-----------------------------------------------------------------------------*
 *  Application entry point
 *-----------------------------------------------------------------------------*/
int main(void)
{
	BaseType_t stat;

	/* Create the tasks */
	stat = xTaskCreate(processing, ( const char * ) "HW2",
				1024, NULL, 2, &comm_task);
	if (stat != pdPASS) {
		LPERROR("cannot create task\r\n");
	} else {
		/* Start running FreeRTOS tasks */
		vTaskStartScheduler();
	}

	/* Will not get here, unless a call is made to vTaskEndScheduler() */
	while (1) ;

	/* suppress compilation warnings*/
	return 0;
}
