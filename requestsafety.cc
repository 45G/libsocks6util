#include "requestsafety.hh"

namespace S6U
{

namespace RequestSafety
{

Recommendation recommend(const S6M::Request &request, bool tls, size_t totalData)
{
	/* if idempotent, anything goes */
	if (request.getOptionSet()->hasToken())
		return Recommendation(SIZE_MAX, tls, false);

	/* TLS early data can be maliciously (and heavily) replayed; TFO is safe without early data */
	if (tls)
		return Recommendation(SIZE_MAX, false, true);

	/* check for sensitive options */
	bool sensitiveRequest = request.getOptionSet()->requestedTokenWindow(); //TODO: session request
	if (sensitiveRequest)
		return Recommendation(0, false, true);

	/* only recommend using token if totalData exceeds TFO payload */
	size_t tfoTolerant = request.getOptionSet()->getTFOPayload();
	return Recommendation(request.packedSize() + tfoTolerant, false, tfoTolerant < totalData);
}

}

}
