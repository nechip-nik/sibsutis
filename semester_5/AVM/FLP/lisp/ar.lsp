(defun delete_end(param)
(cond
((null (cdr param)) nil)
(t (cons (car param) (delete_end (cdr param))))
)
)

(defun repl (x)
(list (car x) (cadr x) (last x) (cdddr (delete_end x)) (caddr x))
)
(repl'(1 2 3 4 5 6))