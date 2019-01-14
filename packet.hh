#ifndef SOCKS6UTIL_PACKET_HH
#define SOCKS6UTIL_PACKET_HH

#include <stdint.h>

namespace S6U
{

namespace Packet
{

size_t tfoPayloadSize(const uint8_t *ipPacket);

}

}

#endif // SOCKS6UTIL_PACKET_HH
