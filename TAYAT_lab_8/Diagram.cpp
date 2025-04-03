#include "Diagram.h"

Diagram::Diagram(Scaner* scaner, Tree* tree)
{
	this->scaner = scaner;
	this->tree = tree;
}

int Diagram::LookForward(int pos)
{
	type_lex lex;
	int saved_pointer = scaner->GetUK();
	int next_type;
	for (int i = 0; i < pos; i++)
		next_type = scaner->UseScaner(lex);
	scaner->PutUK(saved_pointer);
	return next_type;
}

int Diagram::Scan(type_lex lex)
{
	return scaner->UseScaner(lex);
}

void Diagram::Program()
{
	type_lex lex;
	int type;
	int simbol;
	int simbol2;

	type = LookForward(1);

	while (type == typeInt || type == typeShort || type == typeLong || type == typeFloat || type == typeConst || type == typeVoid)
	{
		type = LookForward(1);
		simbol = LookForward(3);
		simbol2 = LookForward(4);
		if (type == typeConst)
		{
			Named_const();
		}
		if ((type == typeInt || type == typeShort || type == typeLong || type == typeFloat) && simbol != typeLeftBracket && simbol2 != typeLeftBracket)
		{
			Data();
		}
		if ((type == typeInt || type == typeShort || type == typeLong || type == typeFloat || type == typeConst || type == typeVoid) && (simbol == typeLeftBracket || simbol2 == typeLeftBracket))
		{
			Function();
		}
		type = LookForward(1);
	}

	if (type != typeEnd)
	{
		type = Scan(lex);
		scaner->PrintError("������� ������ � ��������� Program, �������� ����� ���������, ", lex);
	}

}

void Diagram::Data()
{
	type_lex lex;
	Node* newNode = new Node();
	int type;
	int type2;
	int checkVarType = Type();

	do {
		type = LookForward(1);

		
		Tree* varNode = NULL;
		type_data typeData = tree->GetDataType(checkVarType);
		

		if (type != typeId)
		{
			type = Scan(lex);
			scaner->PrintError("������� ������ � ��������� Data, �������� ������������� ����������, ", lex);
		}

	
		int pointer = scaner->GetUK();
		type = Scan(lex);

		if (tree->IsDoublicateId(tree, lex)) {
			tree->PrintError("��������������� ", lex);
		}

		newNode->id = lex;  // ������������� �������������
		newNode->objectType = OBJ_VAR;
		newNode->dataType = tree->GetDataType(checkVarType);

		type = LookForward(1);
		if (type == typeEval) {
			newNode->flagInit = 1;
			type = Scan(lex); // ��������� '='
			int assignedType = LookForward(1);

			if (checkVarType != assignedType) {
				scaner->PrintError("������: ��� �������� �� ������������� ���� ���������� ", lex);
			}
		}
		else
		{
			newNode->flagInit = 0;
		}


		// ��������� ���� � ����� ���������
		tree->SetLeft(newNode);

		// ������� � ������ ��������� ���� ��� ���������� ��������
		tree = tree->GetLeft();

		scaner->PutUK(pointer);

		type2 = LookForward(2);
		if (type2 == typeEval)
		{
			Assignment();
			type = Scan(lex);
		}
		else
		{
			type = Scan(lex);
			type = Scan(lex);
		}

	} while (type == typeComma);

	if (type != typeSemicolon)
	{
		scaner->PrintError("������� ������ � ��������� Data, �������� ������ ';', ", lex);
	}
	
}

void Diagram::Named_const()
{
	type_lex lex;
	int type, checkConstType;
	Node* newNode = new Node();

	type = LookForward(1);
	if (type != typeConst)
	{
		type = Scan(lex);
		scaner->PrintError("������� ������ � ��������� Named_const, ��������� �������� ����� 'const', ", lex);
	}

	type = Scan(lex);

	//��������� ��� �������� ��������� ������������ �� ����
	checkConstType = TypeConst();
	type = LookForward(3);
	if (checkConstType != type)
	{
		scaner->PrintError("������� ������ � ��������� Named_const, �������� �� ������������ ���� ����������� ��������� ", lex);
	}

	do {
		type = LookForward(1);
		
		Tree* varNode = NULL;
		type_data typeData = tree->GetDataType(checkConstType);
		

		int pointer = scaner->GetUK();
		type = Scan(lex);

		if (tree->IsDoublicateId(tree, lex)) {
			tree->PrintError("��������������� ", lex);
		}

		newNode->id = lex;  // ������������� �������������
		newNode->dataType = tree->GetDataType(checkConstType);
		newNode->objectType = OBJ_NAMED;

		type = LookForward(1);
		if (type == typeEval) {
			newNode->flagInit = 1;
		}
		else
		{
			newNode->flagInit = 0;
		}

		// ��������� ���� � ����� ���������
		newNode->objectType = OBJ_NAMED;
		tree->SetLeft(newNode);

		// ������� � ������ ��������� ���� ��� ���������� ��������
		tree = tree->GetLeft();

		scaner->PutUK(pointer);

		Assignment();
		type = Scan(lex);
	} while (type == typeComma);

	if (type != typeSemicolon)
	{
		scaner->PrintError("������� ������ � ��������� Named_const, �������� ������ ';', ", lex);
	}
}

void Diagram::Function()
{
	type_lex lex;
	int type;

	type = LookForward(1);
	if (type != typeVoid)
	{
		type = Scan(lex);
		scaner->PrintError("������� ������ � ���� �������, �������� ��� ������ (void), ", lex);
	}
	type = Scan(lex);


	type = Scan(lex);
	if (type != typeId && type != typeMain)
	{
		scaner->PrintError("������� ������ � ��������� Function, �������� ������������� ������� ��� �������� ����� 'main', ", lex);
	}

	
	// �������� �� ����������
	if (tree->IsDoublicateId(tree, lex)) {
		scaner->PrintError("���������������, ", lex);
	}

	// ������ ����� ���� ��� �������
	Node* newNode = new Node();
	newNode->id = lex;  // ��������� �������������
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = tree->GetDataType(type);

	// ��������� ���� � ������
	tree->SetLeft(newNode);
	tree = tree->GetLeft(); // ������� � ������ ����
	tree->SetRight(NULL);

	Tree* tmpTree = tree; // ��������� ������� ��������� ������
	tree = tree->GetRight(); // ������� � ������� ���������


	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("������� ������ � ��������� Function, �������� ������ '(', ", lex);
	}

	//������������ ��������� �������, ���� ��� ����
	type = LookForward(1);
	if (type == typeInt || type == typeShort || type == typeLong || type == typeFloat)
	{
		ParameterList();
	}

	type = Scan(lex);
	if (type != typeRightBracket)
	{
		scaner->PrintError("������� ������ � ��������� Function, �������� ������ ')', ", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBrace)
	{
		scaner->PrintError("������� ������ � ��������� Function, �������� ������ '{', ", lex);
	}

	type = LookForward(1);

	while (type != typeRightBrace)
	{
		OperatorsAndDescriptions();
		type = LookForward(1);
	}

	// ������������ � ����������� ����
	tree = tmpTree;

	type = Scan(lex);
	if (type != typeRightBrace)
	{
		scaner->PrintError("������� ������ � ��������� Function, �������� ������ '}', ", lex);
	}
}


void Diagram::OperatorsAndDescriptions()
{
	int type = LookForward(1);

	if (type == typeConst)
	{
		Named_const();
	}
	else if (type == typeInt || type == typeShort || type == typeLong || type == typeFloat)
	{
		Data();
	}
	else Operator();
}

