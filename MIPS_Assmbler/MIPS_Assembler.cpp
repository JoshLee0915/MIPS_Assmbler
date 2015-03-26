/** @file DVD_StoreDriver.cpp
 * An inventory program for a dvd store
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

int main()
{
	command c;
	c.rType.opcode = 0;
	c.rType.rs = 16;
	c.rType.rt = 0;
	c.rType.rd = 8;
	c.rType.shamt = 0;
	c.rType.func = 0x20;
	printf("%#010x\n", c);

	string tmp;
	string line;
	string file = "test.txt";
	FileIO* flIO = new FileIO();
	flIO->openFile(file);
	tmp = flIO->loadTxtFile();
	AssemblerMIPS test(tmp);
	cout << tmp << "\n\n";
	cout << test.assembleCode();
	flIO->writeTextFile("test2.txt", tmp);
	delete flIO;
	getline(cin, tmp);
	return 0;
}