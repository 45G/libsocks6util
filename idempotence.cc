#include "idempotence.hh"

namespace S6U
{

static inline bool modularLess(uint32_t x, uint32_t y)
{
	return 0 < (y - x) && (y - x) < (1UL << 31);
}

static inline bool modularLessEqual(uint32_t x, uint32_t y)
{
	return (y - x) < (1UL << 31);
}

bool TokenWallet::extract(uint32_t *token)
{
	if (!modularLess(current, base + size))
		return false;
	
	*token = current;
	current++;
	
	return true;
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
	uint32_t newSize = optionSet->getTokenWindowSize();
	if (newSize == 0)
		return;
	
	uint32_t newBase = optionSet->getTokenWindowBase();
	
	updateWindow(newBase, newSize);
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

SOCKS6TokenExpenditureCode TokenBank::withdraw(uint32_t token)
{
	if (!(modularLessEqual(base, token) && modularLess(token, base + getSize())))
		return SOCKS6_TOK_EXPEND_OUT_OF_WND;
	
	if (spentTokens[index(token)])
		return SOCKS6_TOK_EXPEND_DUPLICATE;
	
	spentTokens[index(token)] = true;
	
	while (modularLessEqual(base + highWatermark, token) || (modularLessEqual(base + lowWatermark, token) && spentTokens[index(base)]))
	{
		spentTokens[index(base)] = false;
		base++;
		offset = (offset + 1) % spentTokens.size();
	}
	
	return SOCKS6_TOK_EXPEND_SUCCESS;
}

void TokenBank::renew()
{
	uint32_t newBase = base + spentTokens.size();
	while (base != newBase)
	{
		spentTokens[index(base)] = false;
		base++;
		offset = (offset + 1) % spentTokens.size();
	}
}

}
