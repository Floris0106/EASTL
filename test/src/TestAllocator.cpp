/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "EASTLTest.h"
#include <EASTL/allocator.h>
#include <EASTL/fixed_allocator.h>
#include <EASTL/list.h>
#include <EAStdC/EAString.h>
#include <EAStdC/EAAlignment.h>



///////////////////////////////////////////////////////////////////////////////
// fixed_pool_reference
//
struct fixed_pool_reference
{
public:
	fixed_pool_reference(const char* = NULL)
	{
		mpFixedPool = NULL;
	}

	fixed_pool_reference(eastl::fixed_pool& fixedPool)
	{
		mpFixedPool = &fixedPool;
	}

	fixed_pool_reference(const fixed_pool_reference& x)
	{
		mpFixedPool = x.mpFixedPool;
	}

	fixed_pool_reference& operator=(const fixed_pool_reference& x)
	{
		mpFixedPool = x.mpFixedPool;
		return *this;
	}

	void* allocate(size_t /*n*/, int /*flags*/ = 0)
	{
		return mpFixedPool->allocate();
	}

	void* allocate(size_t /*n*/, size_t /*alignment*/, size_t /*offset*/, int /*flags*/ = 0)
	{
		return mpFixedPool->allocate();
	}

	void deallocate(void* p, size_t /*n*/)
	{
		return mpFixedPool->deallocate(p);
	}

	const char* get_name() const
	{
		return "fixed_pool_reference";
	}

	void set_name(const char* /*pName*/)
	{
	}

protected:
	friend bool operator==(const fixed_pool_reference& a, const fixed_pool_reference& b);
	friend bool operator!=(const fixed_pool_reference& a, const fixed_pool_reference& b);

	eastl::fixed_pool* mpFixedPool;
};


inline bool operator==(const fixed_pool_reference& a, const fixed_pool_reference& b)
{
	return (a.mpFixedPool == b.mpFixedPool);
}

inline bool operator!=(const fixed_pool_reference& a, const fixed_pool_reference& b)
{
	return (a.mpFixedPool != b.mpFixedPool);
}


///////////////////////////////////////////////////////////////////////////////
// TestFixedAllocator
//
EA_DISABLE_VC_WARNING(6262)
static int TestFixedAllocator()
{
	using namespace eastl;

	int nErrorCount = 0;

	{  // fixed_allocator
		typedef eastl::list<int, fixed_allocator> IntList;
		typedef IntList::node_type                IntListNode;

		const size_t  kBufferCount = 200;
		IntListNode   buffer1[kBufferCount];
		IntList       intList1;
		const size_t  kAlignOfIntListNode = EA_ALIGN_OF(IntListNode);

		intList1.get_allocator().init(buffer1, sizeof(buffer1), sizeof(IntListNode), kAlignOfIntListNode);

		for(size_t i = 0; i < kBufferCount; i++)
			intList1.push_back(0);

		EATEST_VERIFY(intList1.size() == kBufferCount);

		// Try making a copy.
		IntListNode buffer2[kBufferCount];
		IntList intList2;
		intList2.get_allocator().init(buffer2, sizeof(buffer2), sizeof(IntListNode), kAlignOfIntListNode);
		intList2 = intList1;
		EATEST_VERIFY(intList2.size() == kBufferCount);
	}

	// fixed_allocator_with_overflow, ensure allocations are coming from fixed buffer. This is to
	// prevent a reported user regression where all allocations were being routed to the overflow
	// allocator.
	{
		const int DEFAULT_VALUE = 0xbaadf00d;
		const int TEST_VALUE = 0x12345689;
		const size_t kBufferCount = 10;

		typedef eastl::list<int, fixed_allocator_with_overflow> IntList;
		typedef IntList::node_type IntListNode;

		const size_t kAlignOfIntListNode = EA_ALIGN_OF(IntListNode);

		// ensure the fixed buffer contains the default value that will be replaced
		IntListNode buffer1[kBufferCount];
		for (int i = 0; i < kBufferCount; i++)
		{
			buffer1[i].mValue = DEFAULT_VALUE;
			EATEST_VERIFY(buffer1[i].mValue == DEFAULT_VALUE);
		}

		IntList intList1;

		// replace all the values in the local buffer with the test value
		intList1.get_allocator().init(buffer1, sizeof(buffer1), sizeof(IntListNode), kAlignOfIntListNode);
		for (size_t i = 0; i < kBufferCount; i++)
			intList1.push_back(TEST_VALUE);

		// ensure the local buffer has been altered with the contents of the list::push_back
		for (int i = 0; i < kBufferCount; i++)
		{
			EATEST_VERIFY(buffer1[i].mValue == TEST_VALUE);
		}

		intList1.clear();
	}

	{  // fixed_allocator_with_overflow
		typedef eastl::list<int, fixed_allocator_with_overflow> IntList;
		typedef IntList::node_type                              IntListNode;

		const size_t  kBufferCount = 200;
		IntListNode   buffer1[kBufferCount];
		IntList       intList1;
		const size_t  kAlignOfIntListNode = EA_ALIGN_OF(IntListNode);

		intList1.get_allocator().init(buffer1, sizeof(buffer1), sizeof(IntListNode), kAlignOfIntListNode);

		for(size_t i = 0; i < kBufferCount * 2; i++)
			intList1.push_back(0);

		EATEST_VERIFY(intList1.size() == (kBufferCount * 2));

		// Try making a copy.
		IntListNode buffer2[kBufferCount];
		IntList intList2;
		intList2.get_allocator().init(buffer2, sizeof(buffer2), sizeof(IntListNode), kAlignOfIntListNode);
		intList2 = intList1;
		EATEST_VERIFY(intList2.size() == (kBufferCount * 2));
	}

	{
		// fixed_pool_reference
		typedef eastl::list<int, fixed_pool_reference> WidgetList;
	 
		WidgetList::node_type buffer[16];
		eastl::fixed_pool myPool(buffer, sizeof(buffer), sizeof(WidgetList::node_type), 16);

		WidgetList myList1(myPool);
		WidgetList myList2(myPool);
	 
		myList1.push_back(1);
		myList2.push_back(1);
		EATEST_VERIFY(myList1 == myList2);

		myList1.push_back(2);
		myList1.sort();
		myList2.push_front(2);
		myList2.sort();
		EATEST_VERIFY(myList1 == myList2);
	}

	return nErrorCount;
}
EA_RESTORE_VC_WARNING()



