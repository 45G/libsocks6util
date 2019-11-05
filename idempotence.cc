#include "idempotence.hh"

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

boost::optional<uint32_t> TokenWallet::extract()
{
	if (!modularLess(current, base + size))
		return {};
	
	return { current++ };
}

void TokenWallet::updateWindow(uint32_t newBase, uint32_t newSize)
{
	if (modularLess(newBase, base))
		return;
	
	base = newBase;
	size = newSize;
	
	if (modularLess(current, newBase))
		current = newBase;
}

void TokenWallet::updateWindow(const S6M::OptionSet *optionSet)
{
	auto window = optionSet->idempotence.getAdvertised();
	if (window.second == 0)
		return;
	
	updateWindow(window.first, window.second);
}

uint32_t TokenWallet::remaining() const
{
	return base + size - current;
}

TokenBank::TokenBank(uint32_t base, uint32_t size, uint32_t lowWatermark, uint32_t highWatermark)
	: base(base), offset(0), lowWatermark(lowWatermark), highWatermark(highWatermark)
{
	spentTokens.resize(size, 0);
}

bool TokenBank::withdraw(uint32_t token)
{
	if (!(modularLessEqual(base, token) && modularLess(token, base + getSize())))
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
