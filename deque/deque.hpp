#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu { 

template<class T>
class deque {
private:
	// data members.
	T* data;
	size_t current_size;
	size_t current_capacity;
	size_t head;
	static T *allocate(const size_t &capacity) {
		if(capacity == 0) return nullptr;
		if(capacity > static_cast<size_t>(-1) / sizeof(T)) throw runtime_error();
		return static_cast<T *>(::operator new(sizeof(T) * capacity));
	}
	static void destroy(T *ptr, size_t count) {
		while(count > 0) ptr[--count].~T();
	}
	size_t next_capacity() const {
		const size_t max_capacity = static_cast<size_t>(-1) / sizeof(T);
		if(current_capacity >= max_capacity) throw runtime_error();
		if(current_capacity == 0) return 1;
		if(current_capacity > max_capacity / 2) return max_capacity;
		return current_capacity * 2;
	}
	void reallocate(T* old_data, size_t old_capacity, size_t new_capacity) {
		T* new_data = allocate(new_capacity);
		for(size_t i = 0; i < current_size; ++i) {
			::new (static_cast<void *>(new_data + i)) T(old_data[(head + i) % old_capacity]);
		}
		for(size_t i = 0; i < current_size; ++i) {
			old_data[(head + i) % current_capacity].~T();
		}
		::operator delete(old_data);
		data = new_data;
		current_capacity = new_capacity;
		head = 0;
	}
public:
	class const_iterator;
	class iterator {
	friend class deque;
	friend class const_iterator;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		size_t index;
		deque* my_deque;
		iterator(deque *deq, size_t idx) : my_deque(deq), index(idx) {}
		
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator() : my_deque(nullptr), index(0) {}
		iterator operator+(const int &n) const {
			//TODO
			iterator tmp = *this;
			tmp += n;
			return tmp;
		}
		iterator operator-(const int &n) const {
			//TODO
			iterator tmp = *this;
			tmp -= n;
			return tmp;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			//TODO
			if(my_deque != rhs.my_deque || my_deque == nullptr) throw invalid_iterator();
			return index - rhs.index;
		}
		iterator& operator+=(const int &n) {
			//TODO
			if(my_deque == nullptr || index + n < 0 || index + n > my_deque->size()) throw invalid_iterator();
			index += n;
			return *this;
		}
		iterator& operator-=(const int &n) {
			//TODO
			if(my_deque == nullptr || index - n < 0 || index - n > my_deque->size()) throw invalid_iterator();
			index -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp = *this;
			++(*this);
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			return *this += 1;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp = *this;
			--(*this);
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			return *this -= 1;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const {
			if(my_deque == nullptr || index < 0 || index >= my_deque->size()) throw invalid_iterator();
			return my_deque->data[(my_deque->head + index) % my_deque->current_capacity];
		}
		/**
		 * TODO it->field
		 */
		T* operator->() const noexcept {
			return &operator*();
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return my_deque == rhs.my_deque && index == rhs.index;
		}
		bool operator==(const const_iterator &rhs) const {
			return my_deque == rhs.my_deque && index == rhs.index;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}
	};
	class const_iterator {
		friend class deque;
		friend class iterator;
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
			deque* my_deque;
			size_t index;
		public:
			const_iterator() : my_deque(nullptr), index(0) {
				// TODO
			}
			const_iterator(const iterator &other) : my_deque(other.my_deque), index(other.index) {
				// TODO
			}
			const_iterator(const deque *deq, size_t idx) : my_deque(const_cast<deque*>(deq)), index(idx) {
				// TODO
			}
			const_iterator operator+(const int &n) const {
				//TODO
				const_iterator tmp = *this;
				tmp += n;
				return tmp;
			}
			const_iterator operator-(const int &n) const {
				//TODO
				const_iterator tmp = *this;
				tmp -= n;
				return tmp;
			}
			int operator-(const const_iterator &rhs) const {
				//TODO
				if(my_deque != rhs.my_deque || my_deque == nullptr) throw invalid_iterator();
				return index - rhs.index;
			}
			const_iterator& operator+=(const int &n) {
				//TODO
				if(my_deque == nullptr || index + n < 0 || index + n > my_deque->size()) throw invalid_iterator();
				index += n;
				return *this;
			}
			const_iterator& operator-=(const int &n) {
				//TODO
				if(my_deque == nullptr || index - n < 0 || index - n > my_deque->size()) throw invalid_iterator();
				index -= n;
				return *this;
			}
			const_iterator operator++(int) {
				const_iterator tmp = *this;
				++(*this);
				return tmp;
			}
			const_iterator& operator++() {
				return *this += 1;
			}
			const_iterator operator--(int) {
				const_iterator tmp = *this;
				--(*this);
				return tmp;
			}
			const_iterator& operator--() {
				return *this -= 1;
			}
			const T& operator*() const {
				if(my_deque == nullptr || index >= my_deque->current_size) throw invalid_iterator();
				return my_deque->data[(index + my_deque->head) % my_deque->current_capacity];
			}
			const T* operator->() const noexcept {
				if(my_deque == nullptr || index >= my_deque->current_size) throw invalid_iterator();
				return &my_deque->data[(index + my_deque->head) % my_deque->current_capacity];
			}
			bool operator==(const iterator &rhs) const {
				return my_deque == rhs.my_deque && index == rhs.index;
			}
			bool operator==(const const_iterator &rhs) const {
				return my_deque == rhs.my_deque && index == rhs.index;
			}
			bool operator!=(const iterator &rhs) const {
				return !(*this == rhs);
			}
			bool operator!=(const const_iterator &rhs) const {
				return !(*this == rhs);
			}
	};
	/**
	 * TODO Constructors
	 */
	deque() : data(nullptr), current_size(0), current_capacity(0), head(0) {}
	deque(const deque &other) : data(allocate(other.current_capacity)), current_size(0), current_capacity(other.current_capacity), head(0) {
		for(; current_size < other.current_size; ++current_size) {
			::new (static_cast<void *>(data + current_size)) T(other.data[(current_size + other.head) % other.current_capacity]);
		}
	}
	/**
	 * TODO Deconstructor
	 */
	~deque() {
		clear();
		::operator delete(data);
	}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {
		if(this == &other) return *this;
		clear();
		::operator delete(data);
		data = allocate(other.current_capacity);
		current_size = other.current_size;
		current_capacity = other.current_capacity;
		head = 0;
		for(size_t i = 0; i < current_size; ++i) {
			::new (static_cast<void *>(data + i)) T(other.data[(i + other.head) % other.current_capacity]);
		}
		return *this;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
		if(pos >= current_size) throw index_out_of_bound();
		return data[(pos + head) % current_capacity];
	}
	const T & at(const size_t &pos) const {
		if(pos >= current_size) throw index_out_of_bound();
		return data[(pos + head) % current_capacity];
	}
	T & operator[](const size_t &pos) {
		if(pos >= current_size) throw index_out_of_bound();
		return data[(pos + head) % current_capacity];
	}
	const T & operator[](const size_t &pos) const {
		if(pos >= current_size) throw index_out_of_bound();
		return data[(pos + head) % current_capacity];
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
		if(current_size == 0) throw container_is_empty();
		return data[head];
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
		if(current_size == 0) throw container_is_empty();
		return data[(head + current_size - 1) % current_capacity];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(this, 0);
	}
	const_iterator cbegin() const {
		return const_iterator(this, 0);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(this, current_size);
	}
	const_iterator cend() const {
		return const_iterator(this, current_size);
	}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {
		return current_size == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return current_size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for(size_t i = 0; i < current_size; ++i) {
			data[(head + i) % current_capacity].~T();
		}
		::operator delete(data);
		data = nullptr;
		current_size = 0;
		current_capacity = 0;
		head = 0;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {
		if(pos.my_deque != this || pos.index > current_size || pos.index < 0) throw invalid_iterator();
		T tmp = value;
		if(current_size >= current_capacity) {
			const size_t new_capacity = next_capacity();
			reallocate(data, current_capacity, new_capacity);
		}
		for(size_t i = current_size; i > pos.index; --i) {
			::new (static_cast<void *>(data + (head + i) % current_capacity)) T(data[(head + i - 1) % current_capacity]);
			data[(head + i - 1) % current_capacity].~T();
		}
		::new (static_cast<void *>(data + (head + pos.index) % current_capacity)) T(tmp);
		++current_size;
		return iterator(this, pos.index);
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
		if(pos.my_deque != this || pos.index >= current_size || pos.index < 0) throw invalid_iterator();
		if(current_size == 0) throw container_is_empty();
		size_t ind = pos.index;
		for(size_t i = ind; i < current_size - 1; ++i)
			data[(head + i) % current_capacity] = data[(head + i + 1) % current_capacity];
		--current_size;
		data[(head + current_size) % current_capacity].~T();
		return iterator(this, ind);
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {
		T tmp = value;
		if(current_size >= current_capacity) {
			const size_t new_capacity = next_capacity();
			reallocate(data, current_capacity, new_capacity);
		}
		::new (static_cast<void *>(data + (head + current_size) % current_capacity)) T(tmp);
		++current_size;
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
		if(current_size == 0) throw container_is_empty();
		data[(head + current_size - 1) % current_capacity].~T();
		--current_size;
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {
    	T tmp = value;

    	if(current_size >= current_capacity) {
        	const size_t new_capacity = next_capacity();
	        reallocate(data, current_capacity, new_capacity);
	    }

    	size_t new_head = (head + current_capacity - 1) % current_capacity;
    	::new (static_cast<void *>(data + new_head)) T(tmp);
    	head = new_head;
    	++current_size;
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
		if(current_size == 0) throw container_is_empty();
		data[head].~T();
		head = (head + 1) % current_capacity;
		--current_size;
		if(current_size == 0) head = 0;
	}
};

}

#endif
