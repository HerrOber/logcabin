#!/usr/bin/env python

import Ops

print "hehe"
clusterAddr = "127.0.0.1:5254"
timeout = 0
logPolicy = ""
print "ops constructor"
ops = Ops.Ops(clusterAddr, timeout, logPolicy)
