# universal-array
main.cpp содержит класс UniversalArray, реализованный на АВЛ-дереве по неявному ключу, с реализованными операциями find, insert, remove, split, merge, reverse, с возможностью присвоения и прибавления на отрезке и запросов суммы, минимума и максимума на отрезке.

Сам main.cpp способен отвечать на запросы:

v pos value - вставка value на позицию pos

x pos - удаление элемента на позиции pos

. pos - вывод значения элемента на позиции pos

= left right value - присвоение value на отрезке от left до right

\+ left right value - прибавление value на отрезке от left до right

? left right - вывести сумму, минимум и максимум на отрезке от left до right

r left right - перевернуть отрезок от left до right

exit - выход



left, right, pos должны быть в 0-индексации. Изначально массив пустой.
