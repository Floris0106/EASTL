/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#pragma once


#include <EASTL/internal/config.h>
#include <stddef.h>



namespace eastl
{

	/// alloc_flags
	///
	/// Defines allocation flags.
	///
	enum alloc_flags 
	{
		MEM_TEMP = 0, // Low memory, not necessarily actually temporary.
		MEM_PERM = 1  // High memory, for things that won't be unloaded.
	};


	/// allocator
	///
	/// In this allocator class, note that it is not templated on any type and
	/// instead it simply allocates blocks of memory much like the C malloc and
	/// free functions. It can be thought of as similar to C++ std::allocator<char>.
	/// The flags parameter has meaning that is specific to the allocation 
	///
	/// C++11's std::allocator (20.6.9) doesn't have a move constructor or assignment 
	/// operator. This is possibly because std::allocators are associated with types
	/// instead of as instances. The potential non-equivalance of C++ std::allocator
	/// instances has been a source of some acknowledged design problems.
	/// We don't implement support for move construction or assignment in eastl::allocator,
	/// but users can define their own allocators which do have move functions and 
	/// the eastl containers are compatible with such allocators (i.e. nothing unexpected
	/// will happen).
	///
	class EASTL_API allocator
	{
	public:
		EASTL_ALLOCATOR_EXPLICIT allocator(const char* pName = EASTL_NAME_VAL(EASTL_ALLOCATOR_DEFAULT_NAME));
		allocator(const allocator& x);
		allocator(const allocator& x, const char* pName);

		allocator& operator=(const allocator& x);

		void* allocate(size_t n, int flags = 0);
		void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
		void  deallocate(void* p, size_t n);

		const char* get_name() const;
		void        set_name(const char* pName);

	protected:
		#if EASTL_NAME_ENABLED
			const char* mpName; // Debug name, used to track memory.
		#endif
	};

	bool operator==(const allocator& a, const allocator& b);
#if !defined(EA_COMPILER_HAS_THREE_WAY_COMPARISON)
	bool operator!=(const allocator& a, const allocator& b);
#endif


	/// dummy_allocator
	///
	/// Defines an allocator which does nothing. It returns NULL from allocate calls.
	///
	class EASTL_API dummy_allocator
	{
	public:
		EASTL_ALLOCATOR_EXPLICIT dummy_allocator(const char* = NULL) { }
		dummy_allocator(const dummy_allocator&) { }
		dummy_allocator(const dummy_allocator&, const char*) { }

		dummy_allocator& operator=(const dummy_allocator&) { return *this; }

		void* allocate(size_t, int = 0)                 { return NULL; }
		void* allocate(size_t, size_t, size_t, int = 0) { return NULL; }
		void  deallocate(void*, size_t)                 { }

		const char* get_name() const      { return ""; }
		void        set_name(const char*) { }
	};

	inline bool operator==(const dummy_allocator&, const dummy_allocator&) { return true;  }
#if !defined(EA_COMPILER_HAS_THREE_WAY_COMPARISON)
	inline bool operator!=(const dummy_allocator&, const dummy_allocator&) { return false; }
#endif


	/// Defines a static default allocator which is constant across all types.
	/// This is different from get_default_allocator, which is is bound at
	/// compile-time and expected to differ per allocator type.
	/// Currently this Default Allocator applies only to CoreAllocatorAdapter.
	/// To consider: This naming of this function is too similar to get_default_allocator
	/// and instead should be named something like GetStaticDefaultAllocator.
	EASTL_API allocator* GetDefaultAllocator();
	EASTL_API allocator* SetDefaultAllocator(allocator* pAllocator);


	/// get_default_allocator
	///
	/// This templated function allows the user to implement a default allocator
	/// retrieval function that any part of EASTL can use. EASTL containers take
	/// an Allocator parameter which identifies an Allocator class to use. But 
	/// different kinds of allocators have different mechanisms for retrieving 
	/// a default allocator instance, and some don't even intrinsically support
	/// such functionality. The user can override this get_default_allocator 
	/// function in order to provide the glue between EASTL and whatever their
	/// system's default allocator happens to be.
	///
	/// Example usage:
	///     MyAllocatorType* gpSystemAllocator;
	///     
	///     MyAllocatorType* get_default_allocator(const MyAllocatorType*)
	///         { return gpSystemAllocator; }
	///
	template <typename Allocator>
	Allocator* get_default_allocator(const Allocator*);

	EASTLAllocatorType* get_default_allocator(const EASTLAllocatorType*);


	/// default_allocfreemethod
	///
	/// Implements a default allocfreemethod which uses the default global allocator.
	/// This version supports only default alignment.
	///
	void* default_allocfreemethod(size_t n, void* pBuffer, void* /*pContext*/);


	/// allocate_memory
	///
	/// This is a memory allocation dispatching function.
	/// To do: Make aligned and unaligned specializations.
	///        Note that to do this we will need to use a class with a static
	///        function instead of a standalone function like below.
	///
	template <typename Allocator>
	void* allocate_memory(Allocator& a, size_t n, size_t alignment, size_t alignmentOffset);


} // namespace eastl






