#ifndef SOCKS6UTIL_IDEMPOTENCE_HH
#define SOCKS6UTIL_IDEMPOTENCE_HH

#include <stdint.h>
#include <vector>
#include <sys/types.h>
#include <socks6msg/socks6.h>
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
	
	TokenWallet(uint32_t base, uint32_t size)
		: base(base), current(base), size(size) {}
	
	bool extract(uint32_t *token);
	
	void updateWindow(uint32_t newBase, uint32_t newSize);
	
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
	TokenBank(uint32_t base, uint32_t size, uint32_t lowWatermark, uint32_t highWatermark);
	
	SOCKS6TokenExpenditureCode withdraw(uint32_t token);
	
	void renew();
	
	uint32_t getBase() const
	{
		return base;
	}
	
	uint32_t getSize() const
	{
		return spentTokens.size();
	}
};

}

#endif // SOCKS6UTIL_IDEMPOTENCE_HH
