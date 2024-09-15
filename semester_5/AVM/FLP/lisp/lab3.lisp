(defun run31()
    (rrr '(1 2 3 4 5) '( 2 8))
)

(defun rrr(a b)
    (cond
      ((null a) nil)
      ((member (car a) b)
      T
    )
    (t 
      (rrr (cdr a) b)
    )
  )
)

(defun run32()
    (rrr2 '(1 1 2 3 4) '(1  3))
)    

(defun rrr2(a b)
    (cond
      ((null a) a)
      ((member (car a) b)
        (rrr2 (cdr a) b)
    )
    (t 
        (cons(car a)(rrr2 (cdr a) b)) 
    )
  )
)

(defun apply-operation (list1 list2 operation)
  (if (null list1)
      '()
    (cons (funcall operation (car list1) (car list2))
          (apply-operation (cdr list1) (cdr list2) operation))))

 
  
  ;;(apply-operation list1 list2 max-operation) ; => (5 6 7 8)

  
  ;;(apply-operation list1 list2 #'/)) ; => (1/5 1/3 3/7 1/2)