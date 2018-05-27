#!/bin/sh

../cmake-build-debug/acc $1 &&
../cmake-build-debug/avm a.abc;
