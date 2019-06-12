#include "requestsafety.hh"

namespace S6U
{

namespace RequestSafety
{

Recommendation recommend(const S6M::Request &request, bool tls, size_t totalData)
{
	/* if idempotent, anything goes */
	if (request.options.idempotence.getToken())
		return Recommendation(SIZE_MAX, tls, false);

	/* TLS early data can be maliciously (and heavily) replayed; TFO is safe without early data */
	if (tls)
		return Recommendation(SIZE_MAX, false, true);

	/* check for sensitive options */
	bool sensitiveRequest = request.options.session.requested();
	if (sensitiveRequest)
		return Recommendation(0, false, true);

	/* only recommend using token if totalData exceeds TFO payload */
	size_t tfoTolerant = request.options.stack.tfo.get(SOCKS6_STACK_LEG_PROXY_REMOTE).get_value_or(0);
	return Recommendation(request.packedSize() + tfoTolerant, false, tfoTolerant < totalData);
}

}

}
