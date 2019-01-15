#ifndef SOCKS6UTIL_REQUESTSAFETY_HH
#define SOCKS6UTIL_REQUESTSAFETY_HH

#include <stdlib.h>
#include <socks6msg/request.hh>

namespace S6U
{

namespace RequestSafety
{

struct Recommendation
{
	size_t tfoPayload;
	bool earlyData;
	bool useToken;

	Recommendation()
		: tfoPayload(0), earlyData(false), useToken(true) {}

	Recommendation(size_t tfoPayload, bool earlyData, bool useToken)
		: tfoPayload(tfoPayload), earlyData(earlyData), useToken(useToken) {}

	void tokenSpent(bool tls)
	{
		tfoPayload = SIZE_MAX;
		earlyData = tls;
		useToken = false;
	}
};

/* Assumptions:
 * proxy tolerates rejecting duplicate idempotent requests
 * maliciously replayed requests done over TLS should always be avoided
 * accidentally replayed requests, without the TFO payload, are ok if replaying the side-effects of the options are tolerable
 * accidentally replayed TFO payloads are ok
 */
Recommendation recommend(const S6M::Request &req, bool tls, size_t totalData);

}

}

#endif // SOCKS6UTIL_REQUESTSAFETY_HH
