#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "scanner.cpp"

using namespace std;

// ----- Four Utility Functions and Globals -----------------------------------
void s();
void afterSubject();
void afterNoun();
void afterObject();
void noun();
void verb();
void be();
void tense();

string saved_lexeme;
tokentype saved_token;
bool token_avail = false;


// Type of error: Token mismatch - next token doesn't match what grammar expects
void syntaxerror1(tokentype expected, string saved_lexeme) {
	cout << "SYNTAX ERROR: expected " << tokenName[expected] << " but found " << saved_lexeme << endl;
	exit(1);
}

// Type of error: Invalid token - default case of parser function reached
void syntaxerror2(string saved_lexeme, string parser_Function) {
	cout << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in " << parser_Function << endl;
	exit(1);
}

// Purpose: Call scanner to get next token 
// Returns: the token grabbed from scanner function
tokentype next_token() {
	
	// if no token already saved
	if (!token_avail) {	
		cout << "Scanner called using word: " << saved_lexeme << endl;		
		scanner(saved_token, saved_lexeme);				
		token_avail = true;					
	}
	return saved_token;	
}

// Purpose: Checks to see if expected token is different from next token
// Returns: true if token matched syntax error if false
bool match(tokentype expected) {
	
	if (next_token() != expected) {				// mismatch has occurred		
		syntaxerror1(expected, saved_lexeme);	// generate a syntax error		
	}
	else {
		cout << "Matched " << tokenName[expected] << endl;
		token_avail = false;					// set to false to get new token
		return true;
	}
}


//  ------------------- RDP functions - one per non-term -------------------

// Grammar: <story>::= <s>{<s>}
void story() {
	cout << "Processing <story> ...\n\n";
	s();

	// keep calling s() until end of file marker is reached
	while (saved_lexeme != "eofm")
		s();
	cout << "\nSuccessfully parsed <story>.\n";
}

// Grammar: <s>::= [CONNECTOR] <noun> SUBJECT <afterSubject>
void s() {

	// get the next token
	next_token();
	
	// if not at end of file 
	if (saved_lexeme != "eofm") {
		cout << "Processing <s>\n";

		//next_token(); // grab next token
		if (next_token() == CONNECTOR)
			match(CONNECTOR);
		noun();
		match(SUBJECT);
		afterSubject();
	}
}

// Grammar: <afterSubject>::= <verb> <tense> PERIOD | <noun> <afterNoun>
void afterSubject() {

	cout << "Processing <afterSubject>\n";

	switch (next_token())
	{
	case WORD2:
		verb();
		tense();
		match(PERIOD);
		break;

	case WORD1:
		noun();
		afterNoun();
		break;

	case PRONOUN:
		noun();
		afterNoun();
		break;

	default:
		syntaxerror2(saved_lexeme, "afterSubject"); //generate syntax error if unexpected lexeme
	}
}

// Grammar: <afterNoun>::= <be> PERIOD | DESTINATION <verb> <tense> PERIOD | OBJECT <afterObject>
void afterNoun() {

	cout << "Processing <afterNoun>\n";

	switch (next_token())
	{
	case IS:
		be();
		match(PERIOD);
		break;

	case WAS:
		be();
		match(PERIOD);
		break;

	case DESTINATION:
		match(DESTINATION);
		verb();
		tense();
		match(PERIOD);
		break;

	case OBJECT:
		match(OBJECT);
		afterObject();
		break;

	default:
		syntaxerror2(saved_lexeme, "afterNoun");	//generate syntax error if unexpected lexeme
	}
}

// Grammar: <afterObject>::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> <tense> PERIOD
void afterObject() {

	cout << "Processing <afterObject>\n";

	switch (next_token())
	{
	case WORD2:
		verb();
		tense();
		match(PERIOD);
		break;

	case WORD1:
		noun();
		match(DESTINATION);
		verb();
		tense();
		match(PERIOD);
		break;

	case PRONOUN:
		noun();
		match(DESTINATION);
		verb();
		tense();
		match(PERIOD);
		break;

	default:
		syntaxerror2(saved_lexeme, "afterObject");	//generate syntax error if unexpected lexeme
	}
}

// Grammar: <noun>::= WORD1 | PRONOUN
void noun() {

	cout << "Processing <noun>\n" << endl;

	switch (next_token()) 
	{
	case WORD1:
		match(WORD1);
		break;

	case PRONOUN:
		match(PRONOUN);
		break;

	default:
		syntaxerror2(saved_lexeme, "noun");  //generate syntax error if unexpected lexeme
	}		                                                                                                             
}

// Grammar: <verb>::= WORD2
void verb() {

	cout << "Processing <verb>\n" << endl;

	if (next_token() == WORD2)  // look ahead at next token                                                                                                            
		match(WORD2);
	else
		syntaxerror2(saved_lexeme, "verb");  //generate syntax error if unexpected lexeme                
}

// Grammar: <be>::= IS | WAS
void be() {

	cout << "Processing <be>\n" << endl;

	switch (next_token())
	{
	case IS:
		match(IS);
		break;

	case WAS:
		match(WAS);
		break;

	default:
		syntaxerror2(saved_lexeme, "be");  //generate syntax error if unexpected lexeme
	}		   
}

// Grammar: <tense>::= VERBPAST | VERBPASTNEG | VERB | VERBNEG
void tense() {

	cout << "Processing <tense>\n";

	switch (next_token())
	{
	case VERBPAST:
		match(VERBPAST);
		break;

	case VERBPASTNEG:
		match(VERBPASTNEG);
		break;

	case VERB:
		match(VERB);
		break;

	case VERBNEG:
		match(VERBNEG);
		break;

	default:
		syntaxerror2(saved_lexeme, "tense");	//generate syntax error if unexpected lexeme
	}
}



//--------------------------- Driver ---------------------------
// The new test driver to start the parser
int main()
{
	string filename;
	cout << "Enter the input file name: ";
	cin >> filename;
	fin.open(filename.c_str());
	//fin.open("partCtest1");

	story();
	fin.close();

	//cin.get();	// *** FOR DEBUGGING - REMOVE BEFORE SUBMITTING ***
	//cin.get();
}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions
