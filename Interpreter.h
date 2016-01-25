#include <vector>
#include <string>
#include <map>

using namespace std;

class Interpreter
{


public:

	Interpreter(vector <string> sourcecode);														//CONSTRUCTOR FOR INTERPRETER CLASS
	//TAKES A SOURCE CODE OF LINES

	~Interpreter();

	void NewLines(vector <string> & lines);

	void StoreTokens(vector <string> lines, Interpreter & aProgram);

	static	bool isRead(const string & token);														//TEST TO SEE IF READ LINE WAS AT
	//THE BEGINNING OF THE LINE

	static	bool isDisplay(const string & token);													//TEST TO SEE IF DISPLAY LINE WAS AT
	//THE BEGINNING OF THE LINE

	void PrintElement(vector <string>::iterator x);

	void addValue(const string & token, float val);													//THIS IS A METHOD TO ADD A VALUE TO THE
	//STORED MAP OBJ

	bool getValue(const string & token, float & val);												//THIS IS A METHOD TO TEST AND STORE THE 
	//VALUE IN A MAP IF IT IS THERE

	void executeProgram(vector <string> lines);																			//METHOD TO EXECUTE A CALL TO A PROGRAM OBJ
	//TO EVALUATE ELEMENTS IN IT

	void readElement(string & token);																//METHOD TO TO A STRING ELEMENT


	void evaluateVarExp(string & token, float & val);
	//METHOD TO EVALUATE A VARIABLE EXPRESSION

	vector <vector <string> > GetBracketExp(vector <string>::iterator x, int line);

	int HowManyNests(int i);


	bool isValidExp(vector <string> & token);
	//BY CALLING INFIX EVAL AND STORING VALUE

	vector <string> VecToString(vector <vector <string> > source);

	void clear();

private:
	map<string, float> ValuesMap;
	float val;
	vector <string> NumericalExp, WhileExp, Nests;
	vector <vector <string> > allTokens;
	string LastString;
	vector <string> sourcelines, currentline;
	vector<string>::iterator place, end;
	vector<string>::iterator endofblock;
	bool isvalid = true;
	bool inwhile, iselse;
};
