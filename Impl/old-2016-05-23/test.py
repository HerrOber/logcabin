#!/usr/bin/env python

import Ops

clusterAddr = "127.0.0.1:5254"
timeout = 0
logPolicy = ""
ops = Ops.Ops(clusterAddr, timeout, logPolicy)
