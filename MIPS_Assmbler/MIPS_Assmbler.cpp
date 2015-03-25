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

using namespace std;


union command
{
	struct 
	{
		unsigned func : 6;
		unsigned shamt : 5;
		unsigned rd : 5;
		unsigned rt : 5;
		unsigned rs : 5;
		unsigned opcode : 6;
	} rType;
	struct
	{
		unsigned imm : 16;
		unsigned rt : 5;
		unsigned rs : 5;
		unsigned opcode : 6;
	}iType;
	struct
	{
		unsigned address : 26;
		unsigned opcode : 6;
	}jType;
	unsigned int cmd;
};

int main()
{
	command c;
	c.rType.opcode = 0;
	c.rType.rs = 16;
	c.rType.rt = 0;
	c.rType.rd = 8;
	c.rType.shamt = 0;
	c.rType.func = 0x20;
	printf("%#010x", c);

	string tmp;
	string line;
	string file = "test.txt";
	FileIO* flIO = new FileIO();
	flIO->openFile(file);
	tmp = flIO->loadTxtFile();
//	cout << tmp;
//	flIO->writeTextFile("test2.txt", tmp);
	delete flIO;
	getline(cin, tmp);
	return 0;
}