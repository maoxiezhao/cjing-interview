#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

template <typename T, typename AllocatorT>
class List
{
private:
	AllocatorT mAllocator;

	struct ListImpl;
	struct ListElement
	{
	public:
		T mValue;
		ListElement* mPrev = nullptr;
		ListElement* mNext = nullptr;
		ListImpl* mImpl = nullptr;

	public:
		const T& operator*() const { return mValue; };
		const T* operator->() const { return &mValue; };
		T& operator*() { return mValue; };
		T* operator->() { return &mValue; };

		const ListElement* next()const { return mNext;}
		ListElement* next() { return mNext; }
		const ListElement* prev()const { return mPrev; }
		ListElement* prev() { return mPrev; }

		void erase()
		{
			mImpl->erase(this);
		}
	};

	struct ListImpl
	{
		ListElement* mFirst = nullptr;
		ListElement* mLast = nullptr;
		size_t mSize = 0;
		ContainerAllocator& mAllocator;

		ListImpl(ContainerAllocator& allocator) :
			mAllocator(allocator) {}

		bool erase(ListElement* it)
		{
			ERR_FAIL_COND_V(!it, false);
			ERR_FAIL_COND_V(it->mImpl != this, false);

			if (mFirst == it) {
				mFirst = it->mNext;
			}
			if (mLast == it) {
				mLast = it->mPrev;
			}
			if (it->mPrev) {
				it->mPrev->mNext = it->mNext;
			}
			if (it->mNext) {
				it->mNext->mPrev = it->mPrev;
			}

			ALLOCATOR_DELETE(mAllocator, it);
			mSize--;

			return true;
		}
	};
	ListImpl* mImpl = nullptr;

public:
	List() = default;
	List(const List& rhs)
	{
		const auto it = rhs.front();
		while (it)
		{
			push_back(*it);
			it = it->next();
		}
	}

	List(List&& rhs)
	{
		std::swap(mImpl, rhs.mImpl);
		std::swap(mAllocator, rhs.mAllocator);
	}
	~List()
	{
		clear();
		if (mImpl != nullptr) {
			ALLOCATOR_DELETE(mAllocator, mImpl);
		}
	}

	void operator=(const List& rhs)
	{
		clear();
		const auto it = rhs.front();
		while (it)
		{
			push_back(*it);
			it = it->next();
		}
	}
	void operator=(List&& rhs)
	{
		std::swap(mImpl, rhs.mImpl);
		std::swap(mAllocator, rhs.mAllocator);
	}

	ListElement* push_back(const T& value)
	{
		if (!mImpl) {
			mImpl = ALLOCATOR_NEW(mAllocator, ListImpl)(mAllocator);
		}

		ListElement* newElem = ALLOCATOR_NEW(mAllocator, ListElement);
		newElem->mValue = (T&)value;
		newElem->mPrev = mImpl->mLast;
		newElem->mImpl = mImpl;

		// set last ptr
		if (mImpl->mLast) {
			mImpl->mLast->mNext = newElem;
		}
		mImpl->mLast = newElem;

		// set first ptr
		if (!mImpl->mFirst) {
			mImpl->mFirst = newElem;
		}

		mImpl->mSize++;

		return newElem;
	}

	ListElement* push_front(const T& value)
	{
		if (!mImpl) {
			mImpl = ALLOCATOR_NEW(mAllocator, ListImpl)(mAllocator);
		}

		ListElement* newElem = ALLOCATOR_NEW(mAllocator, ListElement);
		newElem->mValue = (T&)value;
		newElem->mNext = mImpl->mFirst;
		newElem->mImpl = mImpl;

		// set first ptr
		if (mImpl->mFirst) {
			mImpl->mFirst->mPrev = newElem;
		}
		mImpl->mFirst = newElem;

		// set last ptr
		if (!mImpl->mLast) {
			mImpl->mLast = newElem;
		}

		mImpl->mSize++;

		return newElem;
	}

	ListElement* insert_after(ListElement* target, const T& value)
	{
		assert(mImpl && target->mImpl == this);

		if (!target) {
			return push_back(value);
		}

		ListElement* newElem = ALLOCATOR_NEW(mAllocator, ListElement);
		newElem->mValue = (T&)value;
		newElem->mPrev = target;
		newElem->mNext = target->mNext;
		newElem->mImpl = mImpl;

		if (target->mNext == nullptr) {
			mImpl->mLast = newElem;
		}
		else {
			target->mNext->mPrev = newElem;
		}

		target->mNext = newElem;
		mImpl->mSize++;

		return newElem;
	}