#ifndef EASTL_USER_DEFINED_ALLOCATOR // If the user hasn't declared that he has defined a different allocator implementation elsewhere...

	EA_DISABLE_ALL_VC_WARNINGS()
	#include <cstdlib>
	EA_RESTORE_ALL_VC_WARNINGS()

	namespace eastl
	{
		inline allocator::allocator(const char* EASTL_NAME(pName))
		{
			#if EASTL_NAME_ENABLED
				mpName = pName ? pName : EASTL_ALLOCATOR_DEFAULT_NAME;
			#endif
		}


		inline allocator::allocator(const allocator& EASTL_NAME(alloc))
		{
			#if EASTL_NAME_ENABLED
				mpName = alloc.mpName;
			#endif
		}


		inline allocator::allocator(const allocator&, const char* EASTL_NAME(pName))
		{
			#if EASTL_NAME_ENABLED
				mpName = pName ? pName : EASTL_ALLOCATOR_DEFAULT_NAME;
			#endif
		}


		inline allocator& allocator::operator=(const allocator& EASTL_NAME(alloc))
		{
			#if EASTL_NAME_ENABLED
				mpName = alloc.mpName;
			#endif
			return *this;
		}


		inline const char* allocator::get_name() const
		{
			#if EASTL_NAME_ENABLED
				return mpName;
			#else
				return EASTL_ALLOCATOR_DEFAULT_NAME;
			#endif
		}


		inline void allocator::set_name(const char* EASTL_NAME(pName))
		{
			#if EASTL_NAME_ENABLED
				mpName = pName;
			#endif
		}


		inline void* allocator::allocate(size_t n, int)
		{
			#ifdef EA_PLATFORM_MICROSOFT
				return _aligned_malloc(n, EASTL_ALLOCATOR_MIN_ALIGNMENT);
			#else
				void *p = nullptr;
				posix_memalign(&p, EASTL_ALLOCATOR_MIN_ALIGNMENT, n);
				return p;
			#endif
		}


		inline void* allocator::allocate(size_t n, size_t alignment, size_t, int)
		{
			#ifdef EA_PLATFORM_MICROSOFT
				return _aligned_malloc(n, alignment);
			#else
				void *p = nullptr;
				alignment = alignment < sizeof( void *) ? sizeof( void *) : alignment;
				posix_memalign(&p, alignment, n);
				return p;
			#endif
		}


		inline void allocator::deallocate(void* p, size_t)
		{
			#ifdef EA_PLATFORM_MICROSOFT
				_aligned_free(p);
			#else
				free(p);
			#endif
		}


		inline bool operator==(const allocator&, const allocator&)
		{
			return true; // All allocators are considered equal, as they merely use global new/delete.
		}

#if !defined(EA_COMPILER_HAS_THREE_WAY_COMPARISON)
		inline bool operator!=(const allocator&, const allocator&)
		{
			return false; // All allocators are considered equal, as they merely use global new/delete.
		}
#endif

	} // namespace eastl


#endif // EASTL_USER_DEFINED_ALLOCATOR



namespace eastl
{

	template <typename Allocator>
	inline Allocator* get_default_allocator(const Allocator*)
	{
		return NULL; // By default we return NULL; the user must make specialization of this function in order to provide their own implementation.
	}


	inline EASTLAllocatorType* get_default_allocator(const EASTLAllocatorType*)
	{
		return EASTLAllocatorDefault(); // For the built-in allocator EASTLAllocatorType, we happen to already have a function for returning the default allocator instance, so we provide it.
	}


	inline void* default_allocfreemethod(size_t n, void* pBuffer, void* /*pContext*/)
	{
		EASTLAllocatorType* const pAllocator = EASTLAllocatorDefault();

		if(pBuffer) // If freeing...
		{
			EASTLFree(*pAllocator, pBuffer, n);
			return NULL;  // The return value is meaningless for the free.
		}
		else // allocating
			return EASTLAlloc(*pAllocator, n);
	}


	/// allocate_memory
	///
	/// This is a memory allocation dispatching function.
	/// To do: Make aligned and unaligned specializations.
	///        Note that to do this we will need to use a class with a static
	///        function instead of a standalone function like below.
	///
	template <typename Allocator>
	inline void* allocate_memory(Allocator& a, size_t n, size_t alignment, size_t alignmentOffset)
	{
		void *result;
		if (alignment <= EASTL_ALLOCATOR_MIN_ALIGNMENT)
		{
			result = EASTLAlloc(a, n);
			// Ensure the result is correctly aligned.  An assertion likely indicates a mismatch between EASTL_ALLOCATOR_MIN_ALIGNMENT and the minimum alignment
			// of EASTLAlloc.  If there is a mismatch it may be necessary to define EASTL_ALLOCATOR_MIN_ALIGNMENT to be the minimum alignment of EASTLAlloc, or
			// to increase the alignment of EASTLAlloc to match EASTL_ALLOCATOR_MIN_ALIGNMENT.
			EASTL_ASSERT((reinterpret_cast<size_t>(result)& ~(alignment - 1)) == reinterpret_cast<size_t>(result));
		}
		else
		{
			result = EASTLAllocAligned(a, n, alignment, alignmentOffset);
			// Ensure the result is correctly aligned.  An assertion here may indicate a bug in the allocator.
			auto resultMinusOffset = (char*)result - alignmentOffset;
			EA_UNUSED(resultMinusOffset);
			EASTL_ASSERT((reinterpret_cast<size_t>(resultMinusOffset)& ~(alignment - 1)) == reinterpret_cast<size_t>(resultMinusOffset));
		}
		return result;
	}

}