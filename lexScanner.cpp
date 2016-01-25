#include <fstream>
#include <iostream>  // for basic iostream operations
#include <cctype>	// to check C character type using function like isdigit()
#include "lexScanner.h"
// You may include interface information of simpleScanner.cpp in "lexScanner.h", such as
// the coding of component categories into integers, and the function prototype of
// lexScanner.
// The caller of the lexical scanner module will aso include "lexScanner.h"

using namespace std;

const char DOUBLE_QUOTE = '\"';

//***************************************************************************
//*  Section 1. two functions:
//*  getPerLineTokenVectFromOneCharArray and
//*     getPerLineTokenVectFromOneStringObject
//*
//***************************************************************************
//*  They analyze one C-style character array or a string object and
//*  collect the tokens there as a vetor of token strings.
//***************************************************************************
void LexicalScanner::getPerLineTokenVectFromOneCharArray
(
const char* ptrOriginalStatement,
perLineTokenVector & tokenVector
)
// 1. One line of code is stored in a C-style character array: statement,
//	with a terminating '\0' NULL character to end the string stored there.
// 2. In the end, we'll store all the tokens, each as an object of the string class,
//	in tokenVector, which is a vector of strings.
{
	string statement;
	while ((*ptrOriginalStatement) != NULL)
	{
		if (isspace(*ptrOriginalStatement))
			//Convert every white space characters into a space ' '
			//    and append it to the end of statement;
			statement = statement + ' ';
		else
			//Append a character as it is to the end of statement;
			statement = statement + (*ptrOriginalStatement);
		++ptrOriginalStatement;
	}


	string tokenBuffer;
	tokenBuffer.reserve(200);
	//reserve an initial storage of 200 characters

	size_t iBuffIndex = 0; // index into the statement string

	//A while loop that normalizes the statement string
	//into an equaivalent string with one space between two adjacent tokens.
	while (iBuffIndex< statement.size())
		//While it is not the end of thecharacter string stored in the statement array
	{
		char c, nextChar;
		c = statement[iBuffIndex];
		if (iBuffIndex + 1 < statement.size())
			nextChar = statement[iBuffIndex + 1];
		else
			nextChar = ' ';

		if (c == DOUBLE_QUOTE)
			//if a double quote is encountered, search for the matching double quote
			//and skip the entire string literal like "Hello" enclosed in
			//the pair of double quotes.
		{
			tokenBuffer += DOUBLE_QUOTE;
			//Copy the starting double quote into token buffer

			iBuffIndex++;
			c = statement[iBuffIndex];
			while (c != NULL && c != DOUBLE_QUOTE)
				//Search for a matching double quote or a terminating NULL character.
				//Append all achracters in between into tokenBuffer
			{
				tokenBuffer += c;

				iBuffIndex++;
				c = statement[iBuffIndex];
			}
			tokenBuffer += DOUBLE_QUOTE;
			//Copy the endinging double quote into token buffer
		}
		else //The character examined is not a double quote.
			// In the following id statement,
			// we try to detect the end (last character) of a token in the statement array.
			// We insert a space appropriately into the components string to make sure
			// in the components string there is at least one space between two tokens.
			if (
				((isalnum(c) || c == '.') &&
				!(isalnum(nextChar) || nextChar == '.')
				)
				// If the current character c is alphnumerical character or a dot
				// (i.e. a letter ot a digit), but the next character is not.
				||
				((c == '=') && (nextChar != '='))
				// equal sign, but not followed by another equal sign
				||
				c == ',' || c == ':' || c == ';'
				// comma, colon, semicolon
				||
				c == '+' || c == '-' || c == '*' || c == '%' || c == '^'
				// numerical operators: +,-, *, %
				||
				((c == '/') && (nextChar != '/'))
				// numerical operator: / but not followed by /
				||
				((c == '>') && (nextChar != '='))
				// relational operator: > but not followed by =
				||
				((c == '<') && (nextChar != '='))
				// relational operator: < but not followed by =
				||
				((c == '&') && (nextChar != '&'))
				// end of logical operator: &&
				||
				((c == '|') && (nextChar != '|'))
				// end of logical operator: ||
				||
				((c == '!' && nextChar != '='))
				// end of logical operator: !
				||
				(c == '(' || c == ')')
				// parenthesis
				||
				(c == '{' || c == '}')
				// curly braces
				)
				//We detect c as the end of a token.
			{
				// Copy the character c from statement array i
				// and append it into the end of of the token components string.
				tokenBuffer += c;

				// If the next character is not a space,
				// append a space to the end of components to make sure
				// in components there is at least one space between two tokens.
				if (nextChar != ' ')
					tokenBuffer += ' ';
			}
			else
				// c is not the end of a token.
			{
				if (c != ' ')
					// c is not a space, but a part of a token while not the end of the token.
					// So just copy the character c from statement array i
					// and append it into the end of of the token components string.
					tokenBuffer += c;
				else
				{
					if (c == ' ' && nextChar != ' ')
						// c is a space, but the next character is not a space.
						// append a space to the end of components to make sure
						// in components there is at least one space between two tokens.
						tokenBuffer += ' ';
				}
			}

		if (c != NULL)
			//If the character we just finish examining is not the terminating NULL,
			//prepare to examine the next character on the next iteration.
			iBuffIndex++;
		//else we have reached a NULL character signaling the end of the statement.

		//Prepare to check the next character in the statement array.
	}
	//end of while loop that normalizes the statement in the character array
	//into an equaivalent string with one space between two adjacent tokens.


	size_t startIndex; // The index of the beginning of a token
	size_t endIndex;    // The index of the end of a token

	bool keepChecking;
	//Detect the first NON-SPACE character in the string.
	//Set startIndex as the index this character or
	//set it as the length of the string if the entire string is composed of spaces.
	for (startIndex = 0, keepChecking = true;
		startIndex < tokenBuffer.size() && keepChecking;
		)
		if (tokenBuffer[startIndex] != ' ')
			keepChecking = false;
		else
			startIndex++;

	tokenVector.clear();
	//cout << "tokenBuffer=[" <<tokenBuffer<<"]"<<endl;


	while (startIndex < tokenBuffer.size())
	{
		if (tokenBuffer[startIndex] == DOUBLE_QUOTE)
			//If the token is a string LITERAL like "YES" started with a double quote,
			//search for the matching double quote after the index startIndex.
			//Set endIndex as the index after that of the matching double quote.
		{
			for (endIndex = startIndex + 1, keepChecking = true;
				endIndex < tokenBuffer.size() && keepChecking;
				endIndex++
				)
				if (tokenBuffer[endIndex] == DOUBLE_QUOTE)
					keepChecking = false;
		}
		else
			//If the token is not started with a double quote,
			//Detect the first SPACE character after the index startIndex.
			//Set endIndex as the index this space character or
			//set it as the length of the string
			//if none such space exists after the index startIndex.
		{
			for (endIndex = startIndex + 1, keepChecking = true;
				endIndex < tokenBuffer.size() && keepChecking;
				)
				if (tokenBuffer[endIndex] == ' ')
					keepChecking = false;
				else
					endIndex++;
		}

		//tokenBuffer.substr(startIndex, endIndex - startIndex) is a token.
		//Append such a token in the token vector to tokenVector
		tokenVector.push_back(tokenBuffer.substr(startIndex, endIndex - startIndex));
		//cout << "Token found:" << tokenVector.back() << endl;

		//Detect the next non-space character with an index after current endIndex.
		//Set startIndex as the index this character or
		//set it as the length of the string if the reaminder of the string
		//is entirely composed of spaces.
		for (startIndex = endIndex, keepChecking = true;
			startIndex < tokenBuffer.size() && keepChecking;
			)
			if (tokenBuffer[startIndex] != ' ')
				keepChecking = false;
			else
				startIndex++;

	}

}


