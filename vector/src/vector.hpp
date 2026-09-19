#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
private:
	T *data;
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
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	friend class vector;
	friend class const_iterator;
	
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		vector *my_vector;
		std::ptrdiff_t index;
		iterator(vector *vec, std::ptrdiff_t idx) : my_vector(vec), index(idx) {}
		
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator() : my_vector(nullptr), index(0) {}
		iterator operator+(const int &n) const
		{
			//TODO
			iterator tmp = *this;
			tmp += n;
			return tmp;
		}
		iterator operator-(const int &n) const
		{
			//TODO
			iterator tmp = *this;
			tmp -= n;
			return tmp;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			//TODO
			if(my_vector != rhs.my_vector || my_vector == nullptr) throw invalid_iterator();
			return index - rhs.index;
		}

		iterator& operator+=(const int &n)
		{
			//TODO
			if(my_vector == nullptr || index + n < 0 || index + n > my_vector->size()) throw invalid_iterator();
			index += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			//TODO
			if(my_vector == nullptr || index - n < 0 || index - n > my_vector->size()) throw invalid_iterator();
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
		const T& operator*() const{
			if(my_vector == nullptr || index < 0 || index >= my_vector->size()) throw invalid_iterator();
			return my_vector->data[index];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return my_vector == rhs.my_vector && index == rhs.index;
		}
		bool operator==(const const_iterator &rhs) const {
			return my_vector == rhs.my_vector && index == rhs.index;
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
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	friend class vector;
	friend class iterator;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
		const_iterator() : my_vector(nullptr), index(0) {}
		const_iterator(const vector *vec, std::ptrdiff_t idx) : my_vector(vec), index(idx) {}
		const_iterator(const iterator &it) : my_vector(it.my_vector), index(it.index) {}
		const_iterator operator+(const int &n) const
		{
			//TODO
			const_iterator tmp = *this;
			tmp += n;
			return tmp;
		}
		const_iterator operator-(const int &n) const
		{
			//TODO
			const_iterator tmp = *this;
			tmp -= n;
			return tmp;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
			//TODO
			if(my_vector != rhs.my_vector || my_vector == nullptr) throw invalid_iterator();
			return index - rhs.index;
		}

		const_iterator& operator+=(const int &n)
		{
			//TODO
			if(my_vector == nullptr || index + n < 0 || index + n > my_vector->size()) throw invalid_iterator();
			index += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			//TODO
			if(my_vector == nullptr || index - n < 0 || index - n > my_vector->size()) throw invalid_iterator();
			index -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			const_iterator tmp = *this;
			++(*this);
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
			return *this += 1;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			const_iterator tmp = *this;
			--(*this);
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
			return *this -= 1;
		}
		/**
		 * TODO *it
		 */
		const T& operator*() const{
			if(my_vector == nullptr || index < 0 || index >= my_vector->size()) throw invalid_iterator();
			return my_vector->data[index];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const const_iterator &rhs) const {
			return my_vector == rhs.my_vector && index == rhs.index;
		}
		bool operator==(const iterator &rhs) const {
			return my_vector == rhs.my_vector && index == rhs.index;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(rhs == *this);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(rhs == *this);
		}
	private:
		/*TODO*/
		const vector *my_vector;
		std::ptrdiff_t index;
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : data(nullptr), current_size(0), current_capacity(0) {}
	vector(const vector &other) : data(allocate(other.current_capacity)), current_size(0), current_capacity(other.current_capacity) {
		try {
			for(size_t i = 0; i < other.current_size; ++i) {
				::new (static_cast<void *>(data + i)) T(other.data[i]);
				++current_size;
			}
		} catch(...) {
			destroy(data, current_size);
			::operator delete(data);
			throw;
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		clear();
		::operator delete(data);	
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if(this != &other) {
			vector tmp(other);
			std::swap(data, tmp.data);
			std::swap(current_size, tmp.current_size);
			std::swap(current_capacity, tmp.current_capacity);
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if(pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if(pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if(pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}
	const T & operator[](const size_t &pos) const {
		if(pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if(current_size == 0) throw container_is_empty();
		return data[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if(current_size == 0) throw container_is_empty();
		return data[current_size - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(this, 0);
	}
	const_iterator begin() const {
		return const_iterator(this, 0);
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
	const_iterator end() const {
		return const_iterator(this, current_size);
	}
	const_iterator cend() const {
		return const_iterator(this, current_size);
	}
	/**
	 * checks whether the container is empty
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
		while(current_size > 0) {
			data[--current_size].~T();
		}
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		if(pos.my_vector != this || pos.index > current_size || pos.index < 0) throw invalid_iterator();
		return insert(pos.index, value);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if(ind > current_size) throw index_out_of_bound();
		if(ind == current_size) {
			push_back(value);
			return iterator(this, ind);
		}
		if(current_size == current_capacity) {
			const size_t new_capacity = next_capacity();
			T *new_data = allocate(new_capacity);
			for(size_t i = 0; i < ind; ++i) {
				::new (static_cast<void *>(new_data + i)) T(data[i]);
			}
			::new (static_cast<void *>(new_data + ind)) T(value);
			for(size_t i = ind; i < current_size; ++i) {
				::new (static_cast<void *>(new_data + i + 1)) T(data[i]);
			}
			size_t old_size = current_size;
			destroy(data, old_size);
			::operator delete(data);
			data = new_data;
			current_capacity = new_capacity;
		}
		else {
			T tmp = value;
			for(size_t i = current_size; i > ind; --i) {
				::new (static_cast<void *>(data + i)) T(data[i - 1]);
				data[i - 1].~T();
			}
			::new (static_cast<void *>(data + ind)) T(tmp);
		}
		++current_size;
		return iterator(this, ind);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		if(pos.my_vector != this || pos.index >= current_size || pos.index < 0) throw invalid_iterator();
		return erase(pos.index);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if(ind >= current_size) throw index_out_of_bound();
		for(size_t i = ind; i < current_size - 1; ++i) {
			data[i] = data[i + 1];
		}
		data[--current_size].~T();
		return iterator(this, ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if(current_size == current_capacity) {
			T *new_data = allocate(next_capacity());
			for(size_t i = 0; i < current_size; ++i) {
				::new (static_cast<void *>(new_data + i)) T(data[i]);
			}
			::new (static_cast<void *>(new_data + current_size)) T(value);
			if(data) {
				destroy(data, current_size);
				::operator delete(data);
			}
			data = new_data;
			++current_size;
			current_capacity = next_capacity();
		}
		else {
			::new (static_cast<void *>(data + current_size)) T(value);
			++current_size;
			return;
		}
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if(current_size == 0) throw container_is_empty();
		data[--current_size].~T();
	}
};


}

#endif
