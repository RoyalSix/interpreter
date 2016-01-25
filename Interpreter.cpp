#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "Interpreter.h"
#include "expEvaluator.h"
#include "lexScanner.h"
#include <typeinfo>

using namespace std;

Interpreter::Interpreter(vector <string> sourcecode)
{
	sourcelines = sourcecode;
	Interpreter::StoreTokens(sourcelines, *this);
}

Interpreter::~Interpreter()
{
	Interpreter::clear();
}

void Interpreter::NewLines(vector <string> &lines)
{
	Interpreter::StoreTokens(lines, *this);
}

bool Interpreter::getValue(const string & token, float & val)
{
	map<string, float>::iterator pos;
	pos = ValuesMap.find(token);

	if (LexicalScanner::isNUMERICAL_LITERAL(token))
	{
		val = stof((token).c_str());
		return true;
	}

	if (pos != ValuesMap.end())
	{

		val = pos->second;
		return true;
	}

	else
		return false;
}

void Interpreter::addValue(const string & token, float val)
{
	if (this->ValuesMap[token] != val)
		this->ValuesMap[token] = val;
	this->ValuesMap.insert(make_pair(token, val));
}

bool Interpreter::isRead(const string & token)
{
	if (token == "read")
		return true;
	else
		return false;
}

void Interpreter::readElement(string & token)
{
	float val;
	cin >> val;
	Interpreter::addValue(token, val);
}

bool Interpreter::isDisplay(const string & token)
{

	if (token == "display")
	{
		return true;
	}
	else
		return false;
}

void Interpreter::evaluateVarExp(string & token, float & val)
{
	{

		if (LexicalScanner::isNUMERICAL_OP(token))
		{
			WhileExp.push_back(token);
			this->NumericalExp.push_back(token);
		}

		if (LexicalScanner::isRELATIONAL_OP(token))
		{
			WhileExp.push_back(token);
			this->NumericalExp.push_back(token);
		}

		else if (this->getValue(token, val))
		{
			WhileExp.push_back(token);
			this->NumericalExp.push_back(to_string(val));
		}

		if (place == end-1)
		{
			if (ExpressionEvaluator::infixEvaluator(this->NumericalExp, val))
			{
				NumericalExp.clear();
				return;
			}
			else
			{
				cout << endl << "Variable \"" << token << "\" not declared." << endl;
				isvalid = false;
				return;
			}
		}
	}
}

bool Interpreter::isValidExp(vector <string> & token)
{
	if (LexicalScanner::isNUMERICAL_LITERAL(token[0]))
		return false;

	auto x = find(token.begin(), token.end(), "=");

	if (Interpreter::isDisplay(token[0]))
		return true;

	else if (Interpreter::isRead(token[0]))
		return true;

	else if (x != token.end())
		return true;

	else if (LexicalScanner::isKEYWORD(token[0]))
		return true;

	else if (token[0] == "{")
		return true;

	else if (token[0] == "}")
	{
		return true;
	}

	else if (LexicalScanner::isID_NAME(token[0]))
		return true;

	else
	{
		cout << "Syntax error, check expression." << endl;
		return false;
	}
}

void Interpreter::StoreTokens(vector <string> sourcecode, Interpreter & aProgram)
{
	vector <string> rightbracket = { "}" };
	vector <string> leftbracket = { "{" };
	expVector source, Postsource;
	perLineCategoryVector categorys;

	for (size_t i = 0; //index into the string
		i < sourcecode.size();
		i++)
	{

		LexicalScanner::getPerLineTokenVectFromOneStringObject
			(sourcecode[i],
			source
			);

		LexicalScanner::getCategoryVectorFromTokenVector(source, categorys);

		if (!source.empty())
		{

			if (Interpreter::isValidExp(source))
			{
				allTokens.push_back(source);
				if (source == leftbracket)
				{
					Nests.push_back("{");
				}
				else if (source == rightbracket)
				{
					Nests.push_back("}");
				}
				else
				{
					Nests.push_back("0");
				}

			}
			else
			{
				cout << endl << "Invalid expression, check syntax.";
				isvalid = false;
				break;
			}
		}

	}
}


void Interpreter::clear()
{
	Nests.clear();
	val = 0;
	ValuesMap.clear();
	NumericalExp.clear();
	allTokens.clear();
	LastString.clear();
	sourcelines.clear();
	isvalid = true;
}

void Interpreter::PrintElement(vector <string>::iterator x)
{
	string line = *x;
	float aval;

	if (LexicalScanner::isID_NAME(line))
	{
		if (this->getValue(line, aval))
		{
			cout << aval;
		}
		else
		{
			cout << endl << "Variable \"" << line << "\" not declared." << endl;
			isvalid = false;
		}
		return;
	}
	line.pop_back();
	reverse(line.begin(), line.end());
	line.pop_back();
	reverse(line.begin(), line.end());

	if (line == "")
	{
		cout << endl;
	}
	else
	{
		cout << line;
	}

}

int Interpreter::HowManyNests(int z)
{
	int counter = 0;
	while (Nests[z] != "}")
	{
		if (Nests[z] != "0")
		counter++;
		z++;
	}
	
	return counter;
}

