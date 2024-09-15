(defun f1 (x y lst)
    (cond
        ((null lst) 
            lst
        )
        ((= (car lst) x)
            (cons y (f1 x y (cdr lst)))
        )
        (t
            (cons (car lst) (f1 x y (cdr lst )))
        )
    )
)
(defun f2 (x lst)
    (cond
        ((null lst) 
            0
        )
        ((equal (car lst) x)
            (+ 1 (f2 x (cdr lst)))
        )
        (t
            (f2 x (cdr lst ))
        )
    )
)
(defun f3 (lst k n)
    (cond
        ((null lst) 
            lst
        )
        ((and (= k 1) (> n 0))
            (append (list (car lst)) (f3 (cdr lst) k (- n 1)))
        )
        (t
            (f3 (cdr lst ) (- k 1) n)
        )
    )
)
(defun f1r (x n)
    (cond
        ((null lst) 
            lst
        )
        ((and (= k 1) (> n 0))
            (append (list (car lst)) (f3 (cdr lst) k (- n 1)))
        )
        (t
            (f3 (cdr lst ) (- k 1) n)
        )
    )
)