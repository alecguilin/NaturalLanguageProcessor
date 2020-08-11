# NaturalLanguageProcessor
New-Japanese to English Translator (C++)

Group project for _CS 421 - Theory of Computation_.

This Natural Language Processor is a New-Japanese to English Translator that takes a .txt file with sentences in New-Japanese, and provides a _translated.txt_ output file with an English translated version with the associated grammar rules. 

Some sample test cases can be found in

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_TranslatorFiles -> partCtest1, partCtest2, etc._

&nbsp;

## THE SCANNER (does Lexical Analysis)

1.  Get the next string up to a blank or end of line
2.  Call the two token functions (word DFA & period DFA) one after another (if-then-else)

  *And generate a lexical error if both DFAs failed and
  *let the token_type be ERROR in that case.

3.
4.
5.

## THE PARSER (does Syntactic Analysis)

Parser

## THE TRANSLATOR (does Semantic Analysis)

Translator
