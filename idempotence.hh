#ifndef SOCKS6UTIL_IDEMPOTENCE_HH
#define SOCKS6UTIL_IDEMPOTENCE_HH

#include <stdint.h>
#include <vector>
#include <sys/types.h>
#include <optional>
#include <socks6msg/socks6msg.hh>

namespace S6U
{

class TokenManagerBase
{
protected:
	static bool modularLess(uint32_t x, uint32_t y)
	{
		return x != y && (y - x) < (1UL << 31);
	}
	
	static bool modularLessEqual(uint32_t x, uint32_t y)
	{
		return (y - x) < (1UL << 31);
	}
};

template <typename L, typename G>
class LockableTokenManagerBase: public TokenManagerBase
{
protected:
	using Lock      = L;
	using LockGuard = G;
	
	Lock lock;
};

template <>
class LockableTokenManagerBase<void, void>: public TokenManagerBase
{
protected:
	struct Lock {};
	
	struct LockGuard
	{
		LockGuard(const Lock &) {}
	};
	
	static Lock lock;
};

template <typename L = void, typename G = void>
class TokenWallet: public TokenManagerBase<L, G>
{
	std::pair<uint32_t, uint32_t> win;
	uint32_t current;
	
public:
	TokenWallet()
		: win { 0, 0 }, current(0) {}
	
	TokenWallet(std::pair<uint32_t, uint32_t> win)
		: win(win), current(win.first) {}
	
	std::optional<uint32_t> extract()
	{
		LockGuard guard(lock);
		
		if (!modularLess(current, win.first + win.second))
			return {};
		
		return { current++ };
	}
	
	void updateWindow(std::pair<uint32_t, uint32_t> newWin)
	{
		LockGuard guard(lock);
		
		if (modularLess(newWin.first, win.first))
			return;
		
		win = newWin;
		
		if (modularLess(current, newWin.first))
			current = newWin.first;
	}
	
	void updateWindow(const S6M::OptionSet *optionSet)
	{
		LockGuard guard(lock);
		
		auto window = optionSet->idempotence.getAdvertised();
		if (window.second == 0)
			return;
		
		updateWindow(window);
	}
	
	uint32_t remaining() const
	{
		LockGuard guard(lock);
				
		return win.first + win.second - current;
	}
};

template <typename L = void, typename G = void>
class TokenBank: LockableTokenManagerBase<L, G>
{
	uint32_t base;
	uint32_t offset;

	uint32_t lowWatermark;
	uint32_t highWatermark;
	
	std::vector<bool> spentTokens;
	
	size_t index(uint32_t token)
	{
		return (token - base + offset) % spentTokens.size();
	}

public:
	TokenBank(std::pair<uint32_t, uint32_t> win, uint32_t lowWatermark, uint32_t highWatermark)
		: base(win.first), offset(0), lowWatermark(lowWatermark), highWatermark(highWatermark)
	{
		spentTokens.resize(win.second, 0);
	}
	
	bool withdraw(uint32_t token)
	{
		LockGuard guard(lock);
				
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
	
	std::pair<uint32_t, uint32_t> getWindow()
	{
		LockGuard guard(lock);
		
		return { base, spentTokens.size() };
	}
};

}

#endif // SOCKS6UTIL_IDEMPOTENCE_HH
