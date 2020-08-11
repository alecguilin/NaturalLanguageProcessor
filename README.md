
# NaturalLanguageProcessor
New-Japanese to English Translator (C++)

Group project for _CS 421 - Theory of Computation_.

This is a Natural Language Processor that translates New-Japanese to English. It takes a .txt file with sentences in New-Japanese, and provides a _translated.txt_ output file with an English translated version with the associated grammar rules. 

Some sample test cases can be found in

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_TranslatorFiles -> partCtest1, partCtest2, etc._

&nbsp;

## THE SCANNER (does Lexical Analysis)

1.  Get the next string up to a blank or end of line
2.  Call the two token functions (word DFA & period DFA) one after another (if-then-else)
	* Generate a lexical error if both DFAs failed and let the token_type be ERROR in that case.
3. For non-word tokens (i.e. period) return the token type with the actual token string
4. Make sure WORDs are checked against the reserved words table
	* If it is a reserved word return the token type and the actual string
		* e.g. PRONOUN plus "watashi"
	* If not reserved, token_type is WORD1 or WORD2 based on the last character and return the actual string
		* e.g. WORD1 plus "seito"
		* e.g. WORD2 plus "tabE"
5. Return the token type & string (pass by reference)

## THE PARSER (does Syntactic Analysis)

Implements a recursive descent parser using BNF (see _BNF.pdf_)

Two Important Functions:

tokentype next_token()
* Calls scanner to get next token
* Returns the token grabbed from scanner function

bool match(tokentype expected)
* Checks to see if expected token is different from next token
* Returns true if token matched and a syntax error if false

## THE TRANSLATOR (does Semantic Analysis)

Parser implementation but with semantic routines added (see updated rules in the _BNF.pdf_)

Maps the _lexicon.txt_ file to fill the Dictionary.

The translation from New-Japanese to English goes to an output file called _translated.txt_.
