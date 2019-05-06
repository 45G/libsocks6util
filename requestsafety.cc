#include "requestsafety.hh"

namespace S6U
{

namespace RequestSafety
{

Recommendation recommend(const S6M::Request &request, bool tls, size_t totalData)
{
	/* if idempotent, anything goes */
	if (request.getOptionSet()->idempotence.getToken())
		return Recommendation(SIZE_MAX, tls, false);

	/* TLS early data can be maliciously (and heavily) replayed; TFO is safe without early data */
	if (tls)
		return Recommendation(SIZE_MAX, false, true);

	/* check for sensitive options */
	bool sensitiveRequest = request.getOptionSet()->session.requested() ||
		request.getOptionSet()->idempotence.requestedSize() > 0;
	if (sensitiveRequest)
		return Recommendation(0, false, true);

	/* only recommend using token if totalData exceeds TFO payload */
	size_t tfoTolerant = request.getOptionSet()->stack.tfo.get(SOCKS6_STACK_LEG_PROXY_REMOTE).get();
	return Recommendation(request.packedSize() + tfoTolerant, false, tfoTolerant < totalData);
}

}

}
