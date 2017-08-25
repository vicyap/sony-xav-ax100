#!/bin/sh
make -j 8
make install DESTDIR=$PWD/output