**1. 为什么构造函数不能为虚函数？**

  虚函数的调用需要虚函数表指针，而该指针存放在对象的内容空间中；若构造函数声明为虚函数，那么由于对象还未创建，还没有内存空间，更没有虚函数表地址用来调用虚函数——构造函数了。

 

**2. 为什么析构函数可以为虚函数，如果不设为虚函数可能会存在什么问题？**

  首先析构函数可以为虚函数，而且当要使用基类指针或引用调用子类时，最好将基类的析构函数声明为虚函数，否则可以存在内存泄露的问题。

  举例说明：

  子类B继承自基类A；**A \*p = new B; delete p;**

  1） 此时，如果类A的析构函数不是虚函数，那么delete p；**将会仅仅调用A的析构函数，只释放了B对象中的A部分，而派生出的新的部分未释放掉。**（因为这个是对象A的指针，不是B的）

  2） 如果类A的析构函数是虚函数，delete p; 将会先调用B的析构函数，再调用A的析构函数，释放B对象的所有空间。

  补充： B *p = new B; delete p;时也是先调用B的析构函数，再调用A的析构函数。

### 2、 raw pointer, smart pointer , reference

只要是需要动态分配的对象就可以用智能指针。智能指针就是为了方便管理内存而生的，不会有有个地方用了智能指针而它指向的内存却为空这种情况（这是原始指针最大的缺点）（只要你规范的使用智能指针，不随意将它转换成原始指针）。

一个类拥有一个不属于它的成员最好用引用，因为引用必须要在构造函数中赋值，这就明确了引用的生命期。

最好不要用指针，除非是在写c。



### 3、 基类、派生类

派生类一定是基类，但是基类不一定是特定派生类，所以只能基类指向派生类。

一个基类指针指向一个派生类对象，这个时候调用的是派生类的构造函数。但是对于编译器而言，这个指针仅仅只是一个基类指针而已，你只能通过这个指针访问基类的成员。唯一的例外就是虚函数。而虚函数的实现是通过虚函数表来实现的，也就是将基类的同名虚函数覆盖了，指向了派生类的虚函数，也就是说，对于编译器而言，这个指针还是一个基类指针，只不过发生了动态绑定，这个“基类的函数”变成了派生类的函数，而这个派生类的函数自然可以访问派生类的成员，又因为执行的是派生类的构造函数，所以所有派生类成员这个虚函数都可以访问，要注意，并不是基类指针可以访问派生类，而是基类的某个函数发生了动态绑定，使得这个函数指向了派生类，因此可以访问派生类。对于这个指针而言，派生类的一切都是不可见的。

另外，考虑以下情况：

```c++
class a {
public:
	a() {
		cout << "a" << endl;
		//func();
	}
	virtual void func() = 0;// 纯虚函数不能在父类中被调用，虽然理论上来说可以被子类动态绑定，但是你不知道会不会有子类去覆写它，所以编译就不会通过
	virtual void print() {
		cout << "aPrint" << endl;
	}
	void init() {
		cout << "a" << endl;
		print();
	}
};

class b : public a {
public:
	b()
		:a() {
		cout << "b" << endl;
		init();//因为成员函数调用的本质是this指针，所以在自己内部调用成员函数也会发生动态绑定
	}
	void func() {
		std::cout << "b" << std::endl;
	}
private: //访问限定符不影响虚函数的动态绑定
	void print() {
		cout << "bPrint" << endl;
	}
};
```

C++ 归根结底就是只要你声明了函数我就能去使用，但是只有这个函数真的有实现，链接才会成功。

然后就是虚函数，一般是搭配基类指针使用。基类指针所指向的对象实际上是派生类的对象，所以所有基类的函数，只要会被虚函数替换的，都会替换。

#### 基类、派生类的构造顺序

```c++
struct c {
	c(int in) {
		a = in;
		cout << "c" << endl;
	}
	int a = 11;
};
struct d : public c {
	d(int in) 
		:c(in) // 这里也是先构造基类，只不过用的是带参的构造函数。默认是不带参的
	{
		cout << b;
	}
	int b = a + 1;
};
c* p = new d(2);
```

答案是3。

先按基类声明顺序从左到右构造基类，再构造派生类。

先按成员变量声明顺序从上到下构造成员变量，再调用类自身构造函数。

### 4、函数定义和声明不兼容

没加命名空间。有时候编译器不报错。

### 5、访问限定符和继承

```c++
class a {
public:
	void Init()
	{
		cout << "Init" << endl;
		initgl();
	}
protected:
	virtual void initres() {
		cout << "initres" << endl;
	}
private:
	void initgl() {
		cout << "initgl" << endl;
		initres();
	}
};

class b : public a {
	void initres() override {
		cout << "binitres" << endl;
	}
};
int main() {
	b ins;
	ins.Init();
	return 0;
}
```

输出：

init

initgl

binitres

事实一：只要这个函数是定义在基类下的，那么这个函数即使被继承了它的作用域还是基类。

事实二：虚函数无视作用域限定符，只要同名函数，基类的虚函数就会被覆盖（其实也很好理解，因为派生类覆写后的虚函数作用域可能改变）

事实三：一个派生类对象调用的所有虚函数，都是被自己的虚函数覆盖过的。（如果不加基类作用域限定）无论这个函数调用是发生在基类的函数中还是自己的。

**纯虚函数**

纯虚函数如果实现了，还是可以在子类中通过作用域限定符去访问的，如果没实现但去调用了，链接就会报错了。所以因为不知道纯虚函数到底实现没有，尽量还是不要访问。不要实现。



### 6、hash表和树的取舍（map和unordered_map）

