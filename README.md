# Анализатор

**ФИО:** Зыков Александр Дмитриевич

**Группа:** КМБО-05-23

**Вариант:** 3


## Описание
Рекурсивный нисходящий парсер для вычисления битовых выражений с операторами: 
- `|` (ИЛИ)
- `&` (И)
- `<<` (сдвиг влево)
- `>>` (сдвиг вправо)
- `~` (НЕ)
- скобки `(` и `)`
- точка с запятой `;` для разделения нескольких выражений

## Грамматика (с учётом program)
```bnf
<program> ::= <expr> <program'>
<program'> ::= ";" <expr> <program'> | ε
<expr>    ::= <or>
<or>      ::= <and> <or'>
<or'>     ::= "|" <and> <or'> | ε
<and>     ::= <shift> <and'>
<and'>    ::= "&" <shift> <and'> | ε
<shift>   ::= <unary> <shift'>
<shift'>  ::= "<<" <unary> <shift'> | ">>" <unary> <shift'> | ε
<unary>   ::= "~" <unary> | <primary>
<primary> ::= INT | "(" <expr> ")"
```

## Сборка
mkdir build

cd build

cmake ..

cmake --build .

cd ..

## Использование
.\build\Debug\expr_parser.exe

Enter expression: 1 << (2 & 3)
Result: 4
Hex: 0x4

.\build\Debug\expr_parser.exe
Enter expression: 1; 2; 3
Multiple results:
  [0] 1 (0x1)
  [1] 2 (0x2)
  [2] 3 (0x3)


**ИЛИ**

.\build\Debug\expr_parser.exe input.txt

Запуск тестов :
.\build\Debug\expr_parser.exe --test

## Примеры работы
Вход	Выход
1 << (2 & 3)	Result: 4, Hex: 0x4
~0 & 255	Result: 255, Hex: 0xff
(1 | 4) >> 1	Result: 2, Hex: 0x2
1; 2; 3	1, 2, 3

## Обработка ошибок
Программа обнаруживает и сообщает об ошибках:

Лексические
Неизвестные символы (@, $, # и т.д.)

Переполнение чисел (> 2³²-1)

Синтаксические
Незакрытые скобки

Пропущенные операнды (1 |)

Лишние символы (1 2 3)

Пустые выражения после ;

Семантические
Сдвиг на ≥32 бита




