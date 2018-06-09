#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netdb.h>
//#include "common.h"
#include "if_info.h"
#include <errno.h>


bool get_active_interface(const char *interface, IF_INFO *pif)
{
    char ifrbuf[8192];
    struct ifconf ifc;
    struct ifreq ifreq, *ifrp;
    struct ifreq ifrm;
    int	my_socket;
    int	i;
    unsigned int size;
    unsigned int len = 8192;
    int	count = 0;
    bool ret = false;

    //printf("%s()\n", __FUNCTION__);
    memset(pif, 0, sizeof(IF_INFO));
    memset(&ifrm, 0, sizeof(struct ifreq));
    //strcpy(pif->dev, interface);
    strcpy(ifrm.ifr_name, interface);

    my_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (my_socket < 0)
	return ret;

    while (1)
    {
	ifc.ifc_len = len;
	ifc.ifc_buf = (char *)calloc(1, len);
	if (ifc.ifc_buf == NULL)
	    goto end_get_iff;

	if (ioctl(my_socket, SIOCGIFCONF, &ifc) < 0)
	    goto end_get_iff;

	if ((ifc.ifc_len + sizeof(ifreq)) < len)
	    break;
	free(ifc.ifc_buf);
	len *= 2;
    }

    ifrp = ifc.ifc_req;
    ifreq.ifr_name[0] = '\0';
    for (i = 0; i < ifc.ifc_len; )
    {
	ifrp = (struct ifreq *)((caddr_t)ifc.ifc_req + i);
	memcpy(ifrbuf, ifrp, sizeof(*ifrp));

	size = sizeof(ifrp->ifr_addr) + sizeof(ifrp->ifr_name);
	i += size;
	/* avoid alignment issue */
	if (sizeof(ifrbuf) < size)
	    goto end_get_iff;

	memcpy(ifrbuf, ifrp, size);
	ifrp = (struct ifreq *)ifrbuf;

	if (strncmp(ifrm.ifr_name, ifrp->ifr_name, sizeof(ifrp->ifr_name)))
	    continue;

	//sin = (struct sockaddr_in *)&ifrp->ifr_addr;
	// IP Address
	//sprintf(pif->ipaddr, "%s", inet_ntoa(sin->sin_addr));
	pif->ipaddr = ((struct sockaddr_in *)&(ifrp->ifr_addr))->sin_addr;

	// broadcast 
	if (ioctl(my_socket, SIOCGIFBRDADDR, (caddr_t)ifrp) < 0)
	    goto end_get_iff;

	pif->bcast = ((struct sockaddr_in *)&(ifrp->ifr_addr))->sin_addr;

	// netmask
	if (ioctl(my_socket, SIOCGIFNETMASK, (caddr_t)ifrp) < 0)
	    goto end_get_iff;

	pif->netmask = ((struct sockaddr_in *)&(ifrp->ifr_addr))->sin_addr;

//	// gateway
//	if (get_default_gateway(ifrp->ifr_name, &(pif->gateway)) == 0)
//	    pif->gateway.s_addr = 0;

	count++;
    }

    strcpy(ifrp->ifr_name, interface);
    if (ioctl(my_socket, SIOCGIFHWADDR, (caddr_t)ifrp) < 0)
	goto end_get_iff;
    //----------------------------------------------------
    // MAC Address
    //----------------------------------------------------
    memcpy(pif->hwaddr, ifrp->ifr_hwaddr.sa_data, MAC_LEN);
    if (ioctl(my_socket, SIOCGIFFLAGS, &ifrm) < 0)
	goto end_get_iff;

    if (!( ifrm.ifr_flags & 1))
	goto end_get_iff;

    ret = true;
end_get_iff:
    close(my_socket);
    if (ifc.ifc_buf != NULL)
	free(ifc.ifc_buf);

    return ret;
}

int main(void)
{
	IF_INFO if_info;
	if (get_active_interface("ens33", &if_info) &&	// check active interface
			(if_info.ipaddr.s_addr != 0)) {					// check whether IP exists or not on active interface
	}
		printf("if_info.ipaddr.s_addr is %s\n",if_info.ipaddr.s_addr);
	printf("Hello World!\n");
	return 0;
}
