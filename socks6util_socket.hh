#ifndef SOCKS6UTIL_SOCKET_HH
#define SOCKS6UTIL_SOCKET_HH

namespace S6U
{

namespace Socket
{

int saveSYN(int fd);
int tfoAttempted(int fd);
int hasMPTCP(int fd);

}

}

#endif // SOCKS6UTIL_SOCKET_HH
