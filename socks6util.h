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
typedef void S6U_SyncedTokenWallet;
typedef void S6U_SyncedTokenBank;

S6U_TokenWallet *S6U_TokenWallet_create(uint32_t base, uint32_t size);
void S6U_TokenWallet_destroy(S6U_TokenWallet *ctx);
int S6U_TokenWallet_extract(S6U_TokenWallet *ctx, uint32_t *token);
void S6U_TokenWallet_updateWindow(S6U_TokenWallet *ctx, uint32_t newBase, uint32_t newSize);
uint32_t S6U_TokenWallet_remaining(S6U_TokenWallet *ctx);

S6U_TokenBank *S6U_TokenBank_create(uint32_t base, uint32_t size, uint32_t lowWatermark, uint32_t highWatermark);
void S6U_TokenBank_destroy(S6U_TokenBank *ctx);
int S6U_TokenBank_withdraw(S6U_TokenBank *ctx, uint32_t token);
uint32_t S6U_TokenBank_getBase(S6U_TokenBank *ctx);
uint32_t S6U_TokenBank_getSize(S6U_TokenBank *ctx);

S6U_SyncedTokenWallet *S6U_SyncedTokenWallet_create(uint32_t base, uint32_t size);
void S6U_SyncedTokenWallet_destroy(S6U_SyncedTokenWallet *ctx);
int S6U_SyncedTokenWallet_extract(S6U_SyncedTokenWallet *ctx, uint32_t *token);
void S6U_SyncedTokenWallet_updateWindow(S6U_SyncedTokenWallet *ctx, uint32_t newBase, uint32_t newSize);
uint32_t S6U_SyncedTokenWallet_remaining(S6U_SyncedTokenWallet *ctx);

S6U_SyncedTokenBank *S6U_SyncedTokenBank_create(uint32_t base, uint32_t size, uint32_t lowWatermark, uint32_t highWatermark);
void S6U_SyncedTokenBank_destroy(S6U_SyncedTokenBank *ctx);
int S6U_SyncedTokenBank_withdraw(S6U_SyncedTokenBank *ctx, uint32_t token);
uint32_t S6U_SyncedTokenBank_getBase(S6U_SyncedTokenBank *ctx);
uint32_t S6U_SyncedTokenBank_getSize(S6U_SyncedTokenBank *ctx);

int S6U_Packet_hasTFO(const uint8_t *ipPacket);

static const struct in_addr S6U_SOCKET_QUAD_ZERO = { .s_addr = 0 };
int S6U_Socket_saveSYN(int fd);
ssize_t S6U_Socket_tfoAttempted(int fd);
int S6U_Socket_hasMPTCP(int fd);
enum SOCKS6OperationReplyCode S6U_Socket_connectErrnoToReplyCode(int error);
int S6U_Socket_getOriginalDestination(int fd, struct sockaddr_storage *destination);
int S6U_Socket_pendingRecv(int fd);

union S6U_SocketAddress
{
	struct sockaddr_storage storage;
	struct sockaddr_in ipv4;
	struct sockaddr_in6 ipv6;
	struct sockaddr sockAddress;
};

void S6U_SocketAddress_init(union S6U_SocketAddress *sa, const struct S6M_Address *addr, uint16_t port);
ssize_t S6U_SocketAddress_size(const union S6U_SocketAddress *sa);
struct S6M_Address S6U_SocketAddress_getAddress(const union S6U_SocketAddress *sa);
uint16_t S6U_SocketAddress_getPort(const union S6U_SocketAddress *sa);
void S6U_SocketAddress_setPort(union S6U_SocketAddress *sa, uint16_t port);
int S6U_SocketAddress_isValid(const union S6U_SocketAddress *sa);

//TODO: request safety

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // SOCKS6UTIL_H
