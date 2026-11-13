#!/bin/sh
# Host test: g++ (Linux) builds src/atcore + tests/test_atcore.cpp and runs it.
# From Windows: tar c src tests | docker run --rm -i gcc:14 sh -c 'mkdir w && cd w && tar x && sh tests/run.sh'
set -e
cd "$(dirname "$0")/.."
g++ -std=c++11 -Wall -Wextra -Werror -O1 src/atcore/atcore.cpp tests/test_atcore.cpp -o /tmp/test_atcore
/tmp/test_atcore
