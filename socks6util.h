#ifndef SOCKS6UTIL_H
#define SOCKS6UTIL_H

#include <stdlib.h>
#include <socks6msg/socks6.h>

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // SOCKS6UTIL_H