void LexicalScanner::getPerLineTokenVectFromOneStringObject
(const string statement,
perLineTokenVector & tokenVector
)
// 1. One line of code is stored in string object: statement.
// 2. In the end, we'll store all the tokens, each as an object of the string class,
//	in tokenVector, which is a vector of strings.
{

	getPerLineTokenVectFromOneCharArray(statement.c_str(), tokenVector);

}




//******************************************************************
//* Section 2:
//* The following is a colllection of functions to
//* test the categories of tokens
//******************************************************************

//Check whether a token is a numerical token
bool LexicalScanner::isNUMERICAL_LITERAL(const string & token)
{
	if (token == ".")
		return false;

	bool decimalPointSeenBefore = false;
	//no decimal points seen yet

	for (size_t i = 0; //index into the string
		i < token.size();
		i++)
	{
		char c;
		c = token[i];
		if (c != '.' && !isdigit(c))
			// not a digit, nor a demical point ==> Not Numerical
			return false;
		if (c == '.' && decimalPointSeenBefore)
			// demical point appears more than once ==> Not right
			return false; //Not Numerical
		if (c == '.' && !decimalPointSeenBefore)
			// Keep a record when decimal point is seen for the first time
			decimalPointSeenBefore = true;

	}

	return true;
}


bool LexicalScanner::isKEYWORD(const string & token)
// Test whether it is a keyword of BIOLA,
// like function, read, display, if, else, return, while
{
	if (token.size() < 2) //IF THE TOKEN IS SMALLER THAN "IF" IT CANNOT BE A KEYWORD
		return false;

	char c; //VARIABLE TO STORE THE INDEX OF THE STRING TOKEN
	string word; //CREATES A STRING TO COMPARE TO KEYWORDS
	word.reserve(200); //RESERVES STRING SPACE FOR SIZE OF TOKEN ASUMMING IT WILL BE LESS THAN 200

	for (size_t i = 0; //index into the string
		i < token.size();
		i++)
	{
		c = token[i];
		word.push_back(c); //ADDS TOKEN CHARACTER TO THE TEMPORARY WORD
	}

	if (word == "function" ||
		word == "read" ||
		word == "display" ||
		word == "if" ||
		word == "else" ||
		word == "return" ||
		word == "main" ||
		word == "void" ||
		word == "while")
	{
		return true;
	}

	return false;
}

