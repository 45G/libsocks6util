#include "socks6util_idempotence.hh"

namespace S6U
{

static inline bool modularLess(uint32_t x, uint32_t y)
{
	return 0 < (y - x) && (x - y) < (1UL<<31);
}

static bool modularLessEqual(uint32_t x, uint32_t y)
{
	return (x - y) < (1UL<<31);
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

uint32_t TokenWallet::remaining() const
{
	return base + size - current;
}

TokenBank::TokenBank(uint32_t base, uint32_t size, uint32_t backlog, uint32_t lowWatermark, uint32_t highWatermark)
	: base(base), offset(0), backlog(backlog), lowWatermark(lowWatermark), highWatermark(highWatermark)
{
	spentTokens.resize(size, 0);
}

SOCKS6TokenExpenditureCode TokenBank::withdraw(uint32_t token)
{
	/* in window? */
	if (!(modularLessEqual(base, token) && modularLess(token, base + getSize())))
		return SOCKS6_TOK_EXPEND_OUT_OF_WND;
	
	if (spentTokens[index(token)])
		return SOCKS6_TOK_EXPEND_DUPLICATE;
	
	spentTokens[index(token)] = true;
	
	if (token - base >= highWatermark)
	{
		while (token - base > lowWatermark)
		{
			spentTokens[index(base)] = false;
			base++;
			offset = (offset + 1) % spentTokens.size();
		}
	}
	
	while (token - base > backlog && spentTokens[index(base)])
	{
		spentTokens[index(base)] = false;
		base++;
		offset = (offset + 1) % spentTokens.size();
	}
	
	return SOCKS6_TOK_EXPEND_SUCCESS;
}

}