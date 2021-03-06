# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Build the docker image with
#    docker build --rm -t coapp .
# and run the docker image with
#    docker run --rm -v `pwd`:/src coapp

FROM ubuntu:12.04

RUN apt-get install -qq python-software-properties
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test

# Install CMake and gtest
#
RUN apt-get -qq update && apt-get install -qq cmake make libgtest-dev
RUN cmake --version

# Install g++ v4.8
#
RUN apt-get install -qq g++-4.8
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20
RUN update-alternatives --config gcc
RUN update-alternatives --config g++
RUN g++ --version


RUN cd /usr/src/gtest && cmake CMakeLists.txt && make && cp *.a /usr/lib

VOLUME ["/src"]

CMD mkdir -p /src/testbuild && cd /src/testbuild && cmake .. && make && ctest --output-on-error
