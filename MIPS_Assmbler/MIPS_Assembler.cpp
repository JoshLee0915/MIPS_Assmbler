/** @file MIPS_Assembler.cpp
 * A program to assemble and display MIPS code
 *
 * @author  Josh Lee
 * @date 3/22/2015
 **********************************************************/

#include <iostream>
#include <string>
#include <stdlib.h>
#include "FileIO.h"
#include "AssemblerMIPS.h"

using namespace std;

int main(int argc, char* args[])
{
	string line = "y";
	string file = "";
	string asmFile = "";
	FileIO* flIO = new FileIO();
	AssemblerMIPS* assembler;

	// check for a command line arg
	if ( argc >= 2 )
		file = args[1];		// get the passed file
	// prompt for the file
	else
	{
		cout << "Please enter a valid file path > ";
		getline(cin, file);
	}

	// start assembling
	while(true)
	{
		// open the file
		if(flIO->openFile(file))
		{
			// read the file 
			assembler = new AssemblerMIPS(flIO->loadTxtFile());
			// close the file
			flIO->closeFile();

			// assemble the file
			asmFile = assembler->assembleCode();

			// get the file name and create a new name for the saved file
			file.erase(file.find_last_of('.'));
			file += "_Out.txt";

			// clean up and display the code and write the file
			delete assembler;
			flIO->writeTextFile(file,asmFile);
			cout << asmFile << "\nSaved to: " << file << "\n";
		}
		else
			cout << flIO->getLastError();

		cout << "\nWould you like to assemble another file?(y/n) > ";
		cin >> line;
		if(line[0] == 'y' || line[0] == 'Y')
		{
			cout << "Please enter a valid file path > ";
			getline(cin, file);
			getline(cin, file);
		}
		else
			break;	// end
	}

	cout << "Closing...\n";

	delete flIO;
	return 0;
}