bool LexicalScanner::isID_NAME(const string & token)
// Testwhether it is an identifier, i.e. names of variables, labels, and functions
{
	if (isKEYWORD(token)) //ID NAMES CANNOT BE THE SAME AS KEYWORDS
		return false;

	char c;
	for (size_t i = 0; //index into the string
		i < token.size();
		i++)
	{

		c = token[i];

		if (!isalpha(token[0])) //CHECK IF FIRST LETTER IS A NUMBER
			return false;

		if (!isalnum(c)) // CHECK IF CHARACTER IS NOT A ALPHANUMERICAL CHARACTER
			return false;

		if (isspace(c)) // CHECK IF CHARACTER IS A SPACE
			return false;

		if (ispunct(c)) // CHECK IF CHARACTER IS A PUNCTUATION MARK
			return false;

	}

	return true;
}


bool LexicalScanner::isASSIGNMENT_OP(const string & token)
// Test whether it is =, the assignment operator   	 
{
	if (token.size() > 1) //ASSIGNMENT OPERATOR CANNOT BE MORE THAN ONE CHARACTER
		return false;

	char c;
	for (size_t i = 0; //index into the string
		i < token.size();
		i++)
	{
		c = token[i];

		if (c == '=')
			return true;

	}
	return false;
}

bool LexicalScanner::isLOGICAL_OP(const string & token)
{

	char c;

	for (size_t i = 0; //index into the string
		i < token.size();
		i++)
	{
		bool isor = false;  // FLAG FOR CHECK IF OPERATOR HAS ALREADY BEEN SEEN
		bool isand = false;
		bool iseq = false;

		c = token[i];

		for (size_t i = 0; //index into the string
			i < token.size();
			i++)
		{
			if (c == '|' && isor)
				// IF C = '|' AND TOKEN BEFORE C = '|' --> IS RELATIONAL OP
				return true;

			if (c == '&' && isand)
				// IF C = '&' AND TOKEN BEFORE C = '&' --> IS RELATIONAL OP
				return true;

			if (c == '=' && iseq)
				// IF C = '=' AND TOKEN BEFORE C = '=' --> IS RELATIONAL OP
				return true;
		}

		if (c == '|')
			isor = true;

		if (c == '&')
			isand = true;

		if (c == '=')
			iseq = true;

		if (c == '(')
			return true;
		if (c == ')')
			return true;

		if (c == '<')
			return true;
		if (token == "<=")
			return true;
		if (c == '>')
			return true;
		if (token == ">=")
			return true;


	}

	return false;
}

