//cont/map1.cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include "lexScanner.h"
#include "expEvaluator.h"
using namespace std;


//*********************************************************************************
//*********************************************************************************
int ExpressionEvaluator::precedenceLevel(const string & opToken)
//return the precedence level of an operator token
{
	if (opToken == "||")
		return 0;
	if (opToken == "&&")
		return 1;

	if (opToken == "!=")
		return 2;
	if (opToken == "<")
		return 2;
	if (opToken == "==")
		return 2;
	if (opToken == ">")
		return 2;
	if (opToken == ">=")
		return 2;
	if (opToken == "<=")
		return 2;

	if (opToken == "+")
		return 3;
	if (opToken == "-")
		return 3;

	if (opToken == "*")
		return 4;
	if (opToken == "/")
		return 4;
	if (opToken == "%")
		return 4;

	if (opToken == "^")
		return 5;
	if (opToken == "!")
		return 5;
	if (opToken == "(")
		return 6;
	if (opToken == ")")
		return 6;

	//Otherwise
	cout << "unknown op:" << opToken << ". precedence level?? " << endl;
	return -1;

}


//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::postfixEvaluator
(const expVector & postfixExp,
float & expValue
)
//Evaluate the value of a postfix expression postfixExp
//with respect to the variables and their current values  in varTable.
//finally store the value of the expresson in expValue.
//If postfixExp is not a valid postfix expression, return false; otherwsie return true.
{
	vector<float> stack;
	for (expVector::const_iterator pos = postfixExp.begin();
		pos != postfixExp.end();
		pos++
		)
	{
		string token;

		token = *pos;
		if (token.size()< 1)
		{
			expValue = 0;
			cout << "Empty Token, not valid." << endl;
			return false;
		}

		if (LexicalScanner::isNUMERICAL_LITERAL(token))
			//A numerical constant, convert it into a float value, and
			//store push it onto the stack
			stack.push_back(atof(token.c_str()));
		else if (LexicalScanner::isID_NAME(token))
			//An ID_NAME, try to look up its current value from the table.
			//If succeeds, push the value onto the stack.
		{
			stack.push_back(atof(token.c_str()));
		}
		else if (LexicalScanner::isNUMERICAL_OP(token) ||
			LexicalScanner::isRELATIONAL_OP(token) ||
			LexicalScanner::isLOGICAL_OP(token)
			)
			//A numerical or relational operator
			//Try to pop two operands from the stack apply the operator.
			//If succeeds, push te result back to the stack.
		{
			float op1, op2;

			//Try to pop op2 first
			if (!stack.empty())
			{
				op2 = stack.back();
				stack.pop_back();
			}
			else
			{
				cout << "No matching op2 for the op: " << token << endl;
				return false; //op2 not available, Not a valid expression
			}


			//Determine whether this is a bianry operator or a unary operator
			bool isUnaryOP, isBinaryOP;
			if (token == "!")    //unary logical negation operator
			{
				isUnaryOP = true; isBinaryOP = false;
			}
			else
			{
				isUnaryOP = false; isBinaryOP = true;
			}

			if (isUnaryOP) //When the operator is a unary operator
			{    //the only case at this moment: the logical negation operator !
				if (token == "!")
					stack.push_back(!op2);
				else
				{
					cout << "Unknown unary op: " << token << endl;
					return false;
				}
			} //end the processing of a unary operator

			if (isBinaryOP)  //When the operator is a binary operator
			{
				if (stack.empty())
					return false; //op1 not available, Not a valid expression

				op1 = stack.back();
				stack.pop_back();

				//Apply the binary operator and push back the result to the stack
				if (token == "+")
					stack.push_back(op1 + op2);
				else if (token == "-")
					stack.push_back(op1 - op2);
				else if (token == "*")
					stack.push_back(op1*op2);
				else if (token == "/")
					stack.push_back(op1 / op2);
				else if (token == "%")
					stack.push_back(int(op1) % int(op2));
				else if (token == "^")
					stack.push_back(pow(op1,op2));

				else if (token == "<")
					stack.push_back(op1<op2);
				else if (token == "==")
					stack.push_back(op1 == op2);
				else if (token == ">")
					stack.push_back(op1>op2);
				else if (token == ">=")
					stack.push_back(op1 >= op2);
				else if (token == "<=")
					stack.push_back(op1 <= op2);

				else if (token == "&&")
					stack.push_back(op1&&op2);
				else if (token == "||")
					stack.push_back(op1 || op2);
				else
				{
					cout << "Unkown binary op: " << token << endl;
					return false;
				}
			} //end the processing of a binary operator

		}//end the processing of the case that the token is an operator

		else
		{
			cout << "Unkown token type (Not an op or operand): " << token << endl;
			return false;
		}
		// Every token in the expression must be either be a numerical constant,
		// an ID_NAME or an operator; Otherwise it is NOT a valid expression.

	}//end for


	if (stack.size() == 1)
		//The evaluated value of the expression should be the only value left on the stack
	{
		expValue = stack.back();
		return true;
	}
	else
		//Other wise it is not a valis expression.
		return false;

}


