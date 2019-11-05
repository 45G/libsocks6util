#include "idempotence.hh"

using namespace std;

namespace S6U
{

static inline bool modularLess(uint32_t x, uint32_t y)
{
	return x != y && (y - x) < (1UL << 31);
}

static inline bool modularLessEqual(uint32_t x, uint32_t y)
{
	return (y - x) < (1UL << 31);
}

optional<uint32_t> TokenWallet::extract()
{
	if (!modularLess(current, base + size))
		return {};
	
	return { current++ };
}

void TokenWallet::updateWindow(std::pair<uint32_t, uint32_t> window)
{
	if (modularLess(window.first, base))
		return;
	
	base = window.first;
	size = window.second;
	
	if (modularLess(current, window.first))
		current = window.first;
}

void TokenWallet::updateWindow(const S6M::OptionSet *optionSet)
{
	auto window = optionSet->idempotence.getAdvertised();
	if (window.second == 0)
		return;
	
	updateWindow(window);
}

uint32_t TokenWallet::remaining() const
{
	return base + size - current;
}

TokenBank::TokenBank(std::pair<uint32_t, uint32_t> win, uint32_t lowWatermark, uint32_t highWatermark)
	: base(win.first), offset(0), lowWatermark(lowWatermark), highWatermark(highWatermark)
{
	spentTokens.resize(win.second, 0);
}

bool TokenBank::withdraw(uint32_t token)
{
	if (!(modularLessEqual(base, token) && modularLess(token, base + spentTokens.size())))
		return false;
	
	if (spentTokens[index(token)])
		return false;
	
	spentTokens[index(token)] = true;
	
	while (modularLessEqual(base + highWatermark, token) || (modularLessEqual(base + lowWatermark, token) && spentTokens[index(base)]))
	{
		spentTokens[index(base)] = false;
		base++;
		offset = (offset + 1) % spentTokens.size();
	}
	
	return true;
}

}
