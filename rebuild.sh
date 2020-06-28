#!/bin/bash

rm -rf test_package/build ~/.conan/data/ssge-server;
conan create . demo/testing;
