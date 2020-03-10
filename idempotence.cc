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
	if (!modularLess(current, win.first + win.second))
		return {};
	
	return { current++ };
}

void TokenWallet::updateWindow(std::pair<uint32_t, uint32_t> newWin)
{
	if (modularLess(newWin.first, win.first))
		return;
	
	win = newWin;
	
	if (modularLess(current, newWin.first))
		current = newWin.first;
}

void TokenWallet::updateWindow(const S6M::OptionSet *optionSet)
{
	auto window = optionSet->idempotence.getAdvertised();
	if (window.second == 0)
		return;
	
	updateWindow(window);
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
