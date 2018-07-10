#ifndef SOCKS6UTIL_SOCKET_HH
#define SOCKS6UTIL_SOCKET_HH

#include <sys/socket.h>
#include <sys/types.h>
#include <socks6msg/socks6.h>
#include <socks6msg/socks6msg_address.hh>

namespace S6U
{

namespace Socket
{

static const in_addr QUAD_ZERO = { .s_addr = 0 };

int saveSYN(int fd);
int tfoAttempted(int fd);
int hasMPTCP(int fd);
SOCKS6OperationReplyCode connectErrnoToReplyCode(int error);
int getOriginalDestination(int fd, sockaddr_storage *destination);
int setMPTCPSched(int fd, SOCKS6MPTCPScheduler sched);

}

}

#endif // SOCKS6UTIL_SOCKET_HH
