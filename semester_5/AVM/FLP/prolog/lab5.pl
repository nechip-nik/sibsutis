odd(X) :- X mod 2 =\= 0.
odd(X, Y) :- Y >= X, odd(Y), write(Y), write('  '), fail.
odd(X, Y) :- Y > X, Y1 is Y - 1, odd(X, Y1).
odd(X, Y) :- Y == X.

print_odd() :-
    writeln('Предикат, который печатает все нечётные числа из диапазона в порядке убывания.'),
    writeln('******************************************************************************'),
    writeln('Введите начало границы диапазона:'), read(X),
    writeln('Введите конец границы диапазона:'), read(Y),
  nl, writeln('Результат:'), odd(X, Y).

t1:- print_odd().
/*-----------------------------------------------------------------*/

fibonacci(X, Y) :- X < 2, Y is 1, !.

fibonacci(X, Y) :-
  X2 is X - 2,
  X1 is X - 1,
  fibonacci(X2, Y2),
  fibonacci(X1, Y1),
  Y is Y2 + Y1.

fibonacci() :-
  repeat,
    writeln('Введите номер:'),
  read(X),
  (X < 0, !; (fibonacci(X, RES), write('Результат: '), writeln(RES), nl, nl, fail)).

print_fibonacci() :-
    writeln('Предикат, который находит числа Фибоначчи по их номерам, которые в цикле вводятся'),
  writeln('с клавиатуры (осуществляется до тех пор, пока не будет введено отрицательное число.'),
  writeln('***********************************************************************************'),
    fibonacci().


/*------------------------------------------*/


split(N1, N2, [H | T], [H | T1], L2, L3) :-
  H < min(N1, N2), !,
  split(N1, N2, T, T1, L2, L3).

split(N1, N2, [H | T], L1, [H | T2], L3) :-
  H =< max(N1, N2), !,
  split(N1, N2, T, L1, T2, L3).

split(N1, N2, [H | T], L1, L2, [H | T3]) :-
  split(N1, N2, T, L1, L2, T3).

split(_, _, [], [], [], []).


print_split() :-
  writeln('Написать предикат, который разбивает числовой список по двум числам, вводимым с'),
  writeln('клавиатуры на три списка: меньше меньшего введенного числа, от меньшего введенного'),
  writeln('числа до большего введенного числа, больше большего введенного числа. Список и два'),
  writeln('числа вводятся с клавиатуры в процессе работы предиката.'),
  writeln('***********************************************************************************'),
  writeln('Введите список: '), read(L),
  writeln('Введите первое число: '), read(N1),
  writeln('Введите второе число: '), read(N2),
  split(N1, N2, L, L1, L2, L3),
  writeln('Результат: '), writeln(L1), writeln(L2), writeln(L3).

/*------------------------------------------*/



mostСommon([], [], 0) :- !.

mostСommon([H | T], L_res, Max) :-
  delete(T, H, L_temp),
  length([H | T], LenL),
  length(L_temp, LenL_temp),
  Max_temp is LenL - LenL_temp,
  mostСommon(L_temp, L_new, Max_maybe),
  (Max_temp > Max_maybe -> % if
    (L_res = [H], Max is Max_temp);  % true
    (Max is Max_maybe,
    (Max_temp =:= Max_maybe ->  % false
      L_res = [H | L_new];
      L_res = L_new))
  ).

mostСommon(L, L_new) :-  mostСommon(L, L_new, _).

print_mostCommon() :-
  writeln('Написать предикат, который формирует список из наиболее часто встречающихся'),
  writeln('элементов списка. Список вводится с клавиатуры в процессе работы предиката.'),
  writeln('Встроенные предикаты поиска максимума и сортировки не использовать!'),
  writeln('***********************************************************************************'),
  writeln('Введите список: '), read(L),
  mostСommon(L, L_new),
  writeln('Результат: '), writeln(L_new).