#include <errno.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <linux/netfilter_ipv4.h>
#include "socks6util_socket.hh"
#include "socks6util_packet.hh"
#include "socks6util_socketaddress.hh"

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

namespace S6U
{

namespace Socket
{

int saveSYN(int fd)
{
	static const uint32_t one = 1;
	
	return setsockopt(fd, SOL_TCP, TCP_SAVE_SYN, &one, sizeof(one));
}

int tfoAttempted(int fd)
{
	uint8_t buf[BUF_SIZE];
	socklen_t bufSize = BUF_SIZE;
	
	int err = getsockopt(fd, SOL_TCP, TCP_SAVED_SYN, buf, &bufSize);
	if (err < 0)
		return err;
	
	return Packet::hasTFO(buf);
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
	//TODO: check if this works for IPv6
	socklen_t destLen = sizeof(sockaddr_storage);
	
	return getsockopt(fd, SOL_IP, SO_ORIGINAL_DST, destination, &destLen);
}

int fastOpenConnect(int fd, const sockaddr_storage *destination, const void *buffer, size_t length, int flags)
{
	socklen_t addrLen = reinterpret_cast<const SocketAddress *>(destination)->size();
	
#ifdef TCP_FASTOPEN_CONNECT
	int rc;
	static const int one = 1;
	// tolerable error;
	setsockopt(fd, SOL_TCP, TCP_FASTOPEN_CONNECT, &one, sizeof(one));
	
	rc = connect(fd, reinterpret_cast<const sockaddr *>(destination), addrLen);
	if (rc < 0)
		return rc;
	
	return send(fd, buffer, length, flags);
#endif

#ifdef MSG_FASTOPEN
	return sendto(fd, buffer, length, flags, reinterpret_cast<const sockaddr *>(destination), addrLen);
#endif
	
	(void)buffer; (void)length; (void)flags;
	
	return connect(fd, reinterpret_cast<const sockaddr *>(destination), addrLen);
}



}

}
