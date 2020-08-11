#include <stdlib.h>
#include <iomanip>
#include <ios>
#include <map>
#include <iterator>
#include "scanner.cpp"

using namespace std;

string saved_lexeme;
string saved_E_word;
tokentype saved_token;
bool token_avail = false;

// open a new output file
ofstream fout("translated.txt", ios::out | ios::trunc);

// Lexicon - a dictionary map to hold the contents of lexicon.txt
map<string, string> lexicon;

void s();
void afterSubject();
void afterNoun();
void afterObject();
void noun();
void verb();
void be();
void tense();


// using the line type, sends a line of an IR to 
// translated.txt output file (saved_E_word or saved_token is used)
void gen(string s) {
	if (s == "TENSE")
		fout << setw(15) << s << "\t" << tokenName[saved_token] << endl;
	else
		fout << setw(15) << s << "\t" << saved_E_word << endl;
}

// Purpose: Uses a dictionary map to check if current saved_lexeme (Japanese word) 
//			is in the lexicon and has a translation (English word)
//	if in Lexicon - save the English word in saved_E_word
//	if not in Lexicon - save the Japanese word itself in saved_E_word
void getEword() {

	// check if saved_lexeme (Japanese word) found in lexicon
	map<string, string>::iterator it = lexicon.find(saved_lexeme);

	// Japanese word was found - store its value (English word) in saved_E_word
	if (it != lexicon.end())
		saved_E_word = it->second;

	// Japanese word not found so has no translation in lexicon
	// save Japanese word itself in saved_E_word
	else
		saved_E_word = saved_lexeme;
}

// Type of error: Token mismatch - next token doesn't match what grammar expects
void syntaxerror1(tokentype expected, string saved_lexeme) {
	cout << "SYNTAX ERROR: expected " << tokenName[expected] << " but found " << saved_lexeme << endl;
	cin.get();
	cin.get();
	exit(1);
}

// Type of error: Invalid token - default case of parser function reached
void syntaxerror2(string saved_lexeme, string parser_Function) {
	cout << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in " << parser_Function << endl;
	cin.get();
	cin.get();
	exit(1);
}

// Purpose: Call scanner to get next token 
// Returns: the token grabbed from scanner function
tokentype next_token() {

	// if no token already saved
	if (!token_avail) {
		scanner(saved_token, saved_lexeme);
		cout << "Scanner called using word: " << saved_lexeme << endl;
		cout << "Token returned from Scanner: " << saved_token << endl;
		token_avail = true;
	}
	return saved_token;
}

// Purpose: Checks to see if expected token is different from next token
// Returns: true if token matched syntax error if false
bool match(tokentype expected) {

	if (next_token() != expected) {		 // mismatch has occurred
		syntaxerror1(expected, saved_lexeme);// generate a syntax error
	}
	else {
		cout << "Matched " << tokenName[expected] << endl;
		token_avail = false;	// set to false to get new token
		return true;
	}
}


//  ------------------- RDP functions - one per non-term ------------------- //

// Grammar: <story>::= <s>{<s>}
void story() {
	cout << "Processing <story> ...\n\n";
	s();

	// keep calling s() until end of file marker is reached
	while (saved_lexeme != "eofm")
		s();
	cout << "\nSuccessfully parsed <story>.\n";
}


// Grammar: <s> ::= [CONNECTOR #getEword# #gen(“CONNECTOR”)#] |
//					<noun> #getEword# SUBJECT #gen(“ACTOR“)#  <after subject>
void s() {

	// get the next token
	next_token();

	// if not at end of file 
	if (saved_lexeme != "eofm") {

		cout << "Processing <s>\n";

		// grab next connector token here
		if (saved_token == CONNECTOR) {
			match(CONNECTOR);
			getEword();
			gen("CONNECTOR");
		}

		noun();
		match(SUBJECT);
		gen("ACTOR");
		afterSubject();
	}
}


// Grammar: <after subject> ::= <verb> #getEword# #gen(“ACTION“)# <tense> #gen(“TENSE“)# PERIOD |
//							    <noun> #getEword# <after noun>
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
		cout << "SAVED LEXEME IS: " << saved_lexeme << endl;
		syntaxerror2(saved_lexeme, "afterSubject"); //generate syntax error if unexpected lexeme
	}
}


// Grammar: <afterNoun>::= <be> #gen(“DESCRIPTION”)# #gen(“TENSE”)# PERIOD | 
						// DESTINATION #gen(“TO”)# <verb>#getEword# #gen(“ACTION“)# <tense> #gen(“TENSE“)# PERIOD |
						// OBJECT #gen(“OBJECT“)#  <after object>   
void afterNoun() {

	cout << "Processing <afterNoun>\n";

	switch (next_token())
	{
	case IS:
		gen("DESCRIPTION");
		be();
		match(PERIOD);
		break;

	case WAS:
		gen("DESCRIPTION");
		be();
		match(PERIOD);
		break;

	case DESTINATION:
		match(DESTINATION);
		gen("TO");
		verb();
		tense();
		match(PERIOD);
		break;

	case OBJECT:
		match(OBJECT);
		gen("OBJECT");
		afterObject();
		break;

	default:
		syntaxerror2(saved_lexeme, "afterNoun");//generate syntax error if unexpected lexeme
	}
}


// Grammar: <afterObject>::= <verb> #getEword# #gen(“ACTION“)# <tense> #gen(“TENSE“)# PERIOD | 
//							 <noun> #getEword# DESTINATION #gen(“TO”)#<verb> #getEword# #gen(“ACTION“)# <tense> #gen(“TENSE“)# PERIOD
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
		gen("TO");
		verb();
		tense();
		match(PERIOD);
		break;

	case PRONOUN:
		noun();
		match(DESTINATION);
		gen("TO");
		verb();
		tense();
		match(PERIOD);
		break;

	default:
		syntaxerror2(saved_lexeme, "afterObject");//generate syntax error if unexpected lexeme
	}
}


// Grammar: <noun>::= WORD1 | PRONOUN
void noun() {

	cout << "Processing <noun>\n" << endl;

	switch (next_token())
	{
	case WORD1:
		match(WORD1);
		getEword();
		break;

	case PRONOUN:
		match(PRONOUN);
		getEword();
		break;

	default:
		syntaxerror2(saved_lexeme, "noun");  //generate syntax error if unexpected lexeme
	}
}


// Grammar: <verb>::= WORD2
void verb() {

	cout << "Processing <verb>\n" << endl;

	// look ahead at next token  
	if (next_token() == WORD2) {
		match(WORD2);
		getEword();
		gen("ACTION");
	}
	//generate syntax error if unexpected lexeme       
	else
		syntaxerror2(saved_lexeme, "verb");
}


// Grammar: <be>::= IS | WAS
void be() {

	cout << "Processing <be>\n" << endl;

	switch (next_token())
	{
	case IS:
		match(IS);
		gen("TENSE");
		break;

	case WAS:
		match(WAS);
		gen("TENSE");
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
		gen("TENSE");
		break;

	case VERBPASTNEG:
		match(VERBPASTNEG);
		gen("TENSE");
		break;

	case VERB:
		match(VERB);
		gen("TENSE");
		break;

	case VERBNEG:
		match(VERBNEG);
		gen("TENSE");
		break;

	default:
		syntaxerror2(saved_lexeme, "tense");//generate syntax error if unexpected lexeme
	}
}


//  ------------------- Driver ------------------- //

// The final test driver to start the translator
int main()
{
	string Jword;
	string Eword;

	//open lexicon.txt file   
	fin.open("lexicon.txt");
	cout << "Opening the Lexicon File" << endl;

	// each line of lexicon.txt file contains one Japanese and one English word
	// lexicon is a map that has - key: Japanese word, value: English word  
	while (fin) {
		// gets Japanese word first then English word
		fin >> Jword;
		fin >> Eword;
		// read key-value pairs into map
		lexicon[Jword] = Eword;
	}
	//close lexicon.txt
	fin.close();

	//get filename from user and open the file
	string filename;
	cout << "Enter the input file name: ";
	cin >> filename;
	fin.open(filename.c_str());
	cout << "Opening " << filename << "...\n\n";

	//calls the <story> to start parsing
	story();

	//close the input and output files
	fin.close();
	fout.close();
	
}// end main