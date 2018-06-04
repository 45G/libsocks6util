#ifndef SOCKS6UTIL_H
#define SOCKS6UTIL_H

#include <stdlib.h>
#include <socks6msg/socks6msg.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef void S6U_TokenWallet;
typedef void S6U_TokenBank;

S6U_TokenWallet *S6U_TokenWallet_create(uint32_t base, uint32_t size);
void S6U_TokenWallet_destroy(S6U_TokenWallet *ctx);
int S6U_TokenWallet_extract(S6U_TokenWallet *ctx, uint32_t *token);
void S6U_TokenWallet_updateWindow(S6U_TokenWallet *ctx, uint32_t newBase, uint32_t newSize);
uint32_t S6U_TokenWallet_remaining(S6U_TokenWallet *ctx);

S6U_TokenBank *S6U_TokenBank_create(uint32_t base, uint32_t size, uint32_t lowWatermark, uint32_t highWatermark);
void S6U_TokenBank_destroy(S6U_TokenBank *ctx);
enum SOCKS6TokenExpenditureCode S6U_TokenBank_withdraw(S6U_TokenBank *ctx, uint32_t token);
uint32_t S6U_TokenBank_getBase(S6U_TokenBank *ctx);
uint32_t S6U_TokenBank_getSize(S6U_TokenBank *ctx);

int S6U_Packet_hasTFO(const uint8_t *ipPacket);

int S6U_Socket_saveSYN(int fd);
int S6U_Socket_tfoAttempted(int fd);
int S6U_Socket_hasMPTCP(int fd);
enum SOCKS6OperationReplyCode S6U_Socket_connectErrnoToReplyCode(int error);
int S6U_Socket_getOriginalDestination(int fd, struct sockaddr_storage *destination);

union S6U_SocketAddress
{
	struct sockaddr_storage storage;
	struct sockaddr_in ipv4;
	struct sockaddr_in6 ipv6;
};

static inline void S6U_SocketAddress_init(union S6U_SocketAddress *sa, const struct S6M_Address *addr, uint16_t port)
{
	if (addr->type == SOCKS6_ADDR_IPV4)
	{
		sa->ipv4.sin_family = AF_INET;
		sa->ipv4.sin_addr = addr->ipv4;
		sa->ipv4.sin_port = htons(port);
	}
	else if (addr->type == SOCKS6_ADDR_IPV6)
	{
		sa->ipv6.sin6_family = AF_INET6;
		sa->ipv6.sin6_addr = addr->ipv6;
		sa->ipv6.sin6_port = htons(port);
	}
	else
	{
		memset(&sa->storage, 0, sizeof(sa->storage));
	}
}

static inline ssize_t S6U_SocketAddress_size(const union S6U_SocketAddress *sa)
{
	if (sa->storage.ss_family == AF_INET)
		return sizeof(struct sockaddr_in);
	
	if (sa->storage.ss_family == AF_INET6)
		return sizeof(struct sockaddr_in6);
	
	return -1;
}

struct S6M_Address getAddress(const union S6U_SocketAddress *sa)
{
	struct S6M_Address ret;
	
	if (sa->storage.ss_family == AF_INET)
	{
		ret.type = SOCKS6_ADDR_IPV4;
		ret.ipv4 = sa->ipv4.sin_addr;
	}
	else if (sa->storage.ss_family == AF_INET6)
	{
		ret.type = SOCKS6_ADDR_IPV6;
		ret.ipv6 = sa->ipv6.sin6_addr;
	}
	else
	{
		ret.type = (enum SOCKS6AddressType)S6M_ADDRESS_INVALID_TYPE;
	}
	
	return ret;
}

static inline uint16_t S6U_SocketAddress_getPort(const union S6U_SocketAddress *sa)
{
	if (sa->storage.ss_family == AF_INET)
		return ntohs(sa->ipv4.sin_port);
	else if (sa->storage.ss_family == AF_INET6)
		return ntohs(sa->ipv6.sin6_port);
	else
		return 0;
}

static inline void S6U_SocketAddress_setPort(union S6U_SocketAddress *sa, uint16_t port)
{
	if (sa->storage.ss_family == AF_INET)
		sa->ipv4.sin_port = htons(port);
	else if (sa->storage.ss_family == AF_INET6)
		sa->ipv6.sin6_port = htons(port);
}

static inline int S6U_SocketAddress_isValid(const union S6U_SocketAddress *sa)
{
	return S6U_SocketAddress_getPort(sa) != 0;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // SOCKS6UTIL_H
