#pragma once

#include "Scaner.h"
#include "Tree.h"

class Diagram
{
private:
	Scaner* scaner;
	Tree* tree;
	int pointer_2;

	void Data();							// Данные
	void Named_const();						// Именованная константа
	void Function();						// Функция
	int Type();								// Тип
	void Assignment();						// Присваивание
	void Expression();						// Выражение
	void CompositeOperator();				// Составной оператор
	void OperatorsAndDescriptions();		// Операторы и описания
	void Operator();						// Оператор
	void FunctionCall();					// Вызов функции
	void Comparison();						// Сравнение
	void Summand();							// Слагаемое
	void Multiplier();						// Множитель
	void UnaryOperation();					// Унарная операция
	void ElementaryExpression();			// Элементарное выражение
	int TypeConst();						// Тип константы
	int LookForward(int pointer);
	int Scan(type_lex lex);
	void ParameterList();					//Список параметров
	void Switch_operator();					//Оператор Switch
	void CaseOperator();					

public:
	Diagram(Scaner* scaner, Tree* tree);
	void Program();							// Программа
};