class B {
s : String <- "Hello";
g (y:String) : String {
y.concat(s)
};
f (x:Int) : Int {
x+1
};
};
class A inherits C {
a : Int;
b : B <- new B;
f(x:Int) : Int {
x+a
};
};


class X inherits Y{
s : String <- "Hello";
g (y:String) : String {
y.concat(s)
};
f (x:Int) : Int {
x+1
};
};
class Y inherits X {
a : Int;
b : B <- new B;
f(x:Int) : Int {
x+a
};
};