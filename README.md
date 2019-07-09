# libsocks6util
Utility library for SOCKS 6 apps

## Building the library

You will need the following packages:
 * qt4-qmake
 * Boost (libboost-dev(el))
 * libsocks6msg (you can get it here: [https://github.com/45G/libsocks6msg])
 
Afterwards, simply run:

```
qmake
make
sudo make install
```

By default, everything is placed in /usr/local. You can edit socks6msg.pro to change that.
