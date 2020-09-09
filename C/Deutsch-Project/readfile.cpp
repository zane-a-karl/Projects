#include <fstream>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main() {

char* fileName ="map.txt";
char* x;

ifstream inFile;
inFile.open(fileName);

if (!inFile) {
	cerr << "Unable to open" << fileName <<"\n\n";
	exit(1);
	}

while (inFile >> x) {
	cout << x << endl;
}

inFile.close();
/*
use strcspn and strpbrk to find letters in file
first line after letter is propagation speed in km/sec
sectond line after letter is transmission speed in bytes/sec
all lines before next letter are nodes
start vertex | end vertex | distance between vertices in km
Tp = distance/propagation speed
Td = file size/transmission speed
*/
return 0;
}
