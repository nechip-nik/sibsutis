(defun task1()
(caar(cadaar'(((1((*)2 3))))))
)
(defun task2()
(cons'(+ 1 2) '(+ 4 6))
;;((+ 1 2) + 4 6)
)
(defun task3a()
;;(((1 2 3 )))
;;(cons (cons '(1 2 3) nil ) nil )
(cons (cons (cons 1 (cons 2 (cons 3 nil)))nil)nil)
)
(defun task3b()
(list (list (list 1 2 3)))
)
(defun task4(e)
(append (list (car(cddr e))(car(cdr e))(car e)(car(cdddr e))))
)