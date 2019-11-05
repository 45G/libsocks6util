#ifndef SOCKS6UTIL_IDEMPOTENCE_HH
#define SOCKS6UTIL_IDEMPOTENCE_HH

#include <stdint.h>
#include <vector>
#include <sys/types.h>
#include <optional>
#include <socks6msg/socks6msg.hh>

namespace S6U
{

class TokenWallet
{
	uint32_t base;
	uint32_t current;
	uint32_t size;
	
public:
	TokenWallet()
		: base(0), current(0), size(0) {}
	
	TokenWallet(std::pair<uint32_t, uint32_t> win)
		: base(win.first), current(base), size(win.second) {}
	
	std::optional<uint32_t> extract();
	
	void updateWindow(std::pair<uint32_t, uint32_t> window);
	
	void updateWindow(const S6M::OptionSet *optionSet);
	
	uint32_t remaining() const;
};

class TokenBank
{
	uint32_t base;
	uint32_t offset;
	
	uint32_t backlog;
	uint32_t lowWatermark;
	uint32_t highWatermark;
	
	std::vector<bool> spentTokens;
	
	size_t index(uint32_t token)
	{
		return (token - base + offset) % spentTokens.size();
	}

public:
	TokenBank(std::pair<uint32_t, uint32_t> win, uint32_t lowWatermark, uint32_t highWatermark);
	
	bool withdraw(uint32_t token);
	
	std::pair<uint32_t, uint32_t> getWindow()
	{
		return { base, spentTokens.size() };
	}
};

}

#endif // SOCKS6UTIL_IDEMPOTENCE_HH
