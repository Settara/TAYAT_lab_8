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
		scaner->PrintError("найдена ошибка в структуре Program, ожидался конец программы, ", lex);
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
			scaner->PrintError("найдена ошибка в структуре Data, ожидался идентификатор переменной, ", lex);
		}

	
		int pointer = scaner->GetUK();
		type = Scan(lex);

		if (tree->IsDoublicateId(tree, lex)) {
			tree->PrintError("Переопределение ", lex);
		}

		newNode->id = lex;  // Устанавливаем идентификатор
		newNode->objectType = OBJ_VAR;
		newNode->dataType = tree->GetDataType(checkVarType);

		type = LookForward(1);
		if (type == typeEval) {
			newNode->flagInit = 1;
			type = Scan(lex); // Сканируем '='
			int assignedType = LookForward(1);

			if (checkVarType != assignedType) {
				scaner->PrintError("Ошибка: тип значения не соответствует типу переменной ", lex);
			}
		}
		else
		{
			newNode->flagInit = 0;
		}


		// Добавляем узел в левое поддерево
		tree->SetLeft(newNode);

		// Переход к левому дочернему узлу для дальнейших операций
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
		scaner->PrintError("найдена ошибка в структуре Data, ожидался символ ';', ", lex);
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
		scaner->PrintError("найдена ошибка в структуре Named_const, ожидалось ключевое слово 'const', ", lex);
	}

	type = Scan(lex);

	//Проверяем что значение константы соответсвует ее типу
	checkConstType = TypeConst();
	type = LookForward(3);
	if (checkConstType != type)
	{
		scaner->PrintError("найдена ошибка в структуре Named_const, значение не соотвествует типу объявленной константы ", lex);
	}

	do {
		type = LookForward(1);
		
		Tree* varNode = NULL;
		type_data typeData = tree->GetDataType(checkConstType);
		

		int pointer = scaner->GetUK();
		type = Scan(lex);

		if (tree->IsDoublicateId(tree, lex)) {
			tree->PrintError("Переопределение ", lex);
		}

		newNode->id = lex;  // Устанавливаем идентификатор
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

		// Добавляем узел в левое поддерево
		newNode->objectType = OBJ_NAMED;
		tree->SetLeft(newNode);

		// Переход к левому дочернему узлу для дальнейших операций
		tree = tree->GetLeft();

		scaner->PutUK(pointer);

		Assignment();
		type = Scan(lex);
	} while (type == typeComma);

	if (type != typeSemicolon)
	{
		scaner->PrintError("найдена ошибка в структуре Named_const, ожидался символ ';', ", lex);
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
		scaner->PrintError("найдена ошибка в типе функции, ожидался тип данных (void), ", lex);
	}
	type = Scan(lex);


	type = Scan(lex);
	if (type != typeId && type != typeMain)
	{
		scaner->PrintError("найдена ошибка в структуре Function, ожидался идентификатор функции или ключевое слово 'main', ", lex);
	}

	
	// проверка на повторение
	if (tree->IsDoublicateId(tree, lex)) {
		scaner->PrintError("Переопределение, ", lex);
	}

	// Создаём новый узел для функции
	Node* newNode = new Node();
	newNode->id = lex;  // Назначаем идентификатор
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = tree->GetDataType(type);

	// Вставляем узел в дерево
	tree->SetLeft(newNode);
	tree = tree->GetLeft(); // Переход к новому узлу
	tree->SetRight(NULL);

	Tree* tmpTree = tree; // Сохраняем текущий указатель дерева
	tree = tree->GetRight(); // Переход к правому поддереву


	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("найдена ошибка в структуре Function, ожидался символ '(', ", lex);
	}

	//Обрабатываем параметры функции, если они есть
	type = LookForward(1);
	if (type == typeInt || type == typeShort || type == typeLong || type == typeFloat)
	{
		ParameterList();
	}

	type = Scan(lex);
	if (type != typeRightBracket)
	{
		scaner->PrintError("найдена ошибка в структуре Function, ожидался символ ')', ", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBrace)
	{
		scaner->PrintError("найдена ошибка в структуре Function, ожидался символ '{', ", lex);
	}

	type = LookForward(1);

	while (type != typeRightBrace)
	{
		OperatorsAndDescriptions();
		type = LookForward(1);
	}

	// Возвращаемся к предыдущему узлу
	tree = tmpTree;

	type = Scan(lex);
	if (type != typeRightBrace)
	{
		scaner->PrintError("найдена ошибка в структуре Function, ожидался символ '}', ", lex);
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
		scaner->PrintError("найдена ошибка в структуре Type, ожидался тип данных (int, short, long, float), ", lex);
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
		scaner->PrintError("найдена ошибка в структуре TypeConst, ожидался тип данных константы (int, char, float), ", lex);
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
		scaner->PrintError("Семантическая ошибка. ID не найден, ", lex);
	}
	if (node->GetSelfObjectType() == OBJ_NAMED_ASSIGNED)
	{
		scaner->PrintError("Семантическая ошибка. Попытка переприсваивания значения именованной константе, ", lex);
	}
	if (node->GetSelfObjectType() != OBJ_VAR && node->GetSelfObjectType() != OBJ_NAMED) {
		scaner->PrintError("Семантическая ошибка. Попытка использования не переменной в присваивании, ", lex);
	}
	if (node->GetSelfObjectType() == OBJ_NAMED)
	{
		node->changeObjType(OBJ_NAMED_ASSIGNED);
	}
	node->SetInit();

	// Получаем тип переменной узла
	type_data varType = node->GetSelfDataType();
	

	type = Scan(lex);
	if (type != typeEval)
	{
		scaner->PrintError("найдена ошибка в структуре Assignment, ожидался символ '=', ", lex);
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
		scaner->PrintError("найдена ошибка в структуре CompositeOperator, ожидался символ '{', ", lex);
	}

	// Создаём новый узел для функции
	Node* newNode = new Node();
	newNode->id = lex;  // Назначаем идентификатор
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = tree->GetDataType(type);

	// Вставляем узел в дерево
	tree->SetLeft(newNode);
	tree = tree->GetLeft(); // Переход к новому узлу
	tree->SetRight(NULL);

	Tree* tmpTree = tree; // Сохраняем текущий указатель дерева
	tree = tree->GetRight(); // Переход к правому поддереву
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
		scaner->PrintError("найдена ошибка в структуре CompositeOperator, ожидался символ '}', ", lex);
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
			scaner->PrintError("найдена ошибка в структуре Operator, ожидался символ ';' после return <выражение>, ", lex);
		}
		return;
	}

	if (type == typeSemicolon) // пустой оператор
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
			scaner->PrintError("найдена ошибка в структуре Operator, ожидался символ ';', ", lex);
		}
		return;
	}

	if (type == typeId && type2 == typeEval)
	{
		Assignment();
		type = Scan(lex);
		if (type != typeSemicolon)
			scaner->PrintError("найдена ошибка в структуре Operator, ожидался символ ';', ", lex);
		return;
	}

	type = Scan(lex);
	scaner->PrintError("найдена ошибка в структуре Operator, ожидался оператор, ", lex);
}