int Diagram::Type()
{
	type_lex lex;
	int type;
	int type2;
	int type3;
	type = LookForward(1);
	if (type != typeInt && type != typeShort && type != typeLong && type != typeFloat)
	{
		type = Scan(lex);
		scaner->PrintError("������� ������ � ��������� Type, �������� ��� ������ (int, short, long, float), ", lex);
	}
	type = Scan(lex);

	if (type == typeInt)
	{
		return typeConstInt;
	}

	if (type == typeShort)
	{
		return typeConstInt;
	}

	if (type == typeLong)
	{
		return typeConstInt;
	}

	if (type == typeFloat)
	{
		return typeConstFloat;
	}
}

int Diagram::TypeConst()
{
	type_lex lex;
	int type;
	int type2;
	int type3;
	type = LookForward(1);
	if (type != typeInt && type != typeChar && type != typeFloat)
	{
		type = Scan(lex);
		scaner->PrintError("������� ������ � ��������� TypeConst, �������� ��� ������ ��������� (int, char, float), ", lex);
	}
	type = Scan(lex);

	if (type == typeInt)
	{

		return typeConstInt;
	}

	if (type == typeChar)
	{

		return typeConstChar;
	}

	if (type == typeFloat)
	{

		return typeConstFloat;
	}

}

void Diagram::Assignment()
{
	type_lex lex;
	int type;

	type = Scan(lex);

	Tree* node = tree->FindUp(lex);

	if (node == NULL) {
		scaner->PrintError("������������� ������. ID �� ������, ", lex);
	}
	if (node->GetSelfObjectType() == OBJ_NAMED_ASSIGNED)
	{
		scaner->PrintError("������������� ������. ������� ���������������� �������� ����������� ���������, ", lex);
	}
	if (node->GetSelfObjectType() != OBJ_VAR && node->GetSelfObjectType() != OBJ_NAMED) {
		scaner->PrintError("������������� ������. ������� ������������� �� ���������� � ������������, ", lex);
	}
	if (node->GetSelfObjectType() == OBJ_NAMED)
	{
		node->changeObjType(OBJ_NAMED_ASSIGNED);
	}
	node->SetInit();

	// �������� ��� ���������� ����
	type_data varType = node->GetSelfDataType();
	

	type = Scan(lex);
	if (type != typeEval)
	{
		scaner->PrintError("������� ������ � ��������� Assignment, �������� ������ '=', ", lex);
	}

	Expression();
}

void Diagram::CompositeOperator()
{
	type_lex lex;
	int type;

	type = Scan(lex);
	if (type != typeLeftBrace)
	{
		scaner->PrintError("������� ������ � ��������� CompositeOperator, �������� ������ '{', ", lex);
	}

	// ������ ����� ���� ��� �������
	Node* newNode = new Node();
	newNode->id = lex;  // ��������� �������������
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = tree->GetDataType(type);

	// ��������� ���� � ������
	tree->SetLeft(newNode);
	tree = tree->GetLeft(); // ������� � ������ ����
	tree->SetRight(NULL);

	Tree* tmpTree = tree; // ��������� ������� ��������� ������
	tree = tree->GetRight(); // ������� � ������� ���������
	//////

	type = LookForward(1);

	while (type != typeRightBrace)
	{
		OperatorsAndDescriptions();
		type = LookForward(1);
	}

	type = Scan(lex);
	if (type != typeRightBrace)
	{
		scaner->PrintError("������� ������ � ��������� CompositeOperator, �������� ������ '}', ", lex);
	}

	tree = tmpTree;
}

