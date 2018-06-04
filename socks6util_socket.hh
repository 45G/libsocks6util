#ifndef SOCKS6UTIL_SOCKET_HH
#define SOCKS6UTIL_SOCKET_HH

#include <sys/socket.h>
#include <socks6msg/socks6.h>
#include <socks6msg/socks6msg_address.hh>

namespace S6U
{

namespace Socket
{

int saveSYN(int fd);
int tfoAttempted(int fd);
int hasMPTCP(int fd);
SOCKS6OperationReplyCode connectErrnoToReplyCode(int error);
int getOriginalDestination(int fd, sockaddr_storage *destination);
S6M::Address getAddress(const sockaddr_storage *socketAddress);
uint16_t getPort(const sockaddr_storage *socketAddress);

}

}

#endif // SOCKS6UTIL_SOCKET_HH
