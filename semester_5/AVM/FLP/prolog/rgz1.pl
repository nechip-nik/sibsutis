square_third_elem([], []).
square_third_elem([_, _, X | Tail], [X_squared | Rest]) :-
    X_squared is X * X,
    square_third_elem(Tail, Rest).
square_third_elem([_ | Tail], Rest) :-
    square_third_elem(Tail, Rest). 