stl里的hash表，有质数个篮子（对素数取余不容易重复，当然如果有很好的hash函数这个大小就没那么重要了），元素如果大于质数，就扩充一倍（rehash）。如果重复，就使用线性表，所以如果数据不多的话，篮子很少，重复可能很多。

所以小数据用map，大数据可以用unordered_map。

另外自定义类型，map需要定义operator<，hash需要定义hash函数和operator==。

### 7、default copy assignment operator 、default constructor、default copy constructor

**Deleted implicitly-declared copy assignment operator**

A implicitly-declared copy assignment operator for class `T` is defined as *deleted* if any of the following is true:

- `T` has a user-declared move constructor;
- `T` has a user-declared move assignment operator.

Otherwise, it is defined as defaulted.

A defaulted copy assignment operator for class `T` is defined as *deleted* if any of the following is true:

- `T` has a non-static data member of non-class type (or array thereof) that is const;
- `T` has a non-static data member of a reference type;
- `T` has a non-static data member or a direct or virtual base class that cannot be copy-assigned (overload resolution for the copy assignment fails, or selects a deleted or inaccessible function);
- `T` is a [union-like class](https://en.cppreference.com/w/cpp/language/union#Union-like_classes), and has a variant member whose corresponding assignment operator is non-trivial.



**Deleted implicitly-declared default constructor**

The implicitly-declared or defaulted (since C++11) default constructor for class `T` is undefined (until C++11)defined as deleted (since C++11) if any of the following is true:

- `T` has a member of reference type without a default initializer. (since C++11)

- `T` has a non-[const-default-constructible](https://en.cppreference.com/w/cpp/language/default_initialization#Default_initialization_of_a_const_object) const member without a default member initializer (since C++11).

- `T` **has a member (without a default member initializer) (since C++11) which has a deleted default constructor**, or its default constructor is ambiguous or inaccessible from this constructor.

- `T` has a direct or virtual base which has a deleted default constructor, or it is ambiguous or inaccessible from this constructor.

- `T` has a direct or virtual base which has a deleted destructor, or a destructor that is inaccessible from this constructor.

  `T` is a union with at least one variant member with non-trivial default constructor, and no variant member of `T` has a default member initializer.`T` is a non-union class with a variant member `M` with a non-trivial default constructor, and no variant member of the anonymous union containing `M` has a default member initializer.  (since C++11)

- `T` is a union and all of its variant members are const.

  If no user-defined constructors are present and the implicitly-declared default constructor is not trivial, the user may still inhibit the automatic generation of an implicitly-defined default constructor by the compiler with the keyword `delete`.

简单来说就是如果引用成员就无法进行隐式赋值，如果成员有的没有默认构造那么就无法构建默认构造。但是有的成员没有默认构造但是赋值操作还是可以进行，讲实话这个设置还是有一些迷惑的。



**Deleted implicitly-declared copy constructor**

- `T` has non-static data members that cannot be copied (have deleted, inaccessible, or ambiguous copy constructors);
- `T` has direct or virtual base class that cannot be copied (has deleted, inaccessible, or ambiguous copy constructors);
- `T` has direct or virtual base class with a deleted or inaccessible destructor;

* `T` is a union-like class and has a variant member with non-trivial copy constructor;
* `T` has a data member of **rvalue reference type**;
* `T` has a user-defined [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) or [move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment) (this condition only causes the implicitly-declared, not the defaulted, copy constructor to be deleted).



**When to overload them？**

一般是成员中有引用或者指针（动态分配了内存）的时候。而且这里有个0/3/5原则，就是要重载就把destructor, copy constructor, copy assignment operator(move ctor, move assignment )一起重载了。

如果成员用的是智能指针也可以不用改，智能指针会自己帮你管理内存，所以不需要覆写这个deconstructor，所以就万事大吉。

**move constructor**

move constructor一般是在传参有临时变量的时候用到，这个时候就会将临时变量的内存直接给到新的变量上，然后临时变量的内存就变为空（临时变量的内存被“偷”了）。如果是生命期还存在的变量那么就会直接调用复制构造。

另外在函数传非引用传参进去一个临时变量时，编译器可能会优化，然后不调用复制构造，直接用这个临时变量当参数。



### 8、copy constructor and assignment  operator

A **copy constructor** is used to initialize a **previously uninitialized** object from some other object's data.

```cpp
A(const A& rhs) : data_(rhs.data_) {}
```

For example:

```cpp
A aa;
A a = aa;  //copy constructor
```

An **assignment operator** is used to replace the data of a **previously initialized** object with some other object's data.

```cpp
A& operator=(const A& rhs) {data_ = rhs.data_; return *this;}
```

For example:

```cpp
A aa;
A a;
a = aa;  // assignment operator
```

You could replace copy construction by default construction plus assignment, but that would be less efficient.

(As a side note: My implementations above are exactly the ones the compiler grants you for free, so it would not make much sense to implement them manually. If you have one of these two, it's likely that you are manually managing some resource. In that case, per *[The Rule of Three](https://stackoverflow.com/questions/4172722/what-is-the-rule-of-three)*, you'll very likely also need the other one plus a destructor.)



### 9、unique_ptr, shared_ptr, weak_ptr

unique_ptr只能绑定一个对象，使用private使得复制构造和赋值构造无法被访问。

shared_ptr使用引用计算来管理内存，但是有以下缺点：

1. 内部变量暴露，使得独立生成的shared_ptr指向同一块内存，或者对于独立分配的对象在局部作用域内转换为shared_ptr。总而言之就是混用一般的指针和智能指针。
2. 循环引用，那么这个对象永远不会被释放。
3. 在多线程时，需要保证引用计数增加是线程安全的。
4. 比普通指针占用更多开销。

另外，如果觉得std::shared_ptr<classname>这样名字太长可以使用typedef。
