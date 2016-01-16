cloning [![travis](https://travis-ci.org/LB--/cloning.svg?branch=cloning)](https://travis-ci.org/LB--/cloning)
=======
This library is my personal take on the clone pattern. `CloneImplementor<>` uses the CRTP to help reduce boilerplate. `ClonePtr` is a copyable smart pointer for cloneable types.

_This library requires that your compiler support C++1z (the C++ standard after C++14)_

## Usage
### CMake
From the `cmake` directory, copy the `FindLB` directory to a place in your `CMAKE_MODULE_PATH`.
Then, add `find_package(LB/cloning REQUIRED)` to your CMake script.
You may need to set the CMake variable `LB/optional_ROOT` if you installed to a nonstandard location.
Finally, link to the `LB::cloning` imported target with `target_link_libraries()`.

### C++
`#include <LB/cloning/cloning.hpp>`

#### Making a class cloneable
If you want to make a class cloneable, it and all its derived classes should derive from `CloneImplementor<>`.
The template parameter is your deriving class, e.g. `CloneImplementor<MyClass>`.

Once you have derived from `CloneImplementor<>`, you need to override the private virtual member function `clone()` (or omit it for abstract classes).
All `CloneImplementor<>` does for you is provide the static `Clone<>()` member function which is used to properly clone a class.

Example cloneable class:
```cpp
struct MyClass
: LB::cloning::CloneImplementor<MyClass>
{
protected:
	MyClass(MyClass const &from) noexcept;
private:
	virtual MyClass *clone() const noexcept override
	{
		return new MyClass(*this);
	}
};
```
You need to properly implement the protected copy constructor to account for parent classes and deriving classes.
Note that abstract classes do not need to implement the `clone()` member function as it will never (should never) be used.

#### Cloning cloneable types
To properly clone an instance of a cloneable class, you need to call the static `Clone` member function from the class type you want the resulting clone to be accessible through:
```cpp
void f(SomeDerivedType const &v) noexcept
{
	auto clone0 = SomeDerivedType::Clone<>(v); //unique_ptr<SomeDerivedType>
	auto clone1 = SomeParentType::Clone<std::shared_ptr>(v); //shared_ptr<SomeParentType>

	g(*clone0);
	h(std::move(clone1));
}
```
