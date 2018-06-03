#include <exception>
#include <errno.h>
#include "socks6util.h"
#include "socks6util_idempotence.hh"
#include "socks6util_packet.hh"
#include "socks6util_socket.hh"

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
	
	return NULL;
}

void S6U_TokenWallet_destroy(S6U_TokenWallet *ctx)
{
	TokenWallet *wallet = reinterpret_cast<TokenWallet *>(ctx);
	
	delete wallet;
}

int S6U_TokenWallet_extract(S6U_TokenWallet *ctx, uint32_t *token)
{
	TokenWallet *wallet = reinterpret_cast<TokenWallet *>(ctx);
	
	return wallet->extract(token);
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
	
	return NULL;
}

void S6U_TokenBank_destroy(S6U_TokenBank *ctx)
{
	TokenBank *bank = reinterpret_cast<TokenBank *>(ctx);
	
	delete bank;
}

SOCKS6TokenExpenditureCode S6U_TokenBank_withdraw(S6U_TokenBank *ctx, uint32_t token)
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
	return (int)Packet::hasTFO(ipPacket);
}

int S6U_Socket_saveSYN(int fd)
{
	return Socket::saveSYN(fd);
}

int S6U_Socket_tfoAttempted(int fd)
{
	return Socket::tfoAttempted(fd);
}

int S6U_Socket_hasMPTCP(int fd)
{
	return Socket::hasMPTCP(fd);
}

SOCKS6OperationReplyCode S6U_Socket_connectErrnoToReplyCode(int error)
{
	return Socket::connectErrnoToReplyCode(error);
}
