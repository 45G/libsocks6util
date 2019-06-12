#include <string.h>
#include <errno.h>
#include <exception>
#include <socks6msg/address.hh>
#include "socks6util.h"
#include "socks6util.hh"

using namespace std;
using namespace S6U;

#define S6U_CATCH \
	catch (bad_alloc) \
	{ \
		errno = ENOMEM; \
	} \
	catch (...) {}

S6U_TokenWallet *S6U_TokenWallet_create(uint32_t base, uint32_t size)
{
	try
	{
		return new TokenWallet(base, size);
	}
	S6U_CATCH;
	
	return nullptr;
}

void S6U_TokenWallet_destroy(S6U_TokenWallet *ctx)
{
	TokenWallet *wallet = reinterpret_cast<TokenWallet *>(ctx);
	
	delete wallet;
}

int S6U_TokenWallet_extract(S6U_TokenWallet *ctx, uint32_t *token)
{
	TokenWallet *wallet = reinterpret_cast<TokenWallet *>(ctx);
	
	auto extracted = wallet->extract();
	if (!extracted)
		return 0;
	
	*token = extracted.get();
	
	return 1;
}

void S6U_TokenWallet_updateWindow(S6U_TokenWallet *ctx, uint32_t newBase, uint32_t newSize)
{
	TokenWallet *wallet = reinterpret_cast<TokenWallet *>(ctx);
	
	wallet->updateWindow(newBase, newSize);
}

uint32_t S6U_TokenWallet_remaining(S6U_TokenWallet *ctx)
{
	TokenWallet *wallet = reinterpret_cast<TokenWallet *>(ctx);
	
	return wallet->remaining();
}

S6U_TokenBank *S6U_TokenBank_create(uint32_t base, uint32_t size, uint32_t lowWatermark, uint32_t highWatermark)
{
	try
	{
		return new TokenBank(base, size, lowWatermark, highWatermark);
	}
	S6U_CATCH;
	
	return nullptr;
}

void S6U_TokenBank_destroy(S6U_TokenBank *ctx)
{
	TokenBank *bank = reinterpret_cast<TokenBank *>(ctx);
	
	delete bank;
}

int S6U_TokenBank_withdraw(S6U_TokenBank *ctx, uint32_t token)
{
	TokenBank *bank = reinterpret_cast<TokenBank *>(ctx);
	
	return bank->withdraw(token);
}

uint32_t S6U_TokenBank_getBase(S6U_TokenBank *ctx)
{
	TokenBank *bank = reinterpret_cast<TokenBank *>(ctx);
	
	return bank->getBase();
}

uint32_t S6U_TokenBank_getSize(S6U_TokenBank *ctx)
{
	TokenBank *bank = reinterpret_cast<TokenBank *>(ctx);
	
	return bank->getSize();
}

int S6U_Packet_hasTFO(const uint8_t *ipPacket)
{
	return (int)Packet::tfoPayloadSize(ipPacket);
}

int S6U_Socket_saveSYN(int fd)
{
	return Socket::saveSYN(fd);
}

ssize_t S6U_Socket_tfoAttempted(int fd)
{
	return Socket::tfoPayloadSize(fd);
}

int S6U_Socket_hasMPTCP(int fd)
{
	return Socket::hasMPTCP(fd);
}

SOCKS6OperationReplyCode S6U_Socket_connectErrnoToReplyCode(int error)
{
	return Socket::connectErrnoToReplyCode(error);
}

int S6U_Socket_getOriginalDestination(int fd, sockaddr_storage *destination)
{
	return Socket::getOriginalDestination(fd, destination);
}

int S6U_Socket_pendingRecv(int fd)
{
	return S6U::Socket::pendingRecv(fd);
}

static void S6M_Addr_Fill(S6M_Address *cAddr, const S6M::Address *cppAddr)
{
	cAddr->type = cppAddr->getType();
	
	switch (cppAddr->getType())
	{
	case SOCKS6_ADDR_IPV4:
		cAddr->ipv4 = cppAddr->getIPv4();
		break;
		
	case SOCKS6_ADDR_IPV6:
		cAddr->ipv6 = cppAddr->getIPv6();
		break;
		
	case SOCKS6_ADDR_DOMAIN:
		cAddr->domain = const_cast<char *>(cppAddr->getDomain()->c_str());
		if (cAddr->domain == nullptr)
			throw bad_alloc();
		break;
	}
}

//TODO: work around code duplication (these are originally from libsocks6msg)
static S6M::Address S6M_Addr_Flush(const S6M_Address *cAddr)
{
	switch (cAddr->type)
	{
	case SOCKS6_ADDR_IPV4:
		return S6M::Address(cAddr->ipv4);
		
	case SOCKS6_ADDR_IPV6:
		return S6M::Address(cAddr->ipv6);
		
	case SOCKS6_ADDR_DOMAIN:
		return S6M::Address(string(cAddr->domain));
	}
	
	throw invalid_argument("Bad address type");
}

void S6U_SocketAddress_init(S6U_SocketAddress *sa, const S6M_Address *addr, uint16_t port)
{
	SocketAddress *cppSA = reinterpret_cast<SocketAddress *>(sa);
	
	try
	{
		S6M::Address cppAddr = S6M_Addr_Flush(addr);
		*cppSA = SocketAddress(cppAddr, port);
	}
	catch (exception)
	{
		memset(&sa->storage, 0, sizeof(sa->storage));
	}
}

ssize_t S6U_SocketAddress_size(const S6U_SocketAddress *sa)
{
	const SocketAddress *cppSA = reinterpret_cast<const SocketAddress *>(sa);
	
	return cppSA->size();
}

S6M_Address S6U_SocketAddress_getAddress(const S6U_SocketAddress *sa)
{
	const SocketAddress *cppSA = reinterpret_cast<const SocketAddress *>(sa);
	S6M_Address cAddr;
	
	try
	{
		S6M::Address cppAddr = cppSA->getAddress();
		
		S6M_Addr_Fill(&cAddr, &cppAddr);
	}
	catch (exception)
	{
		cAddr.type = SOCKS6_ADDR_IPV4;
		cAddr.ipv4.s_addr = 0;
	}
	
	return cAddr;
}

uint16_t S6U_SocketAddress_getPort(const S6U_SocketAddress *sa)
{
	const SocketAddress *cppSA = reinterpret_cast<const SocketAddress *>(sa);
	
	return cppSA->getPort();
}

void S6U_SocketAddress_setPort(S6U_SocketAddress *sa, uint16_t port)
{
	SocketAddress *cppSA = reinterpret_cast<SocketAddress *>(sa);
	
	cppSA->setPort(port);
}

int S6U_SocketAddress_isValid(const S6U_SocketAddress *sa)
{
	const SocketAddress *cppSA = reinterpret_cast<const SocketAddress *>(sa);
	
	return cppSA->isValid();
}

//TODO: binding for request safety
