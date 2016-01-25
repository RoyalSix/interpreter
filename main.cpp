#include <string>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include "lexScanner.h"
#include "expEvaluator.h"
#include "Interpreter.h"

using namespace std;

int main()
{
	string buffer = " ";
	string fileName;
	vector <string> myLinesOfStatements;
	Interpreter aProgram(myLinesOfStatements);
	Interpreter bProgram(myLinesOfStatements);
	Interpreter cProgram(myLinesOfStatements);

	char z, x;

	bool in = true;	// while the user is using the program this var is set to true and the menu keeps looping.
	// when the user wants to quit, it is set to false and the loop ends.

	char keyPressed, keyPressed2;	// This var stores which menu selection the user picks.

	while (in)
	{
		cout << endl << endl << endl;
		cout << "****************************************************" << endl;
		cout << "**  MENU:(press a character to select an option)  **" << endl;
		cout << "****************************************************" << endl;
		cout << "Q. [QUIT] Quit." << endl;
		cout << endl;
		cout << "T. [TYPE A PRPGRAM] TYPE A PROGRAM TO TEST" << endl;
		cout << endl;
		cout << "A. [STANDARD TEST] TEST PROGRAM WITH STANDARD FILE" << endl;
		cout << endl;
		cout << "L. [LOAD A PROGRAM] READ A PROGRAM FROM FILE " << endl;
		cout << endl;

		cout << "****************************************************" << endl << endl;

		cout << "Your choice is: ";
		cin >> keyPressed;



		switch (keyPressed) {

		case 'Q': case 'q'://Quit
			cout << "[QUIT]:" << endl;
			in = false;
			break;

		case 'L': case 'l':
			myLinesOfStatements.clear();
			LexicalScanner::LoadProgram(myLinesOfStatements);
			cout << endl << endl << "Executing Program..." << endl;
			cProgram.executeProgram(myLinesOfStatements);
			_getch();
			cin.clear();
			aProgram.clear();
			break;

		case 'T': case 't':
			//Ask the user to type in another source program for analysis
			buffer.clear();
			cout << endl
				<< "********************************************************************"
				<< endl;
			cout << endl << "Enter a program (as lines of statements), one line at a time,"
				<< endl << "with a line of single . to signal the end of the program input."
				<< endl;

			myLinesOfStatements.clear();
			while (buffer != ".")
			{
				getline(cin, buffer);
				if (buffer.size() > 0 && buffer != ".")
					myLinesOfStatements.push_back(buffer);
			}

			for (size_t i = 0; i < myLinesOfStatements.size(); i++)
			{
				cout << "Line["
					<< i
					<< "].\t"
					<< myLinesOfStatements[i]
					<< endl;
			}

			cout << endl << "Executing the program..." << endl << endl;
			//Get the lexical information 
			cout << endl << endl << "Executing Program..." << endl;
			bProgram.executeProgram(myLinesOfStatements);
			_getch();
			cin.clear();
			bProgram.clear();
			break;
		case 'A': case 'a':
			myLinesOfStatements.clear();

			cout << endl << endl << endl;
			cout << "****************************************************" << endl;
			cout << "*************  STANDRD TEST OPTION  ****************" << endl;
			cout << "****************************************************" << endl;
			cout << "1 - Standard Test 1" << endl;
			cout << endl;
			cout << "2 - Standard Test 2" << endl;
			cout << endl;
			cout << "3 - Standard Test 3" << endl;
			cout << endl;
			cout << "4 - Standard Test 4" << endl;
			cout << endl;
			cout << "5 - Standard Test 5 Programming 4B" << endl;
			cout << endl;
			cout << "6 - Standard Test 6 Programming 4B" << endl;
			cout << endl;
			cout << "****************************************************" << endl << endl;
			cin >> keyPressed2;

			switch (keyPressed2) {
			case '1':
				fileName = "1.txt";
				LexicalScanner::LoadProgram(myLinesOfStatements, (fileName));
				cout << endl << endl << "Executing Program..." << endl;
				cProgram.executeProgram(myLinesOfStatements);
				_getch();
				cin.clear();
				cProgram.clear();
				break;
			case '2':
				fileName = { "2.txt" };
				LexicalScanner::LoadProgram(myLinesOfStatements, (fileName));
				cout << endl << endl << "Executing Program..." << endl;
				cProgram.executeProgram(myLinesOfStatements);
				_getch();
				cin.clear();
				cProgram.clear();
				break;
			case '3':
				fileName = { "3.txt" };
				LexicalScanner::LoadProgram(myLinesOfStatements, (fileName));
				cout << endl << endl << "Executing Program..." << endl;
				cProgram.executeProgram(myLinesOfStatements);
				_getch();
				cin.clear();
				cProgram.clear();
				break;
			case '4':
				fileName = { "4.txt" };
				LexicalScanner::LoadProgram(myLinesOfStatements, (fileName));
				cout << endl << endl << "Executing Program..." << endl;
				cProgram.executeProgram(myLinesOfStatements);
				_getch();
				cin.clear();
				cProgram.clear();
				break;
			case '5':
				fileName = { "5.txt" };
				LexicalScanner::LoadProgram(myLinesOfStatements, (fileName));
				cout << endl << endl << "Executing Program..." << endl;
				cProgram.executeProgram(myLinesOfStatements);
				_getch();
				cin.clear();
				cProgram.clear();
				break;
			case '6':
				fileName = { "6.txt" };
				LexicalScanner::LoadProgram(myLinesOfStatements, (fileName));
				cout << endl << endl << "Executing Program..." << endl;
				cProgram.executeProgram(myLinesOfStatements);
				_getch();
				cin.clear();
				cProgram.clear();
				break;
			}
		}
	}

	return 0;
}