struct TestClass
{
	mutable int mX;

	TestClass() : mX(37) { }

	void Increment()
		{ mX++; }

	void IncrementConst() const
		{ mX++; }

	int MultiplyBy(int x)
		{ return mX * x; }

	int MultiplyByConst(int x) const
		{ return mX * x; }
};

///////////////////////////////////////////////////////////////////////////////
// TestSwapAllocator
//
static int TestSwapAllocator()
{
	int nErrorCount = 0;

	{
		InstanceAllocator a(nullptr, (uint8_t)111), b(nullptr, (uint8_t)222);
		eastl::swap(a, b);
		
		EATEST_VERIFY(a.mInstanceId == 222);
		EATEST_VERIFY(b.mInstanceId == 111);

		EATEST_VERIFY(EA::StdC::Strcmp(a.get_name(), "InstanceAllocator 222") == 0);
		EATEST_VERIFY(EA::StdC::Strcmp(b.get_name(), "InstanceAllocator 111") == 0);
	}

	return nErrorCount;
}

static int TestAllocationOffsetAndAlignment()
{
	int nErrorCount = 0;

	auto testAllocatorAlignment = [&nErrorCount](int requestedSize, int requestedAlignment, int requestedOffset)
	{
		CountingAllocator::resetCount();
		CountingAllocator a;

		void* p = allocate_memory(a, requestedSize, requestedAlignment, requestedOffset);

		EATEST_VERIFY(p != nullptr);
		EATEST_VERIFY(EA::StdC::IsAligned(p, requestedAlignment));

		a.deallocate(p, requestedSize);
		EATEST_VERIFY(CountingAllocator::getActiveAllocationSize() == 0);
	};

	testAllocatorAlignment(100, 1, 0);
	testAllocatorAlignment(100, 2, 0);
	testAllocatorAlignment(100, 4, 0);
	testAllocatorAlignment(100, 8, 0);
	testAllocatorAlignment(100, 16, 0);

	testAllocatorAlignment(100, 1, 16);
	testAllocatorAlignment(100, 2, 16);
	testAllocatorAlignment(100, 4, 16);
	testAllocatorAlignment(100, 8, 16);
	testAllocatorAlignment(100, 16, 16);

	return nErrorCount;
}


///////////////////////////////////////////////////////////////////////////////
// TestAllocator
//
int TestAllocator()
{
	int nErrorCount = 0;
	
	nErrorCount += TestAllocationOffsetAndAlignment();
	nErrorCount += TestFixedAllocator();
	nErrorCount += TestSwapAllocator();

	return nErrorCount;
}












