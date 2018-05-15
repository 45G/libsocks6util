#ifndef SOCKS6UTIL_H
#define SOCKS6UTIL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int S6U_Socket_SaveSYN(int fd);
int S6U_Socket_TFOAttempted(int fd);
int S6U_Socket_HasMPTCP(int fd);

int S6U_Packet_HasTFO(const uint8_t *ipPacket);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // SOCKS6UTIL_H
