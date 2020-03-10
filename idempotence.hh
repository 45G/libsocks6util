#ifndef SOCKS6UTIL_IDEMPOTENCE_HH
#define SOCKS6UTIL_IDEMPOTENCE_HH

#include <stdint.h>
#include <vector>
#include <sys/types.h>
#include <optional>
#include <socks6msg/socks6msg.hh>
#include <tbb/spin_mutex.h>

namespace S6U
{

class TokenWallet
{
	std::pair<uint32_t, uint32_t> win;
	uint32_t current;
	
public:
	TokenWallet()
		: win { 0, 0 }, current(0) {}
	
	TokenWallet(std::pair<uint32_t, uint32_t> win)
		: win(win), current(win.first) {}
	
	std::optional<uint32_t> extract();
	
	void updateWindow(std::pair<uint32_t, uint32_t> newWin);
	
	void updateWindow(const S6M::OptionSet *optionSet);
	
	uint32_t remaining() const
	{
		return win.first + win.second - current;
	}
};

class TokenBank
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
	
	bool withdraw(uint32_t token);
	
	std::pair<uint32_t, uint32_t> getWindow()
	{
		return { base, spentTokens.size() };
	}
};

class SyncedTokenBank: TokenBank
{
	tbb::spin_mutex spinlock;

public:
	using TokenBank::TokenBank;

	bool withdraw(uint32_t token)
	{
		tbb::spin_mutex::scoped_lock lock(spinlock);
		return TokenBank::withdraw(token);
	}

	std::pair<uint32_t, uint32_t> getWindow()
	{
		tbb::spin_mutex::scoped_lock lock(spinlock);
		return TokenBank::getWindow();
	}
};

class SyncedTokenWallet: TokenWallet
{
	mutable tbb::spin_mutex spinlock;

public:
	using TokenWallet::TokenWallet;

	std::optional<uint32_t> extract()
	{
		tbb::spin_mutex::scoped_lock lock(spinlock);
		return TokenWallet::extract();
	}

	void updateWindow(std::pair<uint32_t, uint32_t> window)
	{
		tbb::spin_mutex::scoped_lock lock(spinlock);
		return TokenWallet::updateWindow(window);
	}

	void updateWindow(const S6M::OptionSet *optionSet)
	{
		tbb::spin_mutex::scoped_lock lock(spinlock);
		return TokenWallet::updateWindow(optionSet);
	}

	uint32_t remaining() const
	{
		tbb::spin_mutex::scoped_lock lock(spinlock);
		return TokenWallet::remaining();
	}
};

}

#endif // SOCKS6UTIL_IDEMPOTENCE_HH
