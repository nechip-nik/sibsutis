insert_before_all([], _, _, []).
insert_before_all([Y|T], X, Y, [X,Y|Rest]) :-
    insert_before_all(T, X, Y, Rest).
insert_before_all([H|T], X, Y, [H|Rest]) :-
    insert_before_all(T, X, Y, Rest).
