#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>

#ifndef TCPOPT_EOL
#define TCPOPT_EOL 0
#endif

#ifndef TCPOPT_NOP
#define TCPOPT_NOP 1
#endif

#ifndef TCPOPT_TFO
#define TCPOPT_TFO 34
#endif

#include "socks6util_packet.hh"

namespace S6U
{

namespace Packet
{

size_t tfoPayloadSize(const uint8_t *ipPacket)
{
	const tcphdr *tcpHeader;
	const ip *ipHeader = (const ip *)ipPacket;
	
	if (ipHeader->ip_v == 4)
		tcpHeader = (const tcphdr *)(ipPacket + ipHeader->ip_hl * 4);
	else if (ipHeader->ip_v == 6)
		tcpHeader = (const tcphdr *)(ipPacket + sizeof(ip6_hdr));
	else
		return 0; //IPv7 is here!
	
	/* sanity assured by the (Linux) kernel up to here; options can still be spurious */
	
	const uint8_t *options = (const uint8_t *)(tcpHeader + 1);
	int optionsLen = tcpHeader->doff * 4 - sizeof(tcphdr);
	
	for (int i = 0; i < optionsLen - 1;)
	{
		if (options[i] == TCPOPT_EOL)
			return 0;
		if (options[i] == TCPOPT_TFO)
			return ipHeader->ip_len - ipHeader->ip_hl * 4 - tcpHeader->doff * 4;
		if (options[i] == TCPOPT_NOP)
		{
			i++;
			continue;
		}
		
		int optlen = options[i + 1];
		if (optlen < 2)
			return 0;
		i += optlen;
	}
	
	return 0;
}

}

}

