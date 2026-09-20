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
public:
	class const_iterator;
	class iterator {
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
		iterator operator+=(const int &n) {
			//TODO
			if(my_deque == nullptr || index + n < 0 || index + n > my_deque->size()) throw invalid_iterator();
			index += n;
			return *this;
		}
		iterator operator-=(const int &n) {
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
			iterator tmp = *this;
			tmp -= 1;
			return tmp;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const {}
		/**
		 * TODO it->field
		 */
		T* operator->() const noexcept {}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {}
		bool operator==(const const_iterator &rhs) const {}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {}
		bool operator!=(const const_iterator &rhs) const {}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
		public:
			const_iterator() {
				// TODO
			}
			const_iterator(const iterator &other) {
				// TODO
			}
			const_iterator operator+(const int &n) const {
				//TODO
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
			const_iterator operator+=(const int &n) {
				//TODO
				if(my_deque == nullptr || index + n < 0 || index + n >= my_deque->size()) throw invalid_iterator();
				index += n;
				return *this;
			}
			const_iterator operator-=(const int &n) {
				//TODO
				if(my_deque == nullptr || index - n < 0 || index - n >= my_deque->size()) throw invalid_iterator();
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
			const T& operator*() const {}
			const T* operator->() const noexcept {}
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
	deque() {}
	deque(const deque &other) : data(other.data) , current_size(other.current_size) , current_capacity(other.current_capacity) {}
	/**
	 * TODO Deconstructor
	 */
	~deque() {}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {}
	const T & at(const size_t &pos) const {}
	T & operator[](const size_t &pos) {}
	const T & operator[](const size_t &pos) const {}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {}
	const_iterator cbegin() const {}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {}
	const_iterator cend() const {}
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
		destroy(data, current_size);
		::operator delete(data);
		data = nullptr;
		current_size = 0;
		current_capacity = 0;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {}
};

}

#endif
