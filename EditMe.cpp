#include <utility>
//Your task is to fill in this (not working) CppLondon::unique_ptr<>


//You are not required to handle deleting arrays, but if you are comfortable enough
//with templates, please feel free to do so and I will review it manually.

namespace CppLondon {
	template<typename T>
	class unique_ptr {
	public:
		//our default constructor should make the pointer a nullptr. There are 2 ways to do this.
		unique_ptr() = default;

		//our T* constructor should set our internal pointer.
		unique_ptr(T* p) : ptr{p} {}

		//our copy constructor & assignment operator should be made compile-time unusable.
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;

		//our move constructor & assignment operator should take the pointer and set the original one's to nullptr.
		unique_ptr(unique_ptr&& other) : ptr{other.ptr} { other.ptr = nullptr; }
		unique_ptr& operator=(unique_ptr&& rhs) { reset(rhs.ptr); rhs.ptr = nullptr; return *this; }

		//our destructor should release the pointer.
		~unique_ptr() { reset(nullptr); }

		//our reset() function should destroy the existing object (if any) and then assign the new one.
		void reset(T* ptr) noexcept {delete this->ptr; this->ptr = ptr;}
		
		//our release() function should return the pointer and clear our own.
		T* release() noexcept { auto ret = ptr; ptr = nullptr; return ret; }

		//Implement an operator-> const noexcept that will return the pointer.
		T* operator->() { return ptr; }
		
		//Implement an operator* const that will return a reference to the object that is pointed to.
		T& operator*() { return *ptr; }
		
		//Implement T* get() const noexcept that also returns the pointer.
		T* get() { return ptr; }
		
		//Implement an operator bool const noexcept that checks if we are holding a valid object (return true if yes, otherwise false)
		operator bool() { return ptr != nullptr; }

		//BONUS: Implement a constructor that will take a parameter pack and perfectly forward arguments to construct the pointed-to object.
		template <typename... Args>
		unique_ptr(Args&&... args) : ptr{new T{std::forward<Args>(args)...}} {}
		
	private:
		T* ptr = nullptr;
	};
}