vector <vector <string> > Interpreter::GetBracketExp(vector <string>::iterator x, int line)
{
	int nestedbrackets;
	vector <vector <string> >::iterator endbracket;
	vector <vector <string> > bracketexp;
	vector <string> lastbracket = { "}" };

	vector <vector <string> >::iterator beginbracket = allTokens.begin() + line;

	nestedbrackets = Interpreter::HowManyNests(line-1);
	auto beginbracketcopy = beginbracket;

	for (int i = 0; i < nestedbrackets; i++)
	{
		endbracket = find(beginbracket, allTokens.end(), lastbracket);
		beginbracket = endbracket+1;
	}

	for_each(beginbracketcopy, endbracket, [&bracketexp](vector <string> z){bracketexp.push_back(z); });
	endofblock = (*endbracket).end()-1;

	return bracketexp;
}

vector <string> Interpreter::VecToString(vector <vector <string> > source)
{
	vector <string> newline;
	string z;

	for (int i = 0; i < source.size(); i++)
	{
		for (int j = 0; j < source[i].size(); j++)
		{
			z = z + " " + source[i][j];
			if (j == source[i].size() - 1)
			{
				newline.push_back(z);
				z.clear();
			}
		}
	}

	return newline;
}


void Interpreter::executeProgram(vector <string> lines)
{
	if (this->allTokens.empty())
		this->NewLines(lines);

	for (size_t i = 0; //index into the string
		i < allTokens.size();
		i++)
	{
		for (int j = 0; //index into the string
			j < allTokens[i].size() && isvalid;
			j++)
		{
			currentline = allTokens[i];
			place = allTokens[i].begin() + j;
			end = allTokens[i].end()-1;

			if (LexicalScanner::isID_NAME(*place))
			{
				LastString = *place;
			}

			if (Interpreter::isDisplay(*place))
			{
				place++;

				while (place != end)
				{
					if (LexicalScanner::isSTRING_LITERAL(*place))
					{
						Interpreter::PrintElement(place);
					}

					else if (LexicalScanner::isID_NAME(*place) || LexicalScanner::isNUMERICAL_LITERAL(*place) || LexicalScanner::isNUMERICAL_OP(*place))
					{
						if ((LexicalScanner::isNUMERICAL_OP(*(place + 1)) || LexicalScanner::isNUMERICAL_OP(*(place - 1)) || LexicalScanner::isNUMERICAL_OP(*place)))
						{
							Interpreter::evaluateVarExp(*place, val);
							if (place == end - 1)
							cout << val << endl;
						}
						else 
							Interpreter::PrintElement(place);
					}
					place++;
				}

				cout << endl;
			}

			else if(Interpreter::isRead(*place))
			{
				Interpreter::readElement(*(place+1));
			}

			else if(LexicalScanner::isASSIGNMENT_OP(*place))
			{ 
				while (place != end-1 && isvalid)
				{
					place++;
					Interpreter::evaluateVarExp(*place, val);
				}
				Interpreter::addValue(LastString, val);
				j = currentline.size();
			}

			if (*place == "while")
			{
				int counter = 0;
				float aval;
				place += 2;
				vector <vector <string> > & bracketexp = Interpreter::GetBracketExp(place, i + 2);
				while (place != (end) && isvalid)
				{
					Interpreter::evaluateVarExp(*place, aval);
					counter++;
					place++;
				}
				if (aval)
				{
					inwhile = true;
					auto newproglines = Interpreter::VecToString(bracketexp);
					Interpreter WhileProg(newproglines);
					WhileProg.ValuesMap = this->ValuesMap;
					while (inwhile)
					{
						WhileProg.executeProgram(newproglines);
						this->ValuesMap = WhileProg.ValuesMap;

						place = place - counter;
						while (place != (end) && isvalid)
						{
							Interpreter::evaluateVarExp(*place, aval);
							place++;
						}
						if (!aval)
							inwhile = false;
					}
				}
				i += bracketexp.size() + 3;
				j = -1;
				NumericalExp.clear();

			}

			else if (*place == "if")
			{
				float aval;
				place += 2;
				vector <vector <string> > & bracketexp = Interpreter::GetBracketExp(place, i + 2);
				while (place != (end) && isvalid)
				{
					Interpreter::evaluateVarExp(*place, aval);
					place++;
				}

				if (!aval)
				{
					iselse = true;
					i += bracketexp.size() + 3;
					j = -1;
					if (i >= allTokens.size())
						return;
				}

				if (aval)
				{
					auto newproglines = Interpreter::VecToString(bracketexp);
					Interpreter InIf(newproglines);
					InIf.ValuesMap = this->ValuesMap;
					InIf.executeProgram(newproglines);
					this->ValuesMap = InIf.ValuesMap;
					NumericalExp.clear();
					iselse = false;
					i += bracketexp.size() + 3;
					j = -1;
				}
				
				else if (*allTokens[i].begin() == "else" && iselse)
				{
					vector <vector <string> > & bracketexp = Interpreter::GetBracketExp(place, i + 2);
					auto newproglines = Interpreter::VecToString(bracketexp);
					Interpreter InIf(newproglines);
					InIf.ValuesMap = this->ValuesMap;
					InIf.executeProgram(newproglines);
					this->ValuesMap = InIf.ValuesMap;
					NumericalExp.clear();

					i += bracketexp.size() + 3;
					j = -1;
				}
				
			}

			else if (*place == "else")
			{
				vector <vector <string> > & bracketexp = Interpreter::GetBracketExp(place, i + 2);
				i += bracketexp.size() + 3;
				j = -1;
				
			}

			if (i == allTokens.size() && j == -1)
				return;
		}
		
	}

}