	ListElement* insert_before(ListElement* target, const T& value)
	{
		assert(mImpl && target->mImpl == this);

		if (!target) {
			return push_back(value);
		}

		ListElement* newElem = ALLOCATOR_NEW(mAllocator, ListElement);
		newElem->mValue = (T&)value;
		newElem->mPrev = target->mPrev;
		newElem->mNext = target;
		newElem->mImpl = mImpl;

		if (target->mPrev == nullptr) {
			mImpl->mFirst = newElem;
		}
		else {
			target->mPrev->mNext = newElem;
		}

		target->mPrev = newElem;
		mImpl->mSize++;

		return newElem;
	}

	void pop_back()
	{
		if (mImpl && mImpl->mLast) {
			erase(mImpl->mLast);
		}
	}

	void pop_front()
	{
		if (mImpl && mImpl->mFirst) {
			erase(mImpl->mFirst);
		}
	}

	ListElement* find(const T& val)
	{
		auto it = front();
		while (it != nullptr)
		{
			if (it->mValue == val) {
				return it;
			}
			it = it->next();
		}
		return nullptr;
	}

	void clear()
	{
		auto it = front();
		while (it != nullptr)
		{
			erase(it);
			it = front();
		}
	}

	bool erase(ListElement* it)
	{
		if (mImpl != nullptr){
			return mImpl->erase(it);
		}
		return false;
	}

	bool erase(const T& value)
	{
		auto it = find(value);
		return erase(it);
	}

	bool empty()const {
		return (!mImpl || mImpl->mSize <= 0);
	}

	size_t size()const {
		return mImpl ? mImpl->mSize : 0;
	}

	T& operator[](int index) 
	{
		int curIndex = 0;
		auto it = front();
		while (it != nullptr)
		{
			if (curIndex == index) {
				return *it;
			}

			it = it->next();
			curIndex++;
		}
		assert(false);
	}

	const T& operator[](int index)const
	{
		int curIndex = 0;
		auto it = front();
		while (it != nullptr)
		{
			if (curIndex == index) {
				return *it;
			}

			it = it->next();
			curIndex++;
		}
		assert(false);
	}

	ListElement* front() { return mImpl ? mImpl->mFirst : nullptr; }
	ListElement* back() { 
		return mImpl ? mImpl->mLast : nullptr; 
	}
	const ListElement* front() const { return mImpl ? mImpl->mFirst : nullptr; }
	const ListElement* back() const { mImpl ? mImpl->mLast : nullptr; }

	// iterator
	template<typename ValueT>
	class ListIteratorBase
	{
	public:
		ListIteratorBase(ListElement* element) : mElement(element) {}

		bool operator!= (const ListIteratorBase<ValueT>& other) {
			return mElement != other.mElement;
		}
		bool operator== (const ListIteratorBase<ValueT>& other) {
			return mElement == other.mElement;
		}

		ListIteratorBase& operator++()
		{
			if (mElement != nullptr) {
				mElement = mElement->next();
			}
			return *this;
		}

		ListIteratorBase& operator++(int)
		{
			if (mElement != nullptr) {
				mElement = mElement->next();
			}
			return *this;
		}

		ValueT& operator*() {
			return mElement->mValue;
		}
		ValueT* operator->() {
			return &mElement->mValue;
		}
		const ValueT& operator*()const {
			return mElement->mValue;
		}
		const ValueT* operator->()const {
			return &mElement->mValue;
		}

		ValueT& value() const
		{
			assert(mElement != nullptr);
			return mElement->mValue;
		}

		ValueT& value()
		{
			assert(mElement != nullptr);
			return mElement->mValue;
		}


	private:
		ListElement* mElement = nullptr;
	};

	using iterator = ListIteratorBase<T>;
	using constIterator = ListIteratorBase<const T>;

	iterator begin() { return iterator(front()); }
	iterator end() { return iterator(nullptr); }
	constIterator begin() const { return constIterator(front());  }
	constIterator end() const { return constIterator(nullptr); }
};