//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::infixToPostfixConversion
(const expVector & infixExp, expVector & postfixExp)
//Convert the infix expression in infixExp into a corresponding postfix expression.
//Store the corresponding postfix expression in postfixExp.
//If infixExp is not a valid infix expression, return false; otherwsie return true.
{
	vector <string> thestack;
	for (expVector::const_iterator pos = infixExp.begin();
		pos != infixExp.end()-1; pos++)
	{

		if (infixExp.size() < 2)
			return false;
		string posnext = *(pos + 1);

		if (*pos == "(")
		{
			if (!LexicalScanner::isNUMERICAL_LITERAL(posnext) && !LexicalScanner::isID_NAME(posnext))
			{
				cout << endl <<
					"****************************" << endl <<
					"NOT A VALID INFIX EXPRESSION" << endl <<
					"****************************" << endl << endl;
				return false;
			}
		}
		else if (LexicalScanner::isNUMERICAL_LITERAL(*pos))
		{
			if (!LexicalScanner::isLOGICAL_OP(posnext) &&								//IF THE TOKEN IS A OPERATOR
				!LexicalScanner::isRELATIONAL_OP(posnext) &&
				!LexicalScanner::isNUMERICAL_OP(posnext) && 
				posnext != ")")
			{
				cout << endl <<
					"****************************" << endl <<
					"NOT A VALID INFIX EXPRESSION" << endl <<
					"****************************" << endl << endl;
				return false;
			}
		}
		else if (*pos == ")")
		{
			if (!LexicalScanner::isLOGICAL_OP(posnext) &&								//IF THE TOKEN IS A OPERATOR
				!LexicalScanner::isRELATIONAL_OP(posnext) &&
				!LexicalScanner::isNUMERICAL_OP(posnext) &&
				*pos != "(")
			{
				cout << endl <<
					"****************************" << endl <<
					"NOT A VALID INFIX EXPRESSION" << endl <<
					"****************************" << endl << endl;
				return false;
			}
		}
	}
		
	
	for (expVector::const_iterator pos = infixExp.begin();							//SCAN THE INFIX STRING FROM LEFT
		pos != infixExp.end(); pos++)												//TO RIGHT
	{

		string token;
		token = *pos;

		if (LexicalScanner::isNUMERICAL_LITERAL(token) ||
			LexicalScanner::isID_NAME(token))								//IF THE TOKEN IS A OPERAND ADD IT TO THE POSTFIX
		{
			postfixExp.push_back(token);
		}

		else if (token == "(")														//IF THE TOKEN IS A (
		{
			thestack.push_back(token);
		}

		else if (token == ")")														//IF THE TOKEN IS )
		{
			for (int i = 0; i < thestack.size()+1; i++)
			{
				string stackpos = thestack.back();
				if (stackpos == "(")
				{
					thestack.pop_back();
					{break; }
				}
					if (stackpos != "(")
					{
						postfixExp.push_back(stackpos);
						thestack.pop_back();
					}
			}
		}

		else if (LexicalScanner::isLOGICAL_OP(token) ||								//IF THE TOKEN IS A OPERATOR
			LexicalScanner::isRELATIONAL_OP(token) ||
			LexicalScanner::isNUMERICAL_OP(token))
		{
			if (thestack.size() == 0)
			{
				thestack.push_back(token);
			}

			else
			{
				while (ExpressionEvaluator::precedenceLevel(thestack.back()) >=
					ExpressionEvaluator::precedenceLevel(token) &&
					!thestack.empty())
				{
					if (thestack.back() == "(")
					{
						break;
					}
					postfixExp.push_back(thestack.back());
					thestack.pop_back();
					if (thestack.size() == 0)
						break;
					
				}

				thestack.push_back(token);	
			}
		}
	}

	while (!thestack.empty())																	//
	{
		if (thestack.back() != "(")
		postfixExp.push_back(thestack.back());
		thestack.pop_back();
	}


	return true;
}



//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::infixEvaluator
(const expVector & infixExp,
float& expValue
)
//Evaluate the value of the infix expression in postfixExp
//with respect to the variables and their current values in varTable.
//finally store the value of the expresson in expValue.
//If postfixExp is not a valid postfix expression, return false; otherwsie return true.
{
	expVector postfixExp;
	infixToPostfixConversion(infixExp, postfixExp);
	return postfixEvaluator(postfixExp, expValue);
}



//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::infixEvaluator
(expVector::iterator begin,
expVector::iterator end, float &expValue
)
//Evaluate the value of an infix expression
//that is embedded within an expression vector
//The two expVector::iterator iterators: begin and end specify
//where the infix expression is embedded.
//
//Evaluate the value of an infix expression
//with respect to the variables and their current values in varTable.
//finally store the value of the expresson in expValue.
//If postfixExp is not a valid postfix expression, return false; otherwsie return true.
{
	expVector infixExp(begin, end);
	return infixEvaluator(infixExp, expValue);
}





// simpleScanner.cpp gives some some hints on the implementation of your lexical scanner
// function. For example, it shows how you can break a statementinto basic
// components (tokens) and determine their categories.
