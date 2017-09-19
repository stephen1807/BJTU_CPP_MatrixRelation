#include "stdafx.h"
#include "Matrix.h"

#include <iostream>
#include <fstream>

using namespace std;

#define MATRIX_FILE_NAME  "test2.txt"

CMatrix::CMatrix(void)
{
}

CMatrix::~CMatrix(void)
{
}

void CMatrix::Init()
{
	//Reset number of elements
	m_nElemNum = 0;

	//Reset relation matrix
	for (int i = 0; i < MAX_ELEMENT_NUMBER; i++)
	{
		for (int j = 0; j < MAX_ELEMENT_NUMBER; j++)
		{
			//Disconnect all elements
			m_aRelations[i][j] = NULL;
		}
	}
}

//Parse matrix from file and save it in a 2D array
void CMatrix::ParseMatrix(void)
{
	//Open file stream
	FILE *pMatrix;

	fopen_s(&pMatrix, MATRIX_FILE_NAME, "r");

	if (!pMatrix)
	{
		cout << ">>>Error in opening " << MATRIX_FILE_NAME << endl;
		return;
	}

	//Get matrix size
	fscanf_s(pMatrix, "%d\n", &m_nElemNum, 1);

	int value = 0;

	for (int i = 0; i < m_nElemNum; i++)
	{
		for (int j = 0; j < m_nElemNum; j++)
		{
			fscanf_s (pMatrix, "%d\n", &value, 1);

			m_aRelations[i][j] = IntToBool(value);
		}
	}
	cout << ">Successfully parse matrix content" << endl;
	cout << ">Matrix size : " << m_nElemNum << endl;

	fclose (pMatrix);
	pMatrix = NULL;
}

//Calculate equivalence closure
void CMatrix::CalculateEquivalenceClosure(void)
{
	bool m_Result[MAX_ELEMENT_NUMBER][MAX_ELEMENT_NUMBER];

	for (int i = 0; i < MAX_ELEMENT_NUMBER; i++)
	{
		for (int j = 0; j < MAX_ELEMENT_NUMBER; j++)
		{
			//Disconnect all elements
			m_Result[i][j] = NULL;
		}
	}

	//Copy content of matrix from the original
	for (int i = 0; i < m_nElemNum; i++)
	{
		for (int j = 0; j < m_nElemNum; j++)
		{
			m_Result[i][j] = m_aRelations[i][j];
		}
	}

	//////Calculate equivalence closure

	//Create reflexive closure
	for (int i = 0; i < m_nElemNum; i++)
	{
		m_Result[i][i] = true;
	}

	//Create symmetric closure
	for (int i = 0; i < m_nElemNum; i++)
	{
		for (int j = 0; j < m_nElemNum; j++)
		{
			if (m_Result[i][j] = true)
				m_Result[j][i] = true;
		}
	}

	//Create transitive closure using Warshall's algorithm
	for(int k = 0; k < m_nElemNum; k++)
	{
	    for(int i = 0; i < m_nElemNum; i++)
		{
			for(int j = 0; j < m_nElemNum; j++)
			{
				m_Result[i][j] = m_Result[i][j] || (m_Result[i][k] && m_Result[k][j]);
			}
	    }
	}

	//////Output to file

	// We need to change it to string variable first.
	string originalName = MATRIX_FILE_NAME;
	string eqc = "Eqc-"; // Name that we want to append
	// add original name after EqC-xxx
	eqc.append(originalName);

	// (File_name, Append mode)
	ofstream resultOut(eqc, ios::app);

	if(!resultOut.is_open()){ // Check if the file is open or not.
		cout << "Opening file failed! Please check the filename again." << endl; 
	}else{		
		for (int i = 0; i < m_nElemNum; i++)
		{
			for (int j = 0; j < m_nElemNum; j++)
			{
				resultOut << BoolToInt(m_Result[i][j]) << " ";
			}
			resultOut << endl;
		}
		cout << "Write to file successful" << endl;
	}
	resultOut.close();
}

