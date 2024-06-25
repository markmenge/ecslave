#!/bin/bash
autoreconf -i
CFLAGS="-Wno-error=address-of-packed-member" ./configure
make


