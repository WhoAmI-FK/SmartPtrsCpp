#include <iostream>
#include <cstddef>

namespace smart_pointers {
	template<typename T, typename D = std::default_delete<T>>
	class unique_ptr {
	public:
		typedef T _val_type;
		typedef D _deleter_type;
		typedef unique_ptr<T, D> uniq;
		typedef T* pointer;
		explicit unique_ptr(pointer data)
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
		explicit operator bool() const {
			return _ptr != nullptr;
		}
	private:
		pointer _ptr;
		_deleter_type _def_del;
	};

	template<typename T> 
	struct _Unique_if {
		typedef unique_ptr<T> _object;
	};

	// to add array variation of unique_ptr
	template<typename T> 
	struct _Unique_if<T[]> {
		typedef unique_ptr<T[]> _Unknown_bound;
	};
	template<typename T, size_t N>
	struct _Unique_if<T[N]> {
		typedef void _Known_bound;
	};

	template<typename T, typename... Args>
	typename _Unique_if<T>::_object
		make_unique(Args&&... args) {
		return unique_ptr<T>(new T(std::forward<Args>(args)...));
	}

	template<typename T, typename D = std::default_delete<T>>
	class shared_ptr {
	public:
		typedef T _val_type;
		typedef D _deleter_type;
		typedef shared_ptr<T, D> shared;
		typedef T* pointer;
		typedef size_t* counterType;
		typedef size_t size_type;
		constexpr shared_ptr() noexcept
			:_ptr(nullptr),
			_count(nullptr)
		{

		}
		constexpr shared_ptr(std::nullptr_t nuptr)
			: _ptr(nullptr),
			_count(new size_t(0))
		{

		}
		explicit shared_ptr(pointer p)
			: _ptr(p),
			_count(new size_t(1))
		{
			
		}
		shared_ptr(const shared& rhs) noexcept {
			if (rhs._count != nullptr && rhs._ptr != nullptr) {
				_count = rhs._count;
				(*_count)++;
				_ptr = rhs._ptr;
			}
			else {
				_ptr = nullptr;
				_count = nullptr;
			}
		}
		~shared_ptr() {
			if (!unique()) {
				(*_count)--;
				_ptr = nullptr;
			}
			else if (unique()) {
				_def_del(_ptr);
				std::default_delete<size_t>(_count);
			}
			else if ((*_count) == 0) {
				_ptr = nullptr;
			}
		}
		shared& operator=(const shared& rhs) noexcept {
			if (unique()) {
				_def_del(_ptr);
				std::default_delete<size_t>(_count);
			}
			else {
				(*_count)--;
			}
			if (rhs._count != nullptr && rhs._ptr != nullptr) {
				_count = rhs._count;
				(*_count)++;
				_ptr = rhs._ptr;
			}
			else {
				_ptr = nullptr;
				_count = nullptr;
			}
		}
		pointer get() const noexcept {
			return _ptr;
		}
		explicit operator bool() const noexcept {
			return _ptr != nullptr;
		}
		pointer operator->() const noexcept {
			return _ptr;
		}
		bool unique() const noexcept {
			return (_count != nullptr && _ptr != nullptr) && ((*(_count)) == 1);
		}
		size_type use_count() const noexcept {
			return (*_count == nullptr ? 0 : *_count);
		}
		void reset() noexcept {
			if (unique()) {
				_def_del(_ptr);
				std::default_delete<size_t>(_count);
			}
			else if (_count != nullptr && _ptr != nullptr) {
				(*_count)--;
			}
			_count = nullptr;
			_ptr = nullptr;
		}
	private:
		counterType _count;
		_deleter_type _def_del;
		pointer _ptr;
		template<typename T, typename D>
		friend class weak_ptr;
	};

	
	template<typename T, typename D = std::default_delete<T>>
	class weak_ptr {
	public:
		typedef T _val_type;
		typedef D _deleter_type;
		typedef weak_ptr<T, D> weak;
		typedef T* pointer;
		weak_ptr() = default;

	private:

	};
};

int main()
{
	smart_pointers::unique_ptr<int> mptr = smart_pointers::make_unique<int>();
	return 0;
}