//Create equivalence relation
void CMatrix::CalculateEquivalenceRelation(void)
{
	//Store the partitions
	int m_cEquiRelation[MAX_ELEMENT_NUMBER][MAX_ELEMENT_NUMBER];

	//Initiate values
	for (int i = 0; i < MAX_ELEMENT_NUMBER; i++)
	{
		for (int j = 0; j < MAX_ELEMENT_NUMBER; j++)
		{
			m_cEquiRelation[i][j] = -1;
		}
	}

	//Selected flag
	bool m_bSelected[MAX_ELEMENT_NUMBER][MAX_ELEMENT_NUMBER];

	//Initiate values
	for (int i = 0; i < MAX_ELEMENT_NUMBER; i++)
	{
		for (int j = 0; j < MAX_ELEMENT_NUMBER; j++)
		{
			m_bSelected[i][j] = false;
		}
	}

	//Calculating equivalence relation
	for (int i = 0; i < m_nElemNum; i++)
	{
		for (int j = 0; j < m_nElemNum; j++)
		{
			//If there is connection between elements
			if (m_aRelations[i][j] && !m_bSelected[i][j])
			{				
				m_bSelected[i][j] = true;
				m_bSelected[j][i] = true;

				int s = (i > j) ? j : i;
				int t = (i > j) ? i : j;

				//Adding to the equivalence relation 2D array
				for (int k = 0; k < m_nElemNum; k++)
				{
					if (m_cEquiRelation[s][k] == s)
						break;
					else if (m_cEquiRelation[s][k] == -1)
					{
						m_cEquiRelation[s][k] = s;
						break;
					}
				}

				for (int k = 0; k < m_nElemNum; k++)
				{
					if (m_cEquiRelation[s][k] == t)
						break;
					else if (m_cEquiRelation[s][k] == -1)
					{
						m_cEquiRelation[s][k] = t;
						break;
					}
				}

			}
		}
	}

	// Format : {{partition 1},{partition 2}, ...}

	//////Output to file

	// We need to change it to string variable first.
	string originalName = MATRIX_FILE_NAME;
	string eqc = "EqR-"; // Name that we want to append
	// add original name after EqR-xxx
	eqc.append(originalName);

	// (File_name, Append mode)
	ofstream resultOut(eqc, ios::app);

	if(!resultOut.is_open())
	{ // Check if the file is open or not.
		cout << "Opening file failed! Please check the filename again." << endl; 
	}
	else
	{
		int i;
		resultOut << "{";
		for (i = 0; i < m_nElemNum; i++)
		{
			int j;
			resultOut << "{";
			for (j = 0; j < m_nElemNum; j++)
			{
				if (m_cEquiRelation[i][j] != -1)
				{
					resultOut << m_cEquiRelation[i][j];
					if (j != m_nElemNum - 1)
						resultOut << ",";
				}

			}
			if (j == m_nElemNum)
			{
				resultOut << "}";
				if (i != m_nElemNum)
				{
					resultOut << ",";
				}
			}
		}
		resultOut << "}";
		cout << "Write to file successful" << endl;
	}
	resultOut.close();

}

//Calculate simplified matrix for Hasse diagram
void CMatrix::CalculateSimpleMat()
{
	bool m_Result[MAX_ELEMENT_NUMBER][MAX_ELEMENT_NUMBER];

	for (int i = 0; i < MAX_ELEMENT_NUMBER; i++)
	{
		for (int j = 0; j < MAX_ELEMENT_NUMBER; j++)
		{
			//Disconnect all elements
			m_Result[i][j] = NULL;
		}
	}

	//Copy content of matrix from the original
	for (int i = 0; i < m_nElemNum; i++)
	{
		for (int j = 0; j < m_nElemNum; j++)
		{
			m_Result[i][j] = m_aRelations[i][j];
		}
	}

	/////Hasse algorithm

	//Deleting loops
	for (int i = 0; i < m_nElemNum; i++)
	{
		m_Result[i][i] = false;
	}

	//Deleting edges with transitive properties
	for(int i = 0; i < m_nElemNum; i++)
	{
	    for(int j = 0; j < m_nElemNum; j++)
		{
			for(int k = 0; k < m_nElemNum; k++)
			{
				if (m_Result[i][j] && m_Result[j][k])
				{
					m_Result[i][k] = false;
				}
			}
	    }
	}

	//Output format : matrix

	// We need to change it to string variable first.
	string originalName = MATRIX_FILE_NAME;
	string eqc = "SpM-"; // Name that we want to append
	// add original name after SpM-xxx
	eqc.append(originalName);

	// (File_name, Append mode)
	ofstream resultOut(eqc, ios::app);

	if(!resultOut.is_open()){ // Check if the file is open or not.
		cout << "Opening file failed! Please check the filename again." << endl; 
	}else{		
		for (int i = 0; i < m_nElemNum; i++)
		{
			for (int j = 0; j < m_nElemNum; j++)
			{
				resultOut << BoolToInt(m_Result[i][j]) << " ";
			}
			resultOut << endl;
		}
		cout << "Write to file successful" << endl;
	}
	resultOut.close();

}

//Check for poset
void CMatrix::CheckPoset()
{
	bool flag;

	bool reflexive = true;

	//Check for relexive
	for (int i = 0; i < m_nElemNum; i++)
	{
		if(!m_aRelations[i][i])
			reflexive = false;
	}

	bool asymmetry = true;

	//Check for antisymmetry
	for (int i = 0; i < m_nElemNum; i++)
	{
		for (int j = 0; j < m_nElemNum; j++)
		{
			if ((i != j) && (m_aRelations[i][j] && m_aRelations[j][i]))
				asymmetry = false;
		}
	}

	bool transitive = false;

	//Check for transitive
	for(int i = 0; i < m_nElemNum; i++)
	{
	    for(int j = 0; j < m_nElemNum; j++)
		{
			for(int k = 0; k < m_nElemNum; k++)
			{
				if (m_aRelations[i][j] && m_aRelations[j][k] && m_aRelations[i][k])
					transitive = true;
			}
	    }
	}

	flag = reflexive && asymmetry && transitive;


	//Output flag

	//////Output to file

	// We need to change it to string variable first.
	string originalName = MATRIX_FILE_NAME;
	string eqc = "CfP-"; // Name that we want to append
	// add original name after CfP-xxx
	eqc.append(originalName);

	// (File_name, Append mode)
	ofstream resultOut(eqc, ios::app);

	if(!resultOut.is_open())
	{ // Check if the file is open or not.
		cout << "Opening file failed! Please check the filename again." << endl; 
	}
	else
	{
		if (flag)
			resultOut << "true";
		else
			resultOut << "false";
		cout << "Write to file successful" << endl;
	}
	resultOut.close();
}

//Convert bool to integer
int CMatrix::BoolToInt(bool boolean)
{
	if (boolean)
		return 1;
	else
		return 0;
}

//Convert integer to bool
bool CMatrix::IntToBool(int i)
{
	if(i == 1)
		return true;
	else
		return false;
}