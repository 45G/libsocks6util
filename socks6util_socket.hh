#ifndef SOCKS6UTIL_SOCKET_HH
#define SOCKS6UTIL_SOCKET_HH

#include <sys/socket.h>
#include <socks6msg/socks6.h>

namespace S6U
{

namespace Socket
{

int saveSYN(int fd);
int tfoAttempted(int fd);
int hasMPTCP(int fd);
SOCKS6OperationReplyCode connectErrnoToReplyCode(int error);
int getOriginalDestination(int fd, sockaddr_storage *destination);

}

}

#endif // SOCKS6UTIL_SOCKET_HH
