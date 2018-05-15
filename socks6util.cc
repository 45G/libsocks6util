#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include "socks6util.h"

#define TCPOPT_EOL 0
#define TCPOPT_NOP 1
#define TCPOPT_TFO 34

#ifndef TCP_SAVE_SYN
#define TCP_SAVE_SYN 27
#endif

#ifndef TCP_SAVED_SYN
#define TCP_SAVED_SYN 28
#endif

#ifndef MPTCP_PATH_MANAGER
#define MPTCP_PATH_MANAGER 43
#endif

#define BUF_SIZE 1500

int S6U_Socket_SaveSYN(int fd)
{
	static const uint32_t one = 1;
	
	return setsockopt(fd, SOL_TCP, TCP_SAVE_SYN, &one, sizeof(one));
}

int S6U_Socket_TFOAttempted(int fd)
{
	uint8_t buf[BUF_SIZE];
	socklen_t bufSize = BUF_SIZE;
	
	int err = getsockopt(fd, SOL_TCP, TCP_SAVED_SYN, buf, &bufSize);
	if (err < 0)
		return err;
	
	return S6U_Packet_HasTFO(buf);
}

int S6U_Socket_HasMPTCP(int fd)
{
	char opt[20];
	socklen_t opt_size = sizeof(opt);
	
	int err = getsockopt(fd, SOL_TCP, MPTCP_PATH_MANAGER, opt, &opt_size);
	if (err < 0)
		return err;
	
	return opt[0] != '\0';
}

int S6U_Packet_HasTFO(const uint8_t *ipPacket)
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
			return 1;
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
