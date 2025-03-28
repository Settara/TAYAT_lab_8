#pragma once

#include "Scaner.h"
#include "Tree.h"

class Diagram
{
private:
	Scaner* scaner;
	Tree* tree;
	int pointer_2;

	void Data();							// ������
	void Named_const();						// ����������� ���������
	void Function();						// �������
	void Type();							// ���
	void Assignment();						// ������������
	void Expression();						// ���������
	void CompositeOperator();				// ��������� ��������
	void OperatorsAndDescriptions();		// ��������� � ��������
	void Operator();						// ��������
	void FunctionCall();					// ����� �������
	void Comparison();						// ���������
	void Summand();							// ���������
	void Multiplier();						// ���������
	void UnaryOperation();					// ������� ��������
	void ElementaryExpression();			// ������������ ���������
	void For_operator();					// ����
	int TypeConst();						// ��� ���������
	int LookForward(int pointer);
	int Scan(type_lex lex);

public:
	Diagram(Scaner* scaner, Tree* tree);
	void Program();							// ���������
};