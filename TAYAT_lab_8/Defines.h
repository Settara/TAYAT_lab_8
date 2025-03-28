#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxText 100000			// Максимальная длина текста
#define maxLex 100				// Максимальная длина лексемы
#define maxKeyword 16			// Максимальное количество ключевых слов
#define maxNumber 10			// Максимальная длина числа
#define maxHexNumber 10			// Максимальная длина 16сс числа
typedef char type_mod[maxText]; // Текст
typedef char type_lex[maxLex];  // Лексема


/**
* Ключевые слова
*/
#define typeInt 1
#define typeShort 2
#define typeLong 3
#define typeFor 4
#define typeReturn 5
#define typeMain 6
#define typeConst 7
#define typeVoid 8
#define typeFloat 9
#define typeSwitch 10
#define typeCase 11
#define typeDefault 12
#define typeChar 13

/**
* Тип лексем
*/
#define typeId 20

/**
* Константы
*/
#define typeConstInt 30
#define typeConstLongInt 31
#define typeConstHex 32
#define typeConstLongHex 33
#define typeConstChar 34
#define typeConstFloat 35


/**
* Знаки операций
*/
#define typePlus 40			    // +
#define typeMinus 41			// -
#define typeMul 42				// *
#define typeDiv 43				// /
#define typeMod 44				// %
#define typeEq 45				// ==
#define typeUnEq 46				// !=
#define typeLess 47				// <
#define typeLessOrEq 48			// <=
#define typeMore 49				// >
#define typeMoreOrEq 50			// >=
#define typeEval 51				// =

/**
* Специальные знаки
*/
#define typeSemicolon 60		// ;
#define typeComma 61			// ,
#define typeLeftBracket 62		// (
#define typeRightBracket 63		// )
#define typeLeftBrace 64		// {
#define typeRightBrace 65		// }
#define typeColon 66		// :


/**
* Дополнительно
*/
#define typeEnd 100
#define typeError 404