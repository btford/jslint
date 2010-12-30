#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2010 Sébastien Arnaud
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

import sys
import os
import js2c

JSLINT_SOURCE = 'lib/jslint/fulljslint.js'
JSLINT_C_OUTPUT = 'src/jslint.h'

BASE_DIR = os.path.dirname(os.path.realpath(__file__)) + "/../"

if __name__ == "__main__":
    if not os.path.exists(BASE_DIR + JSLINT_SOURCE):
      if os.path.exists(BASE_DIR + JSLINT_C_OUTPUT):
          print "WARNING: %s cannot be found. Try updating git submodules." % JSLINT_SOURCE
          print "Using existing %s" % JSLINT_C_OUTPUT
      else:
          print "ERROR: %s cannot be found.\nHave you run `git submodule init; git submodule update`?" % JSLINT_SOURCE
          sys.exit(1)
    else:
      if not os.path.exists(BASE_DIR + JSLINT_C_OUTPUT) or os.path.getmtime(BASE_DIR + JSLINT_SOURCE) > os.path.getmtime(BASE_DIR + JSLINT_C_OUTPUT):
          print "Converting %s to %s..." % (os.path.basename(JSLINT_SOURCE), os.path.basename(JSLINT_C_OUTPUT))
          js2c.JS2C(["./lib/jslint/fulljslint.js"], ["./src/jslint.h"])
      else:
          print "Already converted %s to %s" % (os.path.basename(JSLINT_SOURCE), os.path.basename(JSLINT_C_OUTPUT))