#include "EditMe.cpp"
#include <iostream>

struct LeakTester {
	bool& destroyed;
	LeakTester(bool& b) : destroyed{b} {}
	void callTest() {}
	~LeakTester() { destroyed = true; }
};

using namespace CppLondon;
using UPtr = CppLondon::unique_ptr<LeakTester>;

int main() {
	auto canary = false;
	UPtr{new LeakTester{canary}};
	if (!canary)
		std::cerr << "ERROR: Deletion test failed" << std::endl;
	if (UPtr{}.get() != nullptr)
		std::cerr << "ERROR: Default constructed object is not nullptr" << std::endl;
	if (std::is_copy_constructible<UPtr>::value)
		std::cerr << "ERROR: unique_ptr is copy constructible." << std::endl;
	if (std::is_copy_assignable<UPtr>::value)
		std::cerr << "ERROR: unique_ptr is copy assignable." << std::endl;
	canary = false;
	UPtr a{new LeakTester{canary}};
	UPtr b{std::move(a)};
	if (b.get() == nullptr)
		std::cerr << "ERROR: move constructor does not take pointer from old object" << std::endl;
	if (!b)
		std::cerr << "ERROR: operator bool does not return true when holding an object" << std::endl;
	if (a.get() != nullptr)
		std::cerr << "ERROR: move constructor does not reset old object's pointer." << std::endl;
	if (a)
		std::cerr << "ERROR: operator bool does not return false when there is nothing held" << std::endl;
	a = std::move(b);
	if (b.get() != nullptr)
		std::cerr << "ERROR: move assignment operator does not take pointer from old object" << std::endl;
	if (a.get() == nullptr)
		std::cerr << "ERROR: move assignment operator does not reset old object's pointer." << std::endl;
	a.reset(nullptr);
	if (!canary)
		std::cerr << "ERROR: reset does not destroy old object." << std::endl;
	canary = false;
	a.reset(new LeakTester{canary});
	auto toDel = a.release();
	if (toDel == nullptr)
		std::cerr << "ERROR: release does not give the pointer to the caller" << std::endl;
	if (canary)
		std::cerr << "ERROR: release destroys the underlying object" << std::endl;
	delete toDel;
	a.get()->callTest();
	a->callTest();
	(*a).callTest();
	std::cout << "If you don't see any ERROR messages, congratulations!" << std::endl;
	return 0;
}
