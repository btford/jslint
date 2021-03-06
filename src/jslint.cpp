/*
Copyright (c) 2010 Sébastien Arnaud

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <v8.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "tclap/CmdLine.h"
#include "jslint.h"

using namespace v8;

namespace jslint {
    const char* const valid_options[] = {
        "adsafe", "bitwise", "browser", "cap", "css", "debug", "devel",
        "eqeqeq", "es5", "evil", "forin", "fragment", "immed", "laxbreak",
        "newcap", "nomen", "on", "onevar", "passfail", "plusplus", "regexp",
        "rhino", "safe", "strict", "sub", "undef", "white", "widget",
        "windows"};

    std::string default_options() {
        std::string options = "{passfail: true, white: true, onevar: true, undef: true,  nomen: true, eqeqeq: true, plusplus: true, bitwise: true, regexp: true, newcap: true, immed: true, strict: true}";
        return options;
    }

    std::string find_and_replace(std::string tInput, std::string tFind, std::string tReplace) {
        int uPos = 0;
        int uFindLen = tFind.length();
        int uReplaceLen = tReplace.length();

        if( uFindLen == 0 )
        {
            return tInput;
        }

        for( ;(uPos = tInput.find( tFind, uPos )) != std::string::npos; )
        {
            tInput.replace( uPos, uFindLen, tReplace );
            uPos += uReplaceLen;
        }
        return tInput;
    };

    Handle<String> load_source_js (char* src_file, std::string lint_options) {
        std::string result;
        std::string line;
        std::string jsline;
        std::ifstream myfile (src_file);
        if (myfile.is_open()) {
            //std::cout << jslint::native_fulljslint << std::endl;
            result = std::string(jslint::native_fulljslint) + "\nvar file_to_lint = [];\n";
            while (! myfile.eof() ) {
                std::getline (myfile, line);
                jsline =  "file_to_lint.push(\"" + jslint::find_and_replace(line,"\"", "\\\"") + "\");";
                result.append(jsline);
            }
            myfile.close();
            result.append("(function () { "
            "if (!JSLINT(file_to_lint, ");
            result.append(lint_options);
            result.append(
            ")) {"
            "var results = [];"
            "for(var i=0; i<JSLINT.errors.length-1; i++){"
            "var e = JSLINT.errors[i];"
            "results.push('Lint at line ' + e.line + ' character ' + e.character + ': ' + e.reason + '\\n' +(e.evidence || '').replace(/^\\s*(\\S*(\\s+\\S+)*)\\s*$/, '$1'));};"
            "return results.join('\\n');}"
            "else { return 'jslint: No problems found in "+ std::string(src_file) +"'}"
            "}());");
            //std::cout << result << std::endl;
            return String::New(result.c_str());
            } else {
                std::cout << "jslint: Couldn't open file " << src_file << std::endl;
                throw Exception();
            }
    }
}
int main(int argc, char* argv[]) {

    try {

        // Get user options
        TCLAP::CmdLine cmd("JSLint", ' ', "0.1");
        TCLAP::ValueArg<std::string> optionsArg("o", "options", "JSLint options", false, jslint::default_options(), "string", cmd);
        TCLAP::UnlabeledMultiArg<std::string> filesArg("files", "file names", true, "string", cmd);

        cmd.parse( argc, argv );

        std::string lint_options = optionsArg.getValue();
        std::vector<std::string> files = filesArg.getValue();

        for (int i=0; i < files.size(); i++) {

            std::string fileName = files[i];

            // Create a stack-allocated handle scope.
            HandleScope handle_scope;

            // Create a new context.
            Persistent<Context> context = Context::New();

            // Enter the created context for compiling and
            // running the jslint
            Context::Scope context_scope(context);

            try {
              // Create a string containing the JavaScript source code.

              Handle<String> source = jslint::load_source_js(const_cast<char *>(fileName.c_str()), lint_options);

              // Compile the source code.
              Handle<Script> script = Script::Compile(source);

              // Run the script to get the result.
              Handle<Value> result = script->Run();

              // Convert the result to an ASCII string and print it.
              String::AsciiValue ascii(result);
              printf("%s\n", *ascii);
            } catch(v8::Exception e) {
              // nothing
            }

            // Dispose the persistent context.
            context.Dispose();

        }

    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

    return 0;
}