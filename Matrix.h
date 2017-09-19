#pragma once
#define MAX_ELEMENT_NUMBER 20

struct Element 
{
	//Element ID
	char t_ElementID;

	//Index of element
	int i_Index;

};

struct Connection
{
	//Index of source element
	int source;
	
	//Index of destination element
	int destination;
};

class CMatrix
{
private :
	//Relation matrix between elements
	bool m_aRelations[MAX_ELEMENT_NUMBER][MAX_ELEMENT_NUMBER];

	//Array of list of elements
	Element m_aElements[MAX_ELEMENT_NUMBER];

	//Number of elements or maximum size of matrix
	int m_nElemNum;

public:
	CMatrix(void);
	~CMatrix(void);

	//Initialize relation matrix
	void Init(void);

	//Parse relation matrix from file
	void ParseMatrix(void);

	//Calculate equivalence closure
	void CalculateEquivalenceClosure(void);
	
	//Insert element
	bool InsertElement (Element sElement);

	//Create equivalence relation
	void CalculateEquivalenceRelation(void);

	//Calculate simplified matrix for Hasse diagram
	void CalculateSimpleMat(void);

	//Check for partial order
	void CheckPoset(void);

private :

	//Convert integer to boolean
	bool IntToBool (int i);

	//Convert boolean to integr
	int BoolToInt (bool boolean);
};