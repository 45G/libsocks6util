#include "socks6util_tfosafety.hh"

namespace S6U
{

namespace TFOSafety
{

bool tfoSafe(uint32_t flags)
{
	/* client tolerates TFO */
	if (flags & TFOS_TFO_SYN)
		return true;

	/* got idempotence */
	if (flags & TFOS_SPEND_TOKEN)
		return true;

	/* no application data */
	if (flags & TFOS_NO_DATA)
		return true;

	/* TLS w/o 0-RTT data */
	if ((flags & TFOS_TLS) && (flags & TFOS_TLS_NO_0RTT))
		return true;

	return false;
}

}

}
