#include <iostream>

namespace smart_pointers {
	template<typename T, typename D = std::default_delete<T>>
	class unique_ptr {
	public:
		typedef T _val_type;
		typedef D _deleter_type;
		typedef unique_ptr<T, D> uniq;
		typedef T* pointer;
		explicit unique_ptr(_val_type* data)
			: _ptr(data)
		{

		}
		explicit unique_ptr()
			:_ptr(nullptr)
		{

		}
		unique_ptr(uniq&& rhs) noexcept
			: _ptr(std::move(rhs._ptr))
		{
			rhs._ptr = nullptr;
		}
		uniq& operator=(uniq&& rhs) noexcept {
			if (this == &rhs) return *this;
			_ptr = std::move(rhs._ptr);
			_def_del = rhs.get_deleter();
			rhs._ptr = nullptr;
			return *this;
		}
		unique_ptr(const uniq&) = delete;
		uniq& operator=(const uniq&) = delete;
		~unique_ptr() {
			if (_ptr != nullptr)
				_deleter_type(_ptr);
		}
		pointer get() const noexcept {
			return _ptr;
		}
		_deleter_type& get_deleter() noexcept {
			return _def_del;
		}
		pointer operator->() const noexcept {
			return _ptr;
		}
		pointer release() noexcept {
			pointer toReturn = _ptr;
			_ptr = nullptr;
			return toReturn;
		}
		void reset(pointer p = pointer()) noexcept {
			_deleter_type(_ptr);
			_ptr = p;
		}
		_val_type& operator*() const {
			return *_ptr;
		}
		operator bool() const {
			return _ptr != nullptr;
		}
	private:
		pointer _ptr;
		_deleter_type _def_del;
	};
};

int main()
{
	// code
	return 0;
}
