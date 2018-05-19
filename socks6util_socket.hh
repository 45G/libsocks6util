#ifndef SOCKS6UTIL_SOCKET_HH
#define SOCKS6UTIL_SOCKET_HH

//TODO: find nice way to include this
#include "../libsocks6msg/socks6.h"

namespace S6U
{

namespace Socket
{

int saveSYN(int fd);
int tfoAttempted(int fd);
int hasMPTCP(int fd);
SOCKS6OperationReplyCode connectErrnoToReplyCode(int error);

}

}

#endif // SOCKS6UTIL_SOCKET_HH
