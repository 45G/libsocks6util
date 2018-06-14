#ifndef SOCKS6UTIL_TFOSAFETY_HH
#define SOCKS6UTIL_TFOSAFETY_HH

#include "stdint.h"

namespace S6U
{

namespace TFOSafety
{

enum
{
	TFOS_TFO_SYN     = 1 << 0,
	TFOS_SPEND_TOKEN = 1 << 1,
	TFOS_TLS         = 1 << 2,
	TFOS_TLS_NO_0RTT = 1 << 3,
	TFOS_NO_DATA     = 1 << 4,
};

bool tfoSafe(uint32_t flags);

}

}

#endif // SOCKS6UTIL_TFOSAFETY_HH
