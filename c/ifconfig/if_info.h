
#ifndef _IF_INFO_H
#define _IF_INFO_H

#include <netinet/in.h>

#define MAC_LEN 6

typedef struct 
{
    unsigned char hwaddr[MAC_LEN];
    struct in_addr ipaddr;
    struct in_addr netmask;
    struct in_addr bcast;
    struct in_addr gateway;
} IF_INFO;

#ifdef c_plus_plus
extern "C" {
#endif

	bool get_active_interface(const char *interface, IF_INFO *pif);
	bool GetActiveIFInfo(const char *interface, IF_INFO *pif);

#ifdef c_plus_plus
}
#endif

#endif // _IF_INFO_H
