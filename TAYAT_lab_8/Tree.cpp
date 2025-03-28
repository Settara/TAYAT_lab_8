#include "Tree.h"

void Tree::changeObjType(type_object str)
{
	node->objectType = str;
}

Tree::Tree()
{
	up = NULL;
	left = NULL;
	right = NULL;
	node = NULL;
}

Tree::Tree(Tree* up, Tree* left, Tree* right, Node* node)
{
	this->up = up;
	this->left = left;
	this->right = right;
	this->node = new Node();
	if (node != NULL)
		memcpy(this->node, node, sizeof(Node));
}

Tree::~Tree()
{
	if (left != nullptr)
		delete left;
	if (right != nullptr)
		delete right;
	if (node != nullptr)
		delete node;
}

void Tree::PrintError(std::string errorMessage, std::string lexeme)
{
	std::cout << "������������� ������. " << errorMessage << " �������: " << lexeme << std::endl;
	exit(1);
}

void Tree::SetLeft(Node* node)
{
	this->left = new Tree(this, NULL, NULL, node);
}

Tree* Tree::FindFunction(const type_lex& lex)
{
	Tree* currentNode = this;

	// ���������� ���� � ������
	while (currentNode != nullptr)
	{
		if (currentNode->node != nullptr &&
			currentNode->node->id == lex &&
			currentNode->node->objectType == OBJ_FUNC)
		{						// �������� �� ��� �������
			return currentNode; // ���� ������� �������, ���������� ��
		}
		currentNode = currentNode->GetRight(); // ������� � ���������� ����
	}
	return nullptr; // ����� �� ������
}

void Tree::SetRight(Node* node)
{
	this->right = new Tree(this, NULL, NULL, node);
}

Tree* Tree::GetLeft()
{
	return this->left;
}

Tree* Tree::GetRight()
{
	return this->right;
}

Tree* Tree::FindUp(Tree* from, std::string id)
{
	Tree* current = from;

	while (current != NULL)
	{
		if (current->node != NULL)
		{
			if (id.compare(current->node->id) == 0)
				break;
		}
		current = current->up;
	}

	return current;
}

Tree* Tree::FindUp(std::string id)
{
	return FindUp(this, id);
}

Tree* Tree::FindUp(Tree* from, std::string id, type_object type)
{
	Tree* current = from;

	while (current != NULL)
	{
		if (current->node != NULL)
		{
			if (id.compare(current->node->id) == 0 && type == current->node->objectType)
				break;
		}

		current = current->up;
	}

	return current;
}

Tree* Tree::FindUp(std::string id, type_object type)
{
	return FindUp(this, id, type);
}

Tree* Tree::FindUp(Tree* from, type_object type)
{
	Tree* current = from;

	while (current != NULL)
	{
		if (current->node != NULL)
		{
			if (type == current->node->objectType)
				break;
		}

		current = current->up;
	}

	return current;
}

Tree* Tree::FindUp(type_object type)
{
	return FindUp(this, type);
}

Tree* Tree::FindUpOneLevel(Tree* from, std::string id)
{
	Tree* current = from;

	while (current != NULL && current->up != NULL && current->up->right != current)
	{
		if (id.compare(current->node->id) == 0)
		{
			return current;
		}
		current = current->up;
	}
	return NULL;
}

Tree* Tree::FindRightLeft(Tree* from, std::string id)
{
	Tree* curNode = from->right;
	while (curNode != NULL)
	{
		if (curNode->node != NULL)
		{
			if (id.compare(curNode->node->id) == 0)
				break;
		}

		curNode = curNode->left;
	}
	return curNode;
}

Tree* Tree::FindRightLeft(std::string id)
{
	return FindRightLeft(this, id);
}

Tree* Tree::FindLeft(Tree* from, std::string id)
{
	Tree* current = from;

	while (current != NULL)
	{
		if (current->node != NULL)
			if (id.compare(current->node->id) == 0)
				break;

		current = current->left;
	}

	return current;
}

Tree* Tree::FindLeft(std::string id)
{
	return FindLeft(this, id);
}

type_data Tree::GetDataType(int type)
{
	if (type == typeInt)
		return TYPE_INTEGER;
	if (type == typeShort)
		return TYPE_SHORT;
	if (type == typeLong)
		return TYPE_LONG;
	return TYPE_UNKNOWN;
}

//type_data Tree::GetTypeResult(type_data type1, type_data type2, int operation)
//{
//	if (operation >= typeUnEq && operation <= typeMore)
//		return TYPE_SHORT;
//	if (operation == typeMod)
//		return TYPE_UNKNOWN;
//	if (type1 >= type2)
//		return type1;
//
//	return type2;
//}

void Tree::Print()
{
	if (node != NULL)
	{
		std::cout << "����: " << node->id << " ";
	}

	if (left != NULL)
	{
		std::cout << "����: " << left->node->id << " ";
		left->Print(); // ����������� ����� ��� ������ ��������� ����
	}

	if (right != NULL)
	{
		std::cout << "�����: " << right->node->id << " ";
		right->Print(); // ����������� ����� ��� ������� ���������
	}
	std::cout << std::endl;
}

void Tree::Print(int level)
{
	// std::string indent(level * 2, ' ');
	std::string indent = "";
	std::cout << std::endl;
	if (node != NULL)
	{
		if (node->id == "")
			std::cout << "����: ";
		else
			std::cout << "����: " << node->id;
	}
	else
	{
		std::cout << indent << "����: ";
	}
	if (right != NULL)
	{

		std::cout << indent << " ����� -> " << right->node->id;

		// right->Print(level + 1); // ����������� ����� ��� ������� ��������� ����
	}
	if (left != NULL)
	{
		std::cout << indent << " ���� ->" << left->node->id;
		// left->Print(level + 1); // ����������� ����� ��� ������ ��������� ����
	}
	if (right != NULL)
	{

		// std::cout << indent << " ����� -> ";

		right->Print(level + 1); // ����������� ����� ��� ������� ��������� ����
	}
	if (left != NULL)
	{
		// std::cout << indent << " ���� ->";
		left->Print(level + 1); // ����������� ����� ��� ������ ��������� ����
	}
}

bool Tree::IsDoublicateId(Tree* addr, std::string id)
{
	if (FindUpOneLevel(addr, id) == NULL)
		return false;
	return true;
}

bool Tree::IsAllowChangeId(std::string id)
{
	Tree* node = FindUp(id);
	if (node->node->objectType != OBJ_VAR)
		return false;
	return true;
}

bool Tree::IsMainExists()
{
	Tree* root = this;

	while (root->up != NULL)
		root = root->up;

	Tree* main = root->FindLeft("main");

	if (main == NULL)
		return false;
	return true;
}

type_object Tree::GetSelfObjectType()
{
	return node->objectType;
}

type_data Tree::GetSelfDataType()
{
	return node->dataType;
}

std::string Tree::GetSelfId()
{
	return node->id;
}

bool Tree::IsSelfInit()
{
	return node->flagInit;
}

void Tree::SetInit()
{
	this->node->flagInit = 1;
}