void Diagram::For_operator()
{
	type_lex lex;
	int type;

	Node* newNode = new Node();
	Tree* tmpTree = tree;

	type = Scan(lex);

	// Создаём новый узел для функции
	newNode->id = lex;  // Назначаем идентификатор
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = tree->GetDataType(type);

	// Вставляем узел в дерево
	tree->SetLeft(newNode);
	tree = tree->GetLeft(); // Переход к новому узлу
	tree->SetRight(NULL);

	tmpTree = tree; // Сохраняем текущий указатель дерева
	tree = tree->GetRight(); // Переход к правому поддереву
	

	if (type != typeFor)
	{
		scaner->PrintError("найдена ошибка в структуре For_operator, ожидалось ключевое слово 'for', ", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("найдена ошибка в структуре For_operator, ожидался символ '(', ", lex);
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
			scaner->PrintError("найдена ошибка в структуре For_operator, ожидался символ ';', ", lex);
		}
	}
	else
	{
		type = Scan(lex);
		if (type != typeSemicolon)
		{
			scaner->PrintError("найдена ошибка в структуре For_operator, ожидался символ ';', ", lex);
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
		scaner->PrintError("найдена ошибка в структуре For_operator, ожидался символ ';', ", lex);
	}

	type = LookForward(1);
	if (type != typeRightBracket)
	{
		Assignment();
	}

	type = Scan(lex);
	if (type != typeRightBracket)
	{
		scaner->PrintError("найдена ошибка в структуре For_operator, ожидался символ ')', ", lex);
	}

	OperatorsAndDescriptions();

	// Возвращаемся к предыдущему узлу
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
		scaner->PrintError("найдена ошибка в структуре FunctionCall, ожидался идентификатор функции, ", lex);
	}
	
	// проверка на повторение
	// Ищем узел, называющийся объектом
	Tree* objectNode = tree->FindUp(lex);
	// Получаем указатель на функцию из функции с тем же именем в родительском узле
	Tree* methodNode = objectNode->FindFunction(lex); // Функция для поиска по имени функции в родительском узле на findUp
	if (methodNode == NULL) {
		scaner->PrintError("Функция не найдена, ", lex);
		return; // Выход из функции при ошибке
	}

	// Проверяем, что узел является функцией
	if (methodNode->GetSelfObjectType() != OBJ_FUNC) {
		scaner->PrintError("Не является функцией, ", lex);
	}
	/////////

	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("найдена ошибка в структуре FunctionCall, ожидался символ '(', ", lex);
	}

	//Обрабатываем параметры функции, если они есть
	type = LookForward(1);
	next = -1;
	exit = -1;
	if (type == typeId || type == typeConstInt || type == typeConstFloat)
	{
		//Обрабатываем список параметров
		do {
			//Обрабатываем значение параметра (идентификатор или константа)
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
						scaner->PrintError("Семантическая ошибка. Переменная не найдена, ", lex);
					}
					else
					{
						if (!proverka->IsSelfInit())
						{
							scaner->PrintError("Семантическая ошибка. Переменная не инициализирована, ", lex);
						}
					}
				}
				else
				{
					scaner->PrintError("найдена ошибка в структуре FunctionCall, ожидался идентификатор или константа, ", lex);
				}
			}

			//Проверяем, есть ли следующий параметр
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
		scaner->PrintError("найдена ошибка в структуре FunctionCall, ожидался символ ')', ", lex);
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
		scaner->PrintError("найдена ошибка в структуре Expression, ожидался логический оператор, ';' или ',', ", lex);
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
			scaner->PrintError("Семантическая ошибка. Переменная не найдена, ", lex);
		}
		else if (!node->IsSelfInit())
		{
			scaner->PrintError("Семантическая ошибка. Переменная не инициализирована, ", lex);
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
			scaner->PrintError("найдена ошибка в структуре ElementaryExpression, ожидался символ, ')', ", lex);
		}
		return;
	}
	type = Scan(lex);
	scaner->PrintError("найдена ошибка в структуре ElementaryExpression, ожидалось выражение, ", lex);
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
			scaner->PrintError("найдена ошибка в структуре Data, ожидался идентификатор переменной, ", lex);
		}


		int pointer = scaner->GetUK();
		type = Scan(lex);

		if (tree->IsDoublicateId(tree, lex)) {
			tree->PrintError("Переопределение ", lex);
		}

		newNode->id = lex;  // Устанавливаем идентификатор
		newNode->objectType = OBJ_PARAM;
		newNode->dataType = tree->GetDataType(checkVarType);

		// Добавляем узел в левое поддерево
		tree->SetLeft(newNode);

		// Переход к левому дочернему узлу для дальнейших операций
		tree = tree->GetLeft();

		//Проверяем, есть ли следующий параметр
		type = LookForward(1);
		if (type == typeComma)
		{
			type = Scan(lex); //Пропускаем запятую
			
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

	// Создаём новый узел для функции
	newNode->id = lex;  // Назначаем идентификатор
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = tree->GetDataType(type);

	// Вставляем узел в дерево
	tree->SetLeft(newNode);
	tree = tree->GetLeft(); // Переход к новому узлу
	tree->SetRight(NULL);

	tmpTree = tree; // Сохраняем текущий указатель дерева
	tree = tree->GetRight(); // Переход к правому поддереву


	if (type != typeSwitch)
	{
		scaner->PrintError("Ошибка: ожидалось ключевое слово 'switch'", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("Ошибка: ожидался символ '('", lex);
	}

	Expression();

	type = Scan(lex);
	if (type != typeRightBracket)
	{
		scaner->PrintError("Ошибка: ожидался символ ')'", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBrace)
	{
		scaner->PrintError("Ошибка: ожидался символ '{'", lex);
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
		scaner->PrintError("Ошибка: ожидался символ '}'", lex);
	}

	// Возвращаемся к предыдущему узлу
	tree = tmpTree;

}

void Diagram::CaseOperator()
{
	type_lex lex;
	int type = Scan(lex);
	int next;

	if (type != typeCase && type != typeDefault)
	{
		scaner->PrintError("Ошибка: ожидалось ключевое слово 'case' или 'default'", lex);
	}

	if (type == typeCase)
	{
		Expression();
	}

	type = Scan(lex);
	if (type != typeColon)
	{
		scaner->PrintError("Ошибка: ожидался символ ':'", lex);
	}

	next = LookForward(1);
	if (next != typeCase && next != typeDefault)
	{
		OperatorsAndDescriptions();
	}
}
