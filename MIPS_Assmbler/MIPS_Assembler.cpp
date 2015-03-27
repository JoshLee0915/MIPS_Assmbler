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
	string file = "test.txt";
	FileIO* flIO = new FileIO();
	flIO->openFile(file);
	AssemblerMIPS test(flIO->loadTxtFile());
	cout << test.assembleCode();
	delete flIO;
	return 0;
}