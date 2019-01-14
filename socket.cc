#include <errno.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <linux/netfilter_ipv4.h>
#include <sys/ioctl.h>
#include "socket.hh"
#include "packet.hh"
#include "socketaddress.hh"

#ifndef TCP_SAVE_SYN
#define TCP_SAVE_SYN 27
#endif

#ifndef TCP_SAVED_SYN
#define TCP_SAVED_SYN 28
#endif

#ifndef MPTCP_SCHEDULER
#define MPTCP_SCHEDULER 43
#endif

#ifndef MPTCP_PATH_MANAGER
#define MPTCP_PATH_MANAGER 44
#endif

#define BUF_SIZE 4096

namespace S6U
{

namespace Socket
{

int saveSYN(int fd)
{
	static const uint32_t one = 1;
	
	return setsockopt(fd, SOL_TCP, TCP_SAVE_SYN, &one, sizeof(one));
}

ssize_t tfoPayloadSize(int fd)
{
	uint8_t buf[BUF_SIZE];
	socklen_t bufSize = BUF_SIZE;
	
	int err = getsockopt(fd, SOL_TCP, TCP_SAVED_SYN, buf, &bufSize);
	if (err < 0)
		return err;
	
	return Packet::tfoPayloadSize(buf);
}

int hasMPTCP(int fd)
{
	//TODO: find more elegant way to do this
	char opt[20];
	socklen_t opt_size = sizeof(opt);
	
	int err = getsockopt(fd, SOL_TCP, MPTCP_PATH_MANAGER, opt, &opt_size);
	if (err < 0)
		return err;
	
	return opt[0] != '\0';
}

SOCKS6OperationReplyCode connectErrnoToReplyCode(int error)
{
	switch (error)
	{
	case 0:
		return SOCKS6_OPERATION_REPLY_SUCCESS;
		
	/* assuming the address was not a broadcast address*/
	case EACCES:
	case EPERM:
		return SOCKS6_OPERATION_REPLY_NOT_ALLOWED;
		
	/* better write code to check this case beforehand; also check address type returned by getaddrinfo() */
	case EAFNOSUPPORT:
		return SOCKS6_OPERATION_REPLY_ADDR_NOT_SUPPORTED;
		
	case ECONNREFUSED:
		return SOCKS6_OPERATION_REPLY_REFUSED;
		
	case ENETUNREACH:
		return SOCKS6_OPERATION_REPLY_NET_UNREACH;
		
	case EHOSTUNREACH:
		return SOCKS6_OPERATION_REPLY_HOST_UNREACH;
		
	case ETIMEDOUT:
		return SOCKS6_OPERATION_REPLY_TIMEOUT;
	}
	
	/* assuming no benign errnos were passed */
	return SOCKS6_OPERATION_REPLY_FAILURE;
}

int getOriginalDestination(int fd, sockaddr_storage *destination)
{
	socklen_t destLen = sizeof(sockaddr_storage);
	
	return getsockopt(fd, SOL_IP, SO_ORIGINAL_DST, destination, &destLen);
}

int setMPTCPSched(int fd, SOCKS6MPTCPScheduler sched)
{
	const char *schedStr;
	
	switch (sched)
	{
	case SOCKS6_MPTCP_SCHEDULER_DEFAULT:
		schedStr = "default";
		break;
	case SOCKS6_MPTCP_SCHEDULER_RR:
		schedStr = "roundrobin";
		break;
	case SOCKS6_MPTCP_SCHEDULER_REDUNDANT:
		schedStr = "redundant";
		break;
	default:
		errno = EINVAL;
		return -1;
	}
	
	return setsockopt(fd, SOL_TCP, MPTCP_SCHEDULER, schedStr, strlen(schedStr));
}

int pendingRecv(int fd)
{
	int size;
	int rc = ioctl(fd, FIONREAD, &size);
	if (rc < 0)
		return rc;
	
	return size;
}

}

}
