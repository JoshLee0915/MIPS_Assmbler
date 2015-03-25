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

int main()
{
	string tmp;
	string line;
	string file = "test.txt";
	FileIO* flIO = new FileIO();
	flIO->openFile(file);
	tmp = flIO->loadTxtFile();
	cout << tmp;
	flIO->writeTextFile("test2.txt", tmp);
	delete flIO;
	getline(cin, tmp);
	return 0;
}