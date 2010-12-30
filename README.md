## About ##

jslint is a command line utility for checking JavaScript code quality. It runs Douglas Crockford's original
[JSlint](http://www.jslint.com) using Google's v8 JavaScript engine. Crockford says JSLint will hurt your feelings.

## Installation ##
jslint depends on Google's [v8](http://code.google.com/p/v8/), so you must have installed all of v8's dependencies. See
the [v8 build instructions](http://code.google.com/apis/v8/build.html#pre_reqs) for details.

To download and build:

    git clone git://github.com/ryangreenberg/jslint.git
    cd jslint
    git submodule init
    git submodule update
    make

## Usage ##

To run jslint with the default options:
`bin/jslint file.js`

You can run jslint on multiple files at once:
`bin/jslint file1.js file2.js file3.js`

Provide your own JSLint options with `--options`:
`bin/jslint --options "{eqeqeq: true, plusplus: true}" file.js`

Currently you must provide the options as a string surrounded by curly braces. Future versions may provide an improved syntax.

By default jslint uses "the good parts," the settings recommended by Crockford. This is equivalent to the following:

`bin/jslint --options "{white: true, onevar: true, undef: true, nomen: true, eqeqeq: true, 
plusplus: true, bitwise: true, regexp: true, newcap: true, immed: true, 
strict: true}" file.js`

jslint accepts all the options specified in the [JSLint documentation](http://www.jslint.com/lint.html#options):

`adsafe, bitwise, browser, cap, css, debug, devel, eqeqeq, es5, evil, forin, fragment, immed, indent, laxbreak, maxerr, maxlen, nomen, newcap, on, onevar, passfail, plusplus, predef, regexp, rhino, safe, strict, sub, undef, white, widget, windows`

Please file any problems using Github's issue tracker. For questions or help with JSLint visit the [JSLint Yahoo! group](http://tech.groups.yahoo.com/group/jslint_com/)

Copyright (c) 2010 Sébastien Arnaud

## Contributors ##
- Ryan Greenberg