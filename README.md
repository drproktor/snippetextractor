# How to extract snippets from source code and add them to Markdown articles

Samples copy-pasted from application code will invariably go stale and be out of
date. Such duplication is avoided by retrieving the code samples from the
application code and inserting them into the Markdown documents when needed.
SnippetExtractor is a filter program implementing this functionality.

## Building, installing and running

SnippetExtractor is written in C++ and uses the Qt framework. It uses the QMake
build system. The program takes two parameters, the input and the output file:

~~~ 
: > snippetextractor article.md.in article.md
~~~

## Markup of code samples

Code samples are inserted into a document by a combination of an include
instruction in the article file that specify the snippet to include and the
source code file to take it from, and matching special comments in the source
file that specify the beginning and end of the code sample. The syntax of the
include instruction looks like this:

~~~
@@snippet(HelloWorld/HelloWorld.cpp, helloworld, cpp)
~~~

The first parameter specifies the file that contains the code snippet. The
second parameter contains the name of the snippet as it is marked in the source
file. Multiple snippets may be defined in a single source file, however each
needs to have a unique name. The third parameter chooses a language for the
generated Markdown code, the value has to be a name of a language supported by
Pandoc (or whatever processes the Markdown file later). The snippet instructions
have to be written in one separate line. This line will be replaced with the
content of the snippet. Snippet instructions cannot be nested. The matching
magic comments in the source file are written in the following format:

~~~
//@@snippet_begin(helloworld)
using namespace std; 
cout << "Hello World!" << endl;
//@@snippet_end(helloworld)
~~~

The tool will look up the file, extract the snippet from it, and generate
markdown output to render the code sample in the article. The code sample will
include the lines between the delimiters, but not the delimiters themselves. It
will also add the correct line numbers to the listing.

## Generating the Markdown files

The SnippetExtractor filter needs to run on the Markdown input file that
contains the include instruction, and will then generate a complete Markdown
file from it. Usually this process is automated by `make` tools. A custom rule
for the popular `latexmk` tool to process Latex documents would look something
like this (remember to make sure snippetextractor is in the search path):

~~~
add_cus_dep('md.in', 'md', 0, 'mdin2md');
sub mdin2md {
    return system("snippetextractor '$_[0]'.md.in '$_[0]'.md");
}
~~~

# Extracting snippets on the command lines

SnippetExtractor can also be used extract a particular snippet directly from a
source file without parsing a markdown file before hand. To use this feature run

~~~
: > snippetextractor --snippet <mode> <source file> <snippet name> <style>
~~~

Here `<mode>` is currently either `markdown` or `minted` for markdown or
LaTeX [`minted`](https://www.ctan.org/pkg/minted) code snippets. `<source file>`
is the path to the source file, `<snippet name>` is the tag of the snippet, and
`<style>` is the style of the snippet. Note that the name of the style is
depending on the mode, e.g. `minted` supports a very wide range of languages
(actually all languages supported by the Python syntax
highlighter [Pygments](http://pygments.org/)).

For example, running

~~~
: > snippetextractor --snippet minted sample.cpp sample-1 c++
~~~

on the file `sample.cpp`

~~~
#include <iostream>

int main(int argc, const char *argv[]) {
  //@@snippet_begin(sample-1)
  std::cout << "Hello World!" << std::endl;

  return 0;
  //@@snippet_end(sample-1)
}
~~~

will produce 

~~~
\begin{minted}[breaklines]{c++}
  std::cout << "Hello World!" << std::endl;

  return 0;
\end{minted}
~~~

and directly return it on the command line (standard output). This can be useful
to directly include snippets into your LaTeX documents, e.g. via

~~~
\input{|"snippetextractor --snippet minted sample.cpp sample-1 c++"}
~~~

Note, that when you want to use this you have to call LaTeX with
`-shell-escape`, which is necessary for `minted` anyway.
