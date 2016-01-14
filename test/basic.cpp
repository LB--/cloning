#include "cloning.hpp"

struct Test
: LB::cloning::CloneImplementor<Test>
{
	Test() noexcept = default;

protected:
	Test(Test const &) noexcept = default;

private:
	virtual Test *clone() const noexcept override
	{
		return new Test(*this);
	}
};

int main()
{
	Test t;
	auto c1 = Test::Clone<>(t);
	auto c2 = Test::Clone<std::shared_ptr>(t);
	auto c3 = Test::Clone<LB::cloning::ClonePtr>(*c1);
	auto c4 = c3;
}
