#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include "socks6util_socket.h"
#include "socks6util_packet.h"

#ifndef TCP_SAVE_SYN
#define TCP_SAVE_SYN 27
#endif

#ifndef TCP_SAVED_SYN
#define TCP_SAVED_SYN 28
#endif

#ifndef MPTCP_PATH_MANAGER
#define MPTCP_PATH_MANAGER 43
#endif

#define BUF_SIZE 1500

int S6U_Socket_SaveSYN(int fd)
{
	static const uint32_t one = 1;
	
	return setsockopt(fd, SOL_TCP, TCP_SAVE_SYN, &one, sizeof(one));
}

int S6U_Socket_TFOAttempted(int fd)
{
	uint8_t buf[BUF_SIZE];
	socklen_t bufSize = BUF_SIZE;
	
	int err = getsockopt(fd, SOL_TCP, TCP_SAVED_SYN, buf, &bufSize);
	if (err < 0)
		return err;
	
	return S6U_Packet_HasTFO(buf);
}

int S6U_Socket_HasMPTCP(int fd)
{
	char opt[20];
	socklen_t opt_size = sizeof(opt);
	
	int err = getsockopt(fd, SOL_TCP, MPTCP_PATH_MANAGER, opt, &opt_size);
	if (err < 0)
		return err;
	
	return opt[0] != '\0';
}
