// RelationCProj.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "Matrix.h"

using namespace std;

int main(void)
{
	CMatrix matrix;

	bool bRunning = true;
	char key[10] = {0};

	while (bRunning)
	{
		cout << "===============Relation operation===============" << endl;
		cout << ">>1. Generate matrix" << endl;
		cout << ">>2. Calculate the equivalence enclosure" <<endl;
		cout << ">>3. Calculate the equivalence relation" <<endl;
		cout << ">>4. Generate simplified matrix (Hasse)" <<endl;
		cout << ">>5. Check for minimum partial order set (poset)" <<endl;
		cout << ">>0. Exit\n" <<endl;

		cout << ">>Please enter the command number (0-5) : ";
		cin >> key;

		cout << endl;

		switch (key[0])
		{
		case '1' :
			matrix.Init();
			matrix.ParseMatrix();
			break;
		case '2' :
			matrix.CalculateEquivalenceClosure();
			break;
		case '3' :
			matrix.CalculateEquivalenceRelation();
			break;
		case '4' :
			matrix.CalculateSimpleMat();
			break;
		case '5' :
			matrix.CheckPoset();
			break;
		case '0' :
			bRunning = false;
			cout << ">>>Goodbye" << endl;
			break;
		default :
			cout << ">>>Please enter a proper command" << endl;
			break;
		}
		cout << endl;
	}
	return 0;
}