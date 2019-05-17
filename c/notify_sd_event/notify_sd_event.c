#include <stdio.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

const char *sd_card_block_device_name_list[2] = {"mmcblk0p1", "sda1"};

bool is_any_sd_card_block_device(char *msg) 
{
    bool ret = false;

    int max_index = sizeof(sd_card_block_device_name_list) / sizeof(sd_card_block_device_name_list[0]);

    for (int i = 0; i < max_index; i++) {
        
        /* Find sd_card_block_device is existed or not */
        if (strstr(msg, sd_card_block_device_name_list[i]) != NULL)
            ret = true;
    }

    return ret;
}

int main(int /*argc*/ , char * /*args*/[])
{
	int ret = -1;
	char caKernelMsgBuff[1024];
	const unsigned int uiRecvBuffSize = sizeof(caKernelMsgBuff);
	struct sockaddr_nl snl;
	int sfd = -1;

	/* Setup Netlink for getting messages from kernel */
	do {

		snl.nl_family = AF_NETLINK;
		snl.nl_pad = 0;
		snl.nl_pid = getpid();
		snl.nl_groups = 1;
		sfd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);

		if (sfd < 0) {
			fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
			break;
		}

		ret = setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &uiRecvBuffSize, sizeof(uiRecvBuffSize));
		if (ret != 0){
			fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
			break;
		}

		ret = bind(sfd, (struct sockaddr*)&snl, sizeof(snl));
		if (ret != 0){
			fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
			break;
		}

		ret = 0;

	} while(false);


	//TODO change to for controller while
	//        while (!get_quit() && !get_reload()) {
	while(1) {

		/* STEP1. Waitng for event msg. from kernel */
		recv(sfd, &caKernelMsgBuff, sizeof(caKernelMsgBuff), 0);
		//printf("caKernelMsgBuff:%s\n", caKernelMsgBuff);

		/* STEP2. memcmp the msg. that we want */
		if (memcmp(caKernelMsgBuff, "add@", 4) == 0 && is_any_sd_card_block_device(caKernelMsgBuff)) {

			fprintf(stderr,"%s [%d] SD add\n", __FUNCTION__, __LINE__);
            
		} else if (memcmp(caKernelMsgBuff, "remove@", 7) == 0 && is_any_sd_card_block_device(caKernelMsgBuff)) {

			fprintf(stderr,"%s [%d] SD remove\n", __FUNCTION__, __LINE__);

		}
	}

	/* Release resource */
	if (sfd == 0) {
		
		if (close(sfd) != 0) {
			fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
		}

		sfd = -1;
	}

	return ret;
}

