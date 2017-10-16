#!/usr/bin/env bash
thrift -r --gen cpp -o src/rpc/ src/NFS.thrift