bool LexicalScanner::isNUMERICAL_OP(const string & token)
// Test whether it is one of the following
// numerical operators     (such as +, -, *, /, %)
{
	if (token.size() > 1)
		return false;

	char c;
	for (size_t i = 0; //index into the string
		i < token.size();
		i++)
	{
		c = token[i];

		if (c == '+')
			return true;

		if (c == '-')
			return true;

		if (c == '*')
			return true;

		if (c == '%')
			return true;

		if (c == '/')
			return true;

		if (c == '^')
			return true;

	}

	return false;
}

bool LexicalScanner::isRELATIONAL_OP(const string & token)
// Test whether it is one of the following
// numerical operators     (such as >, ==, <, >=, <=)
{
	char c;
	for (size_t i = 0; //index into the string
		i < token.size();
		i++)
	{
		bool iseq = false; // FLAG FOR CHECKING IF '=' HAS BEEN SEEN BEFORE
		c = token[i];

		if (c == '=')
			iseq = true;

		if (iseq && c == '=')
			return true;

		if (iseq && c == '<')
			return true;

		if (iseq && c == '>')
			return true;

		if (iseq && c == '!')
			return true;

		if (c == '>')
			return true;

		if (c == '<')
			return true;

	}
	return false;
}


bool LexicalScanner::isSTRING_LITERAL(const string & token)
// Test whether it is a string literals
// (like "Hi Hi", in a pair of double quotes)
{
	int isqu = 0;
	char c;
	for (size_t i = 0; //index into the string
		i < token.size();
		i++)
	{
		c = token[i];

		if (c == '"')
			// KEEPS TRACK OF NUMBER OF TIMES '"' HAVE BEEN SEEN
			// IF TWICE THEN THATS THE END OF THE STRING LITERAL
			isqu++;

		if (isqu == 2)
			return true;
	}

	return false;
}

bool LexicalScanner::isCOLON(const string & token)
// Test whether it is a colon :
{
	if (token == ":")
		return true;
	else
		return false;
}

bool LexicalScanner::isSEMICOLON(const string & token)
// Test whether it is a semicolon ;
{
	if (token == ";")
		return true;
	else
		return false;
}

bool LexicalScanner::isCOMMA(const string & token)
// Test whether it is a comma ,
{
	if (token == ",")
		return true;
	else
		return false;

}

bool LexicalScanner::isLEFT_CURLYBRACE(const string & token)
// Test whether it is a left CURLYBRACE {
{
	if (token == "{")
		return true;
	else
		return false;
}

bool LexicalScanner::isRIGHT_CURLYBRACE(const string & token)
// Test whether it is a right CURLYBRACE }
{
	if (token == "}")
		return true;
	else
		return false;
}

bool LexicalScanner::isLEFT_PARENTHESIS(const string & token)
// Test whether it is a left parenthesis (
{
	if (token == "(")
		return true;
	else
		return false;
}

bool LexicalScanner::isRIGHT_PARENTHESIS(const string & token)
// Test whether it is a right parenthesis )
{
	if (token == ")")
		return true;
	else
		return false;
}

bool LexicalScanner::isCOMMENT(const string & token)
// Test whether it is comment //
{
	if (token == "//")
		return true;
	else
		return false;
}




