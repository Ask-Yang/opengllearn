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

smart pointer should be used in the case that you own this object,

the reference should be used in the case that you don't own this object, and you won't change the reference, and can not be null.

raw pointer for object you don't own but you want to change it in some cases. The better implementation for that case maybe use polymorphism（多态）.

总而言之，内存分配的时候用智能指针，如果是需要一个对象，那么用引用。能不用就不用吧。指针最大的问题就是它可以为空吧，所以我们要避免制造一个不知道是否存在的怪物，以降低程序出错的可能性，为此，如果真到必须使用指针的时候，也就是允许nullptr的时候，我们应该首先检查我们的类设计才对。给每个对象一个明确的生命期，可能是使用C++时最应该注意的事情。

java的reference可以更改？java是定义一个引用，然后必须要在构造函数里初始化，只不过可以放在构造函数体里，而不是使用初始化列表。