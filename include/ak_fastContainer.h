/*
 * ak_fastContainer.cpp
 *
 *  Created on: September 22, 2020
 *	Last modified on: September 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

#include <vector>	// vector class
#include <map>

namespace ak {

	//! This structure is used by the fast container to store the information
	template <class T>
	struct fastContainerItem
	{
		fastContainerItem<T> *	next;
		T						value;
	};

	//! This class realizes the functionallity of a list and provides a fast convertation to a std::vector.
	//! The use this container for collecting information with an undefined number of elements which then can be accessed via the std::vector output.
	template <class T> class __declspec(dllexport) fastContainer {
	public:
		//! @brief Default constructor
		fastContainer();

		// @brief Deconstructor
		virtual ~fastContainer();

		//! @brief Will add a new element to the back of the container
		void add(T _v);

		//! @brief Will erase the provided item from the container
		//! @param _v The value to erase
		//! @param _eraseDuplicates If true, then all values that are equal will be erased
		void erase(T _v, bool _eraseDuplicates = true);

		//! @brief Will clear the container
		void clear(void);

		//! @brief Will return the ammount of elements in the container
		int count(void) const;

		//! @brief Will write and return the stored information in a std::vector
		const std::vector<T> & toVector(void);

		//! @brief Will remove all duplicates from the container (keeping the order is not guaranteed
		void clearDuplicates();

	private:

		fastContainer(fastContainer<T> &) = delete;
		fastContainer<T> & operator = (fastContainer<T>) = delete;
		fastContainer<T> & operator = (fastContainer<T> &) = delete;

		fastContainerItem<T> *	my_first;		//! The first element
		fastContainerItem<T> *	my_last;		//! The last (always empty) element
		int						my_count;		//! The ammount of elements in this container
		std::vector<T>			my_vector;		//! The vector created by this container
		fastContainerItem<T> *	my_lastAddress;	//! The address of the last the last item when the vector was created
	};


}

// ################################################################

template<class T>
ak::fastContainer<T>::fastContainer()
	:my_count(0), my_first(nullptr), my_last(nullptr), my_lastAddress(false)
{
	// Create the very last element that always exists
	my_first = new fastContainerItem<T>();
	my_first->next = nullptr;
	my_last = my_first;
	my_lastAddress = my_last;
}

template<class T>
ak::fastContainer<T>::~fastContainer() {
	fastContainerItem<T> * itm = my_first;
	while (itm != nullptr) {
		fastContainerItem<T> * current = itm;
		itm = itm->next; delete current;
	}
	my_first = nullptr; my_last = nullptr; my_lastAddress = nullptr;
}

template<class T>
void ak::fastContainer<T>::add(T _v) {
	my_last->value = _v;
	my_last->next = new fastContainerItem<T>;
	my_last = my_last->next;
	my_last->next = nullptr;
	my_count++;
}

template<class T>
void ak::fastContainer<T>::erase(T _v, bool _eraseDuplicates) {
	fastContainerItem<T> * itm = my_first;
	if (itm == nullptr) { return; }
	fastContainerItem<T> * prev = itm;
	while (itm->next != nullptr) {
		if (itm->value == _v) {
			if (itm == prev) {
				itm = itm->next;
				delete prev;
				prev = itm;
				my_first = itm;
				my_count--;
			}
			else {
				prev->next = itm->next;
				delete itm;
				itm = prev;
				prev = itm;
				itm = itm->next;
				my_count--;
			}
			if (!_eraseDuplicates) { return; }
		}
		else { prev = itm; itm = itm->next; }
	}
	my_lastAddress = nullptr;
}

template<class T>
void ak::fastContainer<T>::clear() {
	fastContainerItem<T> * itm = my_first;
	while (itm->next != nullptr) {
		fastContainerItem<T> * c = itm;
		itm = itm->next;
		delete c;
	}
	my_first = itm;
	my_last = itm;
	my_lastAddress = my_last;
	my_vector.clear();
	my_count = 0;
}

template <class T>
int ak::fastContainer<T>::count(void) const { return my_count; }

template <class T>
const std::vector<T> & ak::fastContainer<T>::toVector(void) {
	if (my_lastAddress == my_last) { return my_vector; }
	my_vector.clear();
	my_vector.reserve(my_count);
	fastContainerItem<T> * itm = my_first;
	while (itm->next != nullptr) { my_vector.push_back(itm->value); itm = itm->next; }
	my_lastAddress = my_last;
	return my_vector;
}

template <class T>
void ak::fastContainer<T>::clearDuplicates() {
	std::map<T, bool> m;
	const std::vector<T> & v = toVector();
	for (size_t i = 0; i < v.size(); i++) { m.insert_or_assign(v.at(i), false); }
	clear();
	typedef std::map<T, bool>::iterator it;
	for (it itm = m.begin(); itm != m.end(); itm++) { add(itm->first); my_count++; }
}