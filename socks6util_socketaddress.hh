#ifndef SOCKS6UTIL_SOCKETADDRESS_HH
#define SOCKS6UTIL_SOCKETADDRESS_HH

#include <string.h>
#include <netinet/in.h>
#include <socks6msg/socks6msg_address.hh>

namespace S6U
{

union SocketAddress
{
	sockaddr_storage storage;
	sockaddr_in ipv4;
	sockaddr_in6 ipv6;
	sockaddr sockAddress;
	
	SocketAddress()
	{
		memset(&storage, 0, sizeof(storage));
	}
	
	SocketAddress(const SocketAddress &other)
	{
		storage = other.storage;
	}
	
	SocketAddress(in_addr ipv4a, uint16_t port = 0)
	{
		ipv4.sin_family = AF_INET;
		ipv4.sin_addr = ipv4a;
		ipv4.sin_port = htons(port);
	}
	
	SocketAddress(in6_addr ipv6a, uint16_t port = 0)
	{
		ipv6.sin6_family = AF_INET6;
		ipv6.sin6_addr = ipv6a;
		ipv6.sin6_port = htons(port);
	}
	
	SocketAddress(const sockaddr_storage &storage)
		: storage(storage) {}
	
	SocketAddress(const sockaddr_in &ipv4)
		: ipv4(ipv4) {}
	
	SocketAddress(const sockaddr_in6 &ipv6)
		: ipv6(ipv6) {}
	
	SocketAddress(const S6M::Address &addr, uint16_t port)
	{
		if (addr.getType() == SOCKS6_ADDR_IPV4)
		{
			ipv4.sin_family = AF_INET;
			ipv4.sin_addr = addr.getIPv4();
			ipv4.sin_port = htons(port);
		}
		else if (addr.getType() == SOCKS6_ADDR_IPV6)
		{
			ipv6.sin6_family = AF_INET6;
			ipv6.sin6_addr = addr.getIPv6();
			ipv6.sin6_port = htons(port);
		}
		else
		{
			memset(&storage, 0, sizeof(storage));
		}
	}
	
	ssize_t size() const
	{
		if (storage.ss_family == AF_INET)
			return sizeof(sockaddr_in);
		
		if (storage.ss_family == AF_INET6)
			return sizeof(sockaddr_in6);
		
		return -1;
	}
	
	S6M::Address getAddress() const
	{
		if (storage.ss_family == AF_INET)
			return S6M::Address(ipv4.sin_addr);
		else if (storage.ss_family == AF_INET6)
			return S6M::Address(ipv6.sin6_addr);
		else
			return S6M::Address();
	}
	
	uint16_t getPort() const
	{
		if (storage.ss_family == AF_INET)
			return ntohs(ipv4.sin_port);
		else if (storage.ss_family == AF_INET6)
			return ntohs(ipv6.sin6_port);
		else
			return 0;
	}
	
	void setPort(uint16_t port)
	{
		if (storage.ss_family == AF_INET)
			ipv4.sin_port = htons(port);
		else if (storage.ss_family == AF_INET6)
			ipv6.sin6_port = htons(port);
	}
	
	bool isValid() const
	{
		return getPort() != 0;
	}
};

}

#endif // SOCKS6UTIL_SOCKETADDRESS_HH