//**************************************************************************
//Section 3: one function:
//getLexCategoryVectorOneLine and getLexCategoryVectorAllLines
//
//You should extend this function to categorize the tokens in the
//tokenVector and store the categories of the tokens in categoryVector
//by calling the categoriry testing functions described in
//the section 2 above.
//**************************************************************************
void LexicalScanner::getCategoryVectorFromTokenVector(
	const perLineTokenVector & tokenVector,
	perLineCategoryVector & categoryVector)
{
	for (size_t i = 0; //index into the string
		i < tokenVector.size();
		i++)
	{
		// CREATES A TEMPORARY STRING VAR TO HOLD TOKEN INFORMATION
		// THEN CHECKS IT WITH EACH FUNCTION
		string tempString = tokenVector[i];

		if (isKEYWORD(tempString)) {
			categoryVector.push_back(KEYWORD);
		}

		else if (isSTRING_LITERAL(tempString))
			categoryVector.push_back(STRING_LITERAL);

		else if (isID_NAME(tempString))
			categoryVector.push_back(ID_NAME);

		else if (isASSIGNMENT_OP(tempString))
			categoryVector.push_back(ASSIGNMENT_OP);

		else if (isNUMERICAL_OP(tempString))
			categoryVector.push_back(NUMERICAL_OP);

		else if (isNUMERICAL_LITERAL(tempString))
			categoryVector.push_back(NUMERICAL_LITERAL);

		else if (isLOGICAL_OP(tempString))
			categoryVector.push_back(LOGICAL_OP);

		else if (isRELATIONAL_OP(tempString))
			categoryVector.push_back(RELATIONAL_OP);

		else if (isCOLON(tempString))
			categoryVector.push_back(COLON);

		else if (isSEMICOLON(tempString))
			categoryVector.push_back(SEMICOLON);

		else if (isCOMMA(tempString))
			categoryVector.push_back(COMMA);

		else if (isLEFT_PARENTHESIS(tempString))
			categoryVector.push_back(LEFT_PARENTHESIS);

		else if (isRIGHT_PARENTHESIS(tempString))
			categoryVector.push_back(RIGHT_PARENTHESIS);

		else if (isLEFT_CURLYBRACE(tempString))
			categoryVector.push_back(LEFT_CURLYBRACE);

		else if (isRIGHT_CURLYBRACE(tempString))
			categoryVector.push_back(RIGHT_CURLYBRACE);

		else if (isCOMMENT(tempString))
			categoryVector.push_back(COMMENT);

		else
			categoryVector.push_back(UNKNOWN);
	}

}


//**************************************************************************
// Section 4: a core function for the lexical scanner module
//
// getLexicalInfo is used to
// to get two kinds of lexical information from
// all lines of a program, i.e.
// (1) the vector of token vectors of all lines and
// (2) the vector of category vectors of all lines.
//
// Implementation:
// You should implement them by appropriately using the following functions
//    getPerLineTokenVectFromOneStringObject (implemented in section 1 above), and
//    getCategoryVectorFromTokenVector (to be implemented in section 3 above)
//****************************************************************************
void LexicalScanner::getLexicalInfo(
	const vector<string> & linesOfStatements,
	vectOfTokenVects & ALLtokenVectors,
	vectOfCategoryVects & ALLcategoryVectors)
{
	ALLcategoryVectors.clear(); //CLEARS ALL VECTORS BEFORE STARTING NEW READ
	ALLtokenVectors.clear();
	perLineTokenVector tokenVect;
	perLineCategoryVector catVect;

	for (size_t i = 0; //index into the string
		i < linesOfStatements.size();
		i++)
	{
		tokenVect.clear(); //CLEARS ALL LINES OF TOKENS BEFORE STARTING NEW ITERATION
		catVect.clear();

		getPerLineTokenVectFromOneStringObject(linesOfStatements[i], tokenVect); //READS ENTIRE CODE AND STORES AS INDIVIDUAL TOKEN VECTORS
		ALLtokenVectors.push_back(tokenVect);

		getCategoryVectorFromTokenVector(tokenVect, catVect); //READS ENTIRE CODE OF TOKENS AND STORES CORRESPONDING CATEGORIES
		ALLcategoryVectors.push_back(catVect);
	}


}



