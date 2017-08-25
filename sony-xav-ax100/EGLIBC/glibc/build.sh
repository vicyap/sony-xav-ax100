#!/bin/sh
mkdir -p buildglibc
cp glibc_ports-2010q1-202.tar.bz2 ./buildglibc
cp glibc-2010q1-202.tar.bz2 ./buildglibc
cp sp_build_glibc.sh ./buildglibc/
cd buildglibc
tar xjf ./glibc_ports-2010q1-202.tar.bz2 
tar xjf ./glibc-2010q1-202.tar.bz2 
chmod +x ./sp_build_glibc.sh
./sp_build_glibc.sh