void Diagram::Operator()
{
	type_lex lex;
	int type = LookForward(1);

	if (type == typeReturn)
	{
		type = Scan(lex);

		Expression();

		type = Scan(lex);
		if (type != typeSemicolon)
		{
			scaner->PrintError("������� ������ � ��������� Operator, �������� ������ ';' ����� return <���������>, ", lex);
		}
		return;
	}

	if (type == typeSemicolon) // ������ ��������
	{
		type = Scan(lex);
		return;
	}

	if (type == typeLeftBrace)
	{
		CompositeOperator();
		return;
	}

	if (type == typeSwitch)
	{
		Switch_operator();
		return;
	}

	int type2 = LookForward(2);
	if (type == typeId && type2 == typeLeftBracket)
	{
		FunctionCall();
		type = Scan(lex);
		if (type != typeSemicolon)
		{
			scaner->PrintError("������� ������ � ��������� Operator, �������� ������ ';', ", lex);
		}
		return;
	}

	if (type == typeId && type2 == typeEval)
	{
		Assignment();
		type = Scan(lex);
		if (type != typeSemicolon)
			scaner->PrintError("������� ������ � ��������� Operator, �������� ������ ';', ", lex);
		return;
	}

	type = Scan(lex);
	scaner->PrintError("������� ������ � ��������� Operator, �������� ��������, ", lex);
}

void Diagram::For_operator()
{
	type_lex lex;
	int type;

	Node* newNode = new Node();
	Tree* tmpTree = tree;

	type = Scan(lex);

	// ������ ����� ���� ��� �������
	newNode->id = lex;  // ��������� �������������
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = tree->GetDataType(type);

	// ��������� ���� � ������
	tree->SetLeft(newNode);
	tree = tree->GetLeft(); // ������� � ������ ����
	tree->SetRight(NULL);

	tmpTree = tree; // ��������� ������� ��������� ������
	tree = tree->GetRight(); // ������� � ������� ���������
	

	if (type != typeFor)
	{
		scaner->PrintError("������� ������ � ��������� For_operator, ��������� �������� ����� 'for', ", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("������� ������ � ��������� For_operator, �������� ������ '(', ", lex);
	}

	type = LookForward(1);


	if (type == typeInt || type == typeShort || type == typeLong)
	{
		Data();
	}
	else if (type != typeSemicolon)
	{
		Assignment();
		type = Scan(lex);
		if (type != typeSemicolon)
		{
			scaner->PrintError("������� ������ � ��������� For_operator, �������� ������ ';', ", lex);
		}
	}
	else
	{
		type = Scan(lex);
		if (type != typeSemicolon)
		{
			scaner->PrintError("������� ������ � ��������� For_operator, �������� ������ ';', ", lex);
		}
	}

	type = LookForward(1);
	if (type != typeSemicolon)
	{
		Expression();
	}
	type = Scan(lex);
	if (type != typeSemicolon)
	{
		scaner->PrintError("������� ������ � ��������� For_operator, �������� ������ ';', ", lex);
	}

	type = LookForward(1);
	if (type != typeRightBracket)
	{
		Assignment();
	}

	type = Scan(lex);
	if (type != typeRightBracket)
	{
		scaner->PrintError("������� ������ � ��������� For_operator, �������� ������ ')', ", lex);
	}

	OperatorsAndDescriptions();

	// ������������ � ����������� ����
	tree = tmpTree;

}

void Diagram::FunctionCall()
{
	Tree* proverka;
	type_lex lex;
	int type, next, exit;

	type = Scan(lex);
	if (type != typeId)
	{
		scaner->PrintError("������� ������ � ��������� FunctionCall, �������� ������������� �������, ", lex);
	}
	
	// �������� �� ����������
	// ���� ����, ������������ ��������
	Tree* objectNode = tree->FindUp(lex);
	// �������� ��������� �� ������� �� ������� � ��� �� ������ � ������������ ����
	Tree* methodNode = objectNode->FindFunction(lex); // ������� ��� ������ �� ����� ������� � ������������ ���� �� findUp
	if (methodNode == NULL) {
		scaner->PrintError("������� �� �������, ", lex);
		return; // ����� �� ������� ��� ������
	}

	// ���������, ��� ���� �������� ��������
	if (methodNode->GetSelfObjectType() != OBJ_FUNC) {
		scaner->PrintError("�� �������� ��������, ", lex);
	}
	/////////

	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("������� ������ � ��������� FunctionCall, �������� ������ '(', ", lex);
	}

	//������������ ��������� �������, ���� ��� ����
	type = LookForward(1);
	next = -1;
	exit = -1;
	if (type == typeId || type == typeConstInt || type == typeConstFloat)
	{
		//������������ ������ ����������
		do {
			//������������ �������� ��������� (������������� ��� ���������)
			if (type == typeConstInt || type == typeConstFloat)
			{
				type = Scan(lex);
			}
			else
			{
				if (type == typeId)
				{
					type = Scan(lex);
					proverka = tree->FindUp(lex);
					
					if (proverka == nullptr)
					{
						scaner->PrintError("������������� ������. ���������� �� �������, ", lex);
					}
					else
					{
						if (!proverka->IsSelfInit())
						{
							scaner->PrintError("������������� ������. ���������� �� ����������������, ", lex);
						}
					}
				}
				else
				{
					scaner->PrintError("������� ������ � ��������� FunctionCall, �������� ������������� ��� ���������, ", lex);
				}
			}

			//���������, ���� �� ��������� ��������
			type = LookForward(1);
			exit = LookForward(2);
			if (type == typeComma && (exit == typeId || exit == typeConstInt || exit == typeConstFloat))
			{
				next = Scan(lex);
				type = LookForward(1);
			}
			if (type == typeRightBracket)
			{
				next = -1;
			}
		} while (next == typeComma);
	}

	type = Scan(lex);
	if (type != typeRightBracket)
	{
		scaner->PrintError("������� ������ � ��������� FunctionCall, �������� ������ ')', ", lex);
	}
}

void Diagram::Expression()
{
	type_lex lex;
	int type;

	Comparison();
	type = LookForward(1);
	while (type == typeEq || type == typeUnEq)
	{
		type = Scan(lex);
		Comparison();
		type = LookForward(1);
	}

	/*if (type != typeSemicolon && type != typeComma)
	{
		type = Scan(lex);
		scaner->PrintError("������� ������ � ��������� Expression, �������� ���������� ��������, ';' ��� ',', ", lex);
	}*/
}
//&& type != typeRightBracket

void Diagram::Comparison()
{
	type_lex lex;
	Summand();
	int type = LookForward(1);
	while (type == typeLess || type == typeLessOrEq || type == typeMore || type == typeMoreOrEq)
	{
		type = Scan(lex);

		Tree* node = tree->FindUp(lex);

		Summand();
		type = LookForward(1);
	}
}

void Diagram::Summand()
{
	type_lex lex;
	int type;
	Multiplier();
	type = LookForward(1);
	while (type == typePlus || type == typeMinus)
	{
		type = Scan(lex);
		Multiplier();
		type = LookForward(1);
	}
}

void Diagram::Multiplier()
{
	type_lex lex;
	int type;
	UnaryOperation();
	type = LookForward(1);
	while (type == typeMul || type == typeDiv || type == typeMod)
	{
		type = Scan(lex);

		UnaryOperation();
		type = LookForward(1);
	}
}


void Diagram::UnaryOperation()
{
	type_lex lex;
	int type = LookForward(1);

	if (type == typePlus || type == typeMinus)
	{
		type = Scan(lex);
	}
	ElementaryExpression();
}

void Diagram::ElementaryExpression()
{
	type_lex lex;
	int type;

	type = Scan(lex);
	int amount = strlen(lex);

	pointer_2 = scaner->GetUK() - amount;
	scaner->PutUK(pointer_2);
	type = LookForward(1);
	//type = Scan(lex);

	//type = LookForward(1);
	if (type == typeId)
	{

		Tree* node = tree->FindUp(lex);

		type = LookForward(2);
		if (type == typeLeftBracket)
		{
			FunctionCall();
			return;
		}

		type = Scan(lex);

		if (node == nullptr)
		{
			scaner->PrintError("������������� ������. ���������� �� �������, ", lex);
		}
		else if (!node->IsSelfInit())
		{
			scaner->PrintError("������������� ������. ���������� �� ����������������, ", lex);
		}
		return;
	}
	if (type == typeConstInt || type == typeConstChar || type == typeConstFloat)
	{
		type = Scan(lex);
		return;
	}
	if (type == typeLeftBracket)
	{
		type = Scan(lex);
		Expression();
		type = Scan(lex);
		if (type != typeRightBracket)
		{
			scaner->PrintError("������� ������ � ��������� ElementaryExpression, �������� ������, ')', ", lex);
		}
		return;
	}
	type = Scan(lex);
	scaner->PrintError("������� ������ � ��������� ElementaryExpression, ��������� ���������, ", lex);
}

void Diagram::ParameterList()
{
	type_lex lex;
	Node* newNode = new Node();
	int type;
	int type2;
	int checkVarType;

	do {
		checkVarType = Type();
		type = LookForward(1);


		Tree* varNode = NULL;
		type_data typeData = tree->GetDataType(checkVarType);


		if (type != typeId)
		{
			type = Scan(lex);
			scaner->PrintError("������� ������ � ��������� Data, �������� ������������� ����������, ", lex);
		}


		int pointer = scaner->GetUK();
		type = Scan(lex);

		if (tree->IsDoublicateId(tree, lex)) {
			tree->PrintError("��������������� ", lex);
		}

		newNode->id = lex;  // ������������� �������������
		newNode->objectType = OBJ_PARAM;
		newNode->dataType = tree->GetDataType(checkVarType);

		// ��������� ���� � ����� ���������
		tree->SetLeft(newNode);

		// ������� � ������ ��������� ���� ��� ���������� ��������
		tree = tree->GetLeft();

		//���������, ���� �� ��������� ��������
		type = LookForward(1);
		if (type == typeComma)
		{
			type = Scan(lex); //���������� �������
			
		}

	} while (type == typeComma);


}

void Diagram::Switch_operator()
{
	type_lex lex;
	int type;

	Node* newNode = new Node();
	Tree* tmpTree = tree;

	type = Scan(lex);

	// ������ ����� ���� ��� �������
	newNode->id = lex;  // ��������� �������������
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = tree->GetDataType(type);

	// ��������� ���� � ������
	tree->SetLeft(newNode);
	tree = tree->GetLeft(); // ������� � ������ ����
	tree->SetRight(NULL);

	tmpTree = tree; // ��������� ������� ��������� ������
	tree = tree->GetRight(); // ������� � ������� ���������


	if (type != typeSwitch)
	{
		scaner->PrintError("������: ��������� �������� ����� 'switch'", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("������: �������� ������ '('", lex);
	}

	Expression();

	type = Scan(lex);
	if (type != typeRightBracket)
	{
		scaner->PrintError("������: �������� ������ ')'", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBrace)
	{
		scaner->PrintError("������: �������� ������ '{'", lex);
	}

	type = LookForward(1);
	while (type == typeCase || type == typeDefault)
	{
		CaseOperator();
		type = LookForward(1);
	}

	type = Scan(lex);
	if (type != typeRightBrace)
	{
		scaner->PrintError("������: �������� ������ '}'", lex);
	}

	// ������������ � ����������� ����
	tree = tmpTree;

}

void Diagram::CaseOperator()
{
	type_lex lex;
	int type = Scan(lex);
	int next;

	if (type != typeCase && type != typeDefault)
	{
		scaner->PrintError("������: ��������� �������� ����� 'case' ��� 'default'", lex);
	}

	if (type == typeCase)
	{
		Expression();
	}

	type = Scan(lex);
	if (type != typeColon)
	{
		scaner->PrintError("������: �������� ������ ':'", lex);
	}

	next = LookForward(1);
	if (next != typeCase && next != typeDefault)
	{
		OperatorsAndDescriptions();
	}
}