//**************************************************************************
// Section 5: one function: displayLexicalInfo
//
// You should implement this function to display the lexical information
// strored in
// a vector of token vectors of all lines of statements, and
// a vector of category vectors of all lines of statements.
//****************************************************************************
void LexicalScanner::displayLexicalInfo(
	vectOfTokenVects & tokenVectors,
	vectOfCategoryVects & categoryVectors)
{
	if (tokenVectors.size() != categoryVectors.size())
	{
		cout << "# of token vectors inconsistent with # of category vectors." << endl;
		return;
	}

	for (size_t i = 0; i<tokenVectors.size(); i++)
	{
		perLineTokenVector tokenVect;
		perLineCategoryVector categoryVect;

		tokenVect = tokenVectors[i];
		categoryVect = categoryVectors[i];

		if (tokenVect.size() != categoryVect.size())
		{
			cout << "Line " << i
				<< ": # of tokens inconsistent with # of category labels." << endl;
			return;
		}

		cout << endl
			<< "***************************** "
			<< "Lexical Info of Line " << i
			<< "***************************** "
			<< endl;

		for (size_t j = 0; j<tokenVect.size(); j++)
		{
			string categoryInfo;
			switch (categoryVect[j])
			{
			case KEYWORD:
				categoryInfo = "KEYWORD";
				break;
			case ASSIGNMENT_OP:
				categoryInfo = "ASSIGNEMT_OP";
				break;
			case COLON:
				categoryInfo = "COLON";
				break;
			case COMMA:
				categoryInfo = "COMMA";
				break;
			case COMMENT:
				categoryInfo = "COMMENT";
				break;
			case ID_NAME:
				categoryInfo = "ID_NAME";
				break;
			case LEFT_CURLYBRACE:
				categoryInfo = "LEFT_CURLYBRACE";
				break;
			case LEFT_PARENTHESIS:
				categoryInfo = "LEFT_PARENTHESIS";
				break;
			case LOGICAL_OP:
				categoryInfo = "LOGICAL_OP";
				break;
			case RIGHT_CURLYBRACE:
				categoryInfo = "RIGHT_CURLYBRACE";
				break;
			case RIGHT_PARENTHESIS:
				categoryInfo = "RIGHT_PARENTHESIS";
				break;
			case NUMERICAL_LITERAL:
				categoryInfo = "NUMERICAL_LITERAL";
				break;
			case NUMERICAL_OP:
				categoryInfo = "NUMERICAL_OP";
				break;
			case RELATIONAL_OP:
				categoryInfo = "RELATIONAL_OP";
				break;
			case SEMICOLON:
				categoryInfo = "SEMICOLON";
				break;
			case STRING_LITERAL:
				categoryInfo = "STRING_LITERAL";
				break;
			default:
				categoryInfo = "UNKNOWN";
			}

			cout << "Token["
				<< j
				<< "]:"
				<< tokenVect[j]
				<< "\t\t"
				<< categoryInfo
				<< endl;
		}//end for

	}

}


void LexicalScanner::LoadProgram(vector<string> &lines, string fileName)
//THIS METHOD ALLOWS USER TO ENTER OTHER SOURCE CODE FOR THE SAKE OF TESTING
{
	// this char array stores the name of the file the program reads from.
	if (fileName == "")
	{

		cout << "Name of file to upload program from (ex. file.txt): ";
		cin >> fileName;
		while (fileName.size() == 0)
		{
			cin >> fileName;
		}
	}

	ifstream inFile;
	inFile.open(fileName);
	if (inFile.fail())
	{
		cout << endl << "Cannot open this file!" << endl;
		return;
	}

	lines.clear();  //Delete all the lines of statements
	string inputBuffer;
	while (!inFile.eof())
	{
		getline(inFile, inputBuffer);
		while (!inFile.eof() && inputBuffer.size() == 0)
		{
			getline(inFile, inputBuffer);
		}

		if (!inFile.eof() + 1)
		{
			cout << "["
				<< lines.size()
				<< "] read:"
				<< inputBuffer << endl;
			lines.push_back(inputBuffer);
		}

	}

	inFile.close();
}
