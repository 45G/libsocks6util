#ifndef SOCKS6UTIL_H
#define SOCKS6UTIL_H

//TODO: find nice way to include this
#include "../libsocks6msg/socks6.h"

/* C-only headers */
#include "socks6util_socket.h"
#include "socks6util_packet.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef void S6U_IdempotenceClientContext;
typedef void S6U_IdempotenceServerContext;

S6U_IdempotenceServerContext *S6U_IdempotenceServerContext_create();
void S6U_IdempotenceServerContext_destroy(S6U_IdempotenceServerContext *ctx);
int S6U_IdempotenceServerContext_issueToken(S6U_IdempotenceServerContext *ctx, uint32_t *token);
void S6U_IdempotenceServerContext_updateWindow(S6U_IdempotenceServerContext *ctx, uint32_t newBase, uint32_t newSize);
uint32_t S6U_IdempotenceServerContext_remaining(S6U_IdempotenceServerContext *ctx);

IdempotenceServerContext *IdempotenceServerContext_create(uint32_t base, uint32_t size, uint32_t backlog, uint32_t lowWatermark, uint32_t highWatermark);
SOCKS6TokenExpenditureCode IdempotenceServerContext_spend(IdempotenceServerContext *ctx, uint32_t token);
uint32_t IdempotenceServerContext_getBase(IdempotenceServerContext *ctx);
uint32_t getSize(IdempotenceServerContext *ctx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // SOCKS6UTIL_H
