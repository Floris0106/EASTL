/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "EASTLTest.h"
#include <EASTL/vector.h>
#include <EASTL/algorithm.h>
#include <EASTL/type_traits.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/random.h>
#include <EASTL/tuple.h>

#ifndef EA_COMPILER_NO_STANDARD_CPP_LIBRARY
	EA_DISABLE_ALL_VC_WARNINGS()
	#include <algorithm>
	EA_RESTORE_ALL_VC_WARNINGS()
#endif


///////////////////////////////////////////////////////////////////////////////
// TestMapConstruction
//
// This test compares eastl::map/multimap to std::map/multimap. It could possibly
// work for comparing eastl::hash_map to C++11 std::unordered_map, but we would 
// rather move towards making this test be independent of any std comparisons.
//
// Requires a container that can hold at least 1000 items.
//
template <typename T1, typename T2, bool bMultimap>
int TestMapConstruction()
{
	int nErrorCount = 0;

	TestObject::Reset();

	{
		// We use new because fixed-size versions these objects might be too big for declaration on a stack.
		eastl::unique_ptr<T1> pt1A(new T1);
		eastl::unique_ptr<T2> pt2A(new T2);
		T1& t1A = *pt1A;
		T2& t2A = *pt2A;
		nErrorCount += CompareContainers(t1A, t2A, "Map ctor", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());
		EATEST_VERIFY(t1A.validate());


		eastl::unique_ptr<T1> pt1B(new T1);
		eastl::unique_ptr<T2> pt2B(new T2);
		T1& t1B = *pt1B;
		T2& t2B = *pt2B;
		nErrorCount += CompareContainers(t1B, t2B, "Map ctor", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


		eastl::unique_ptr<T1> pt1C(new T1);
		eastl::unique_ptr<T2> pt2C(new T2);
		T1& t1C = *pt1C;
		T2& t2C = *pt2C;
		for(int i = 0; i < 1000; i++)
		{
			t1C.insert(typename T1::value_type(typename T1::key_type(i), typename T1::mapped_type(i)));
			t2C.insert(typename T2::value_type(typename T2::key_type(i), typename T2::mapped_type(i)));
			EATEST_VERIFY(t1C.validate());
			nErrorCount += CompareContainers(t1C, t2C, "Map insert", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());
		}


		eastl::unique_ptr<T1> pt1D(new T1);
		eastl::unique_ptr<T2> pt2D(new T2);
		T1& t1D = *pt1D;
		T2& t2D = *pt2D;
		nErrorCount += CompareContainers(t1D, t2D, "Map ctor", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


		eastl::unique_ptr<T1> pt1E(new T1(t1C));
		eastl::unique_ptr<T2> pt2E(new T2(t2C));
		T1& t1E = *pt1E;
		T2& t2E = *pt2E;
		EATEST_VERIFY(t1E.validate());
		nErrorCount += CompareContainers(t1E, t2E, "Map ctor", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


		eastl::unique_ptr<T1> pt1F(new T1(t1C.begin(), t1C.end()));
		eastl::unique_ptr<T2> pt2F(new T2(t2C.begin(), t2C.end()));
		T1& t1F = *pt1F;
		T2& t2F = *pt2F;
		EATEST_VERIFY(t1F.validate());
		nErrorCount += CompareContainers(t1F, t2F, "Map ctor", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


		// operator=(const map&)
		t1E = t1D;
		t2E = t2D;
		nErrorCount += CompareContainers(t1D, t2D, "Map operator=", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());
		nErrorCount += CompareContainers(t1E, t2E, "Map operator=", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


		// operator=(map&&)
		// We test just the EASTL container here.
		eastl::unique_ptr<T1> pT1P(new T1);
		eastl::unique_ptr<T1> pT1Q(new T1);
		T1& t1P = *pT1P;
		T1& t1Q = *pT1Q;

		typename T1::key_type k10(0);
		typename T1::key_type k11(1);
		typename T1::key_type k12(2);
		typename T1::key_type k13(3);
		typename T1::key_type k14(4);
		typename T1::key_type k15(5);

		typename T1::value_type v10(k10, typename T1::mapped_type(0));
		typename T1::value_type v11(k11, typename T1::mapped_type(1));
		typename T1::value_type v12(k12, typename T1::mapped_type(2));
		typename T1::value_type v13(k13, typename T1::mapped_type(3));
		typename T1::value_type v14(k14, typename T1::mapped_type(4));
		typename T1::value_type v15(k15, typename T1::mapped_type(5));

		t1P.insert(v10);
		t1P.insert(v11);
		t1P.insert(v12);

		t1Q.insert(v13);
		t1Q.insert(v14);
		t1Q.insert(v15);

		t1Q = eastl::move(t1P); // We are effectively requesting to swap t1A with t1B.
	  //EATEST_VERIFY((t1P.size() == 3) && (t1P.find(k13) != t1P.end()) && (t1P.find(k14) != t1P.end()) && (t1P.find(k15) != t1P.end()));  // Currently operator=(this_type&& x) clears x instead of swapping with it.
		EATEST_VERIFY((t1Q.size() == 3) && (t1Q.find(k10) != t1Q.end()) && (t1Q.find(k11) != t1Q.end()) && (t1Q.find(k12) != t1Q.end()));


		// swap
		t1E.swap(t1D);
		t2E.swap(t2D);
		EATEST_VERIFY(t1D.validate());
		EATEST_VERIFY(t1E.validate());
		nErrorCount += CompareContainers(t1D, t2D, "Map swap", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());
		nErrorCount += CompareContainers(t1E, t2E, "Map swap", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


		// clear
		t1A.clear();
		t2A.clear();
		EATEST_VERIFY(t1A.validate());
		nErrorCount += CompareContainers(t1A, t2A, "Map clear", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());

		t1B.clear();
		t2B.clear();
		EATEST_VERIFY(t1B.validate());
		nErrorCount += CompareContainers(t1B, t2B, "Map clear", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


		// global operators (==, !=, <, etc.)
		t1A.clear();
		t1B.clear();
							   // Make t1A equal to t1B
		t1A.insert(typename T1::value_type(typename T1::key_type(0), typename T1::mapped_type(0)));
		t1A.insert(typename T1::value_type(typename T1::key_type(1), typename T1::mapped_type(1)));
		t1A.insert(typename T1::value_type(typename T1::key_type(2), typename T1::mapped_type(2)));

		t1B.insert(typename T1::value_type(typename T1::key_type(0), typename T1::mapped_type(0)));
		t1B.insert(typename T1::value_type(typename T1::key_type(1), typename T1::mapped_type(1)));
		t1B.insert(typename T1::value_type(typename T1::key_type(2), typename T1::mapped_type(2)));

		EATEST_VERIFY( (t1A == t1B));
		EATEST_VERIFY(!(t1A != t1B));
		EATEST_VERIFY( (t1A <= t1B));
		EATEST_VERIFY( (t1A >= t1B));
		EATEST_VERIFY(!(t1A  < t1B));
		EATEST_VERIFY(!(t1A  > t1B));
							   // Make t1A less than t1B
		t1A.insert(typename T1::value_type(typename T1::key_type(3), typename T1::mapped_type(3)));
		t1B.insert(typename T1::value_type(typename T1::key_type(4), typename T1::mapped_type(4)));

		EATEST_VERIFY(!(t1A == t1B));
		EATEST_VERIFY( (t1A != t1B));
		EATEST_VERIFY( (t1A <= t1B));
		EATEST_VERIFY(!(t1A >= t1B));
		EATEST_VERIFY( (t1A  < t1B));
		EATEST_VERIFY(!(t1A  > t1B));
	}

	EATEST_VERIFY(TestObject::IsClear());
	TestObject::Reset();

	return nErrorCount;
}




///////////////////////////////////////////////////////////////////////////////
// TestMapMutation
//
// Requires a container that can hold at least 1000 items.
//
template <typename T1, typename T2, bool bMultimap>
int TestMapMutation()
{
	int nErrorCount = 0;

	TestObject::Reset();

	{
		eastl::unique_ptr<T1> pt1A(new T1); // We use a pointers instead of concrete object because it's size may be huge.
		eastl::unique_ptr<T2> pt2A(new T2);
		T1&                   t1A = *pt1A;
		T2&                   t2A = *pt2A;
		int                   i, iEnd, p;

		// Set up an array of values to randomize / permute.
		eastl::vector<typename T1::key_type> valueArrayInsert;

		if(gEASTL_TestLevel >= kEASTL_TestLevelLow)
		{
			EASTLTest_Rand rng(EA::UnitTest::GetRandSeed());

			valueArrayInsert.clear();

			for(i = 0; i < 1000; i++)
			{
				valueArrayInsert.push_back(typename T1::key_type(i));

				// Occasionally attempt to duplicate an element, both for map and multimap.
				if(((i + 1) < 1000) && (rng.RandLimit(4) == 0))
				{
					valueArrayInsert.push_back(typename T1::key_type(i));
					i++;
				}
			}

			for(p = 0; p < gEASTL_TestLevel * 100; p++) // For each permutation...
			{
				eastl::random_shuffle(valueArrayInsert.begin(), valueArrayInsert.end(), rng);

				// insert
				for(i = 0, iEnd = (int)valueArrayInsert.size(); i < iEnd; i++)
				{
					typename T1::key_type& k = valueArrayInsert[i];

					t1A.insert(typename T1::value_type(k, k)); // We expect that both arguments are the same.
					t2A.insert(typename T2::value_type(k, k));

					EATEST_VERIFY(t1A.validate());
					nErrorCount += CompareContainers(t1A, t2A, "Map insert", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());
				}


				// reverse iteration
				typename T1::reverse_iterator r1 = t1A.rbegin();
				typename T2::reverse_iterator r2 = t2A.rbegin();

				while(r1 != t1A.rend())
				{
					typename T1::key_type k1 = (*r1).first;
					typename T2::key_type k2 = (*r2).first;
					EATEST_VERIFY(k1 == k2);
				}


				// erase
				for(i = 0, iEnd = (int)valueArrayInsert.size(); i < iEnd; i++)
				{
					typename T1::key_type& k = valueArrayInsert[i];

					typename T1::size_type n1 = t1A.erase(k);
					typename T2::size_type n2 = t2A.erase(k);

					EATEST_VERIFY(n1 == n2);
					EATEST_VERIFY(t1A.validate());
					nErrorCount += CompareContainers(t1A, t2A, "Map erase", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());
				}

				EATEST_VERIFY((TestObject::sTOCount == 0) || (TestObject::sTOCount == (int64_t)valueArrayInsert.size())); // This test will only have meaning when T1 contains TestObject.
			}
		}


		EATEST_VERIFY(TestObject::IsClear());
		TestObject::Reset();


		// Possibly do extended testing.
		if(gEASTL_TestLevel > 6)
		{
			#ifndef EA_COMPILER_NO_STANDARD_CPP_LIBRARY

				valueArrayInsert.clear();

				for(i = 0; i < 9; i++) // Much more than this count would take too long to test all permutations.
					valueArrayInsert.push_back(typename T1::key_type(i));

				// Insert these values into the map in every existing permutation.
				for(p = 0; std::next_permutation(valueArrayInsert.begin(), valueArrayInsert.end()); p++) // For each permutation...
				{
					for(i = 0, iEnd = (int)valueArrayInsert.size(); i < iEnd; i++)
					{
						typename T1::key_type& k = valueArrayInsert[i];

						t1A.insert(typename T1::value_type(k, k)); // We expect that both arguments are the same.
						t2A.insert(typename T2::value_type(k, k));

						EATEST_VERIFY(t1A.validate());
						nErrorCount += CompareContainers(t1A, t2A, "Map insert", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());
					}

					for(i = 0, iEnd = (int)valueArrayInsert.size(); i < iEnd; i++)
					{
						typename T1::key_type& k = valueArrayInsert[i];

						t1A.erase(k);
						t2A.erase(k);

						EATEST_VERIFY(t1A.validate());
						nErrorCount += CompareContainers(t1A, t2A, "Map erase", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());
					}

					EATEST_VERIFY((TestObject::sTOCount == 0) || (TestObject::sTOCount == (int64_t)valueArrayInsert.size())); // This test will only have meaning when T1 contains TestObject.

				}

			#endif // EA_COMPILER_NO_STANDARD_CPP_LIBRARY
		}
	}


	EATEST_VERIFY(TestObject::IsClear());
	TestObject::Reset();


	{  // Other insert and erase operations

		#ifndef EA_COMPILER_NO_STANDARD_CPP_LIBRARY
			eastl::unique_ptr<T1> pt1A(new T1); // We use a pointers instead of concrete object because it's size may be huge.
			eastl::unique_ptr<T2> pt2A(new T2);
			T1& t1A = *pt1A;
			T2& t2A = *pt2A;
			int i;

			// Set up an array of values to randomize / permute.
			eastl::vector<eastl::pair<typename T1::key_type, typename T1::mapped_type> > valueArrayInsert1;
			eastl::vector<  std::pair<typename T2::key_type, typename T2::mapped_type> > valueArrayInsert2;

			EA::UnitTest::Rand rng(EA::UnitTest::GetRandSeed());

			for(i = 0; i < 100; i++)
			{
				valueArrayInsert1.push_back(typename T1::value_type(typename T1::key_type(i), typename T1::mapped_type(i)));
				valueArrayInsert2.push_back(typename T2::value_type(typename T2::key_type(i), typename T2::mapped_type(i)));

				if(rng.RandLimit(3) == 0)
				{
					valueArrayInsert1.push_back(typename T1::value_type(typename T1::key_type(i), typename T1::mapped_type(i)));
					valueArrayInsert2.push_back(typename T2::value_type(typename T2::key_type(i), typename T2::mapped_type(i)));
				}
			}


			// insert(InputIterator first, InputIterator last)
			t1A.insert(valueArrayInsert1.begin(), valueArrayInsert1.end());
			t2A.insert(valueArrayInsert2.begin(), valueArrayInsert2.end());
			EATEST_VERIFY(t1A.validate());
			nErrorCount += CompareContainers(t1A, t2A, "Map insert", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


			// insert_return_type insert(const Key& key);
			t1A.insert(typename T1::key_type(8888));
			t2A.insert(typename T2::value_type(typename T2::key_type(8888), typename T2::mapped_type(0)));
			EATEST_VERIFY(t1A.validate());
			nErrorCount += CompareContainers(t1A, t2A, "Map insert", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


			// iterator insert(iterator position, const value_type& value);
			//
			// If bMultimap == true, then the insertions below should fail due to the 
			// item being present. But they should return the correct iterator value.
			typename T1::iterator it1 = t1A.insert(t1A.find(typename T1::key_type(2)), typename T1::value_type(typename T1::key_type(1), typename T1::mapped_type(1)));
			typename T2::iterator it2 = t2A.insert(t2A.find(typename T2::key_type(2)), typename T2::value_type(typename T2::key_type(1), typename T2::mapped_type(1)));
			EATEST_VERIFY(t1A.validate());
			EATEST_VERIFY(it1->first == typename T1::key_type(1));
			EATEST_VERIFY(it2->first == typename T2::key_type(1));
			nErrorCount += CompareContainers(t1A, t2A, "Map insert", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());

			it1 = t1A.insert(t1A.end(), typename T1::value_type(typename T1::key_type(5), typename T1::mapped_type(5)));
			it2 = t2A.insert(t2A.end(), typename T2::value_type(typename T2::key_type(5), typename T2::mapped_type(5)));
			EATEST_VERIFY(t1A.validate());
			EATEST_VERIFY(it1->first == typename T1::key_type(5));
			EATEST_VERIFY(it2->first == typename T2::key_type(5));
			nErrorCount += CompareContainers(t1A, t2A, "Map insert", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());

			// Now we remove these items so that the insertions above can succeed.
			t1A.erase(t1A.find(typename T1::key_type(1)));
			t2A.erase(t2A.find(typename T2::key_type(1)));
			it1 = t1A.insert(t1A.find(typename T1::key_type(2)), typename T1::value_type(typename T1::key_type(1), typename T1::mapped_type(1)));
			it2 = t2A.insert(t2A.find(typename T2::key_type(2)), typename T2::value_type(typename T2::key_type(1), typename T2::mapped_type(1)));
			EATEST_VERIFY(t1A.validate());
			EATEST_VERIFY(it1->first == typename T1::key_type(1));
			EATEST_VERIFY(it2->first == typename T2::key_type(1));
			nErrorCount += CompareContainers(t1A, t2A, "Map insert", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());

			t1A.erase(t1A.find(typename T1::key_type(5)));
			t2A.erase(t2A.find(typename T2::key_type(5)));
			it1 = t1A.insert(t1A.end(), typename T1::value_type(typename T1::key_type(5), typename T1::mapped_type(5)));
			it2 = t2A.insert(t2A.end(), typename T2::value_type(typename T2::key_type(5), typename T2::mapped_type(5)));
			EATEST_VERIFY(t1A.validate());
			EATEST_VERIFY(it1->first == typename T1::key_type(5));
			EATEST_VERIFY(it2->first == typename T2::key_type(5));
			nErrorCount += CompareContainers(t1A, t2A, "Map insert", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


			// iterator erase(iterator first, iterator last);
			typename T1::iterator it11 = t1A.find(typename T1::key_type(17));
			typename T1::iterator it12 = t1A.find(typename T2::key_type(37));
			t1A.erase(it11, it12);

			typename T2::iterator it21 = t2A.find(typename T1::key_type(17));
			typename T2::iterator it22 = t2A.find(typename T2::key_type(37));
			t2A.erase(it21, it22);

			EATEST_VERIFY(t1A.validate());
			nErrorCount += CompareContainers(t1A, t2A, "Map erase(first, last)", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


			// iterator erase(iterator position);
			t1A.erase(t1A.find(typename T1::key_type(60)));
			t2A.erase(t2A.find(typename T1::key_type(60)));
			EATEST_VERIFY(t1A.validate());
			nErrorCount += CompareContainers(t1A, t2A, "Map erase(first, last)", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());


			// Disabled because this function isn't exposed outside the rbtree yet.
			// void erase(const key_type* first, const key_type* last);
			//typename T1::key_type keyArray1[3] = { typename T1::key_type(70), typename T1::key_type(71), typename T1::key_type(72) };
			//typename T2::key_type keyArray2[3] = { typename T2::key_type(70), typename T2::key_type(71), typename T2::key_type(72) };
			//t1A.erase(keyArray1 + 0, keyArray1 + 3);
			//t2A.erase(keyArray2 + 0, keyArray2 + 3);
			//EATEST_VERIFY(t1A.validate());
			//nErrorCount += CompareContainers(t1A, t2A, "Map erase(first, last)", eastl::use_first<typename T1::value_type>(), eastl::use_first<typename T2::value_type>());

		#endif // EA_COMPILER_NO_STANDARD_CPP_LIBRARY
	}

	{
		// map(std::initializer_list<value_type> ilist, const Compare& compare = Compare(), const allocator_type& allocator = EASTL_MAP_DEFAULT_ALLOCATOR);
		// this_type& operator=(std::initializer_list<T> ilist);
		// void insert(std::initializer_list<value_type> ilist);

		// VS2013 has a known issue when dealing with std::initializer_lists
		// https://connect.microsoft.com/VisualStudio/feedback/details/792355/compiler-confused-about-whether-to-use-a-initializer-list-assignment-operator
		#if !defined(EA_COMPILER_NO_INITIALIZER_LISTS) && !(defined(_MSC_VER) && _MSC_VER == 1800)
			T1 myMap = { {typename T1::key_type(10),typename T1::mapped_type(0)}, {typename T1::key_type(11),typename T1::mapped_type(1)} };
			EATEST_VERIFY(myMap.size() == 2);
			EATEST_VERIFY(myMap.begin()->first == typename T1::key_type(10));
			typename T1::iterator it = myMap.rbegin().base();
			EATEST_VERIFY((--it)->first == typename T1::key_type(11));

			myMap = { {typename T1::key_type(20),typename T1::mapped_type(0)}, {typename T1::key_type(21),typename T1::mapped_type(1)} };
			EATEST_VERIFY(myMap.size() == 2);
			EATEST_VERIFY(myMap.begin()->first == typename T1::key_type(20));
			it = myMap.rbegin().base();
			EATEST_VERIFY((--it)->first == typename T1::key_type(21));

			myMap.insert({ {typename T1::key_type(40),typename T1::mapped_type(0)}, {typename T1::key_type(41),typename T1::mapped_type(1)} });
			EATEST_VERIFY(myMap.size() == 4);
			it = myMap.rbegin().base();
			EATEST_VERIFY((--it)->first == typename T1::key_type(41));
		#endif
	}


	EATEST_VERIFY(TestObject::IsClear());
	TestObject::Reset();

	return nErrorCount;
}




template <typename T1>
int TestMapSpecific(T1& t1A, eastl::false_type) // false_type means this is a map and not a multimap.
{
	int nErrorCount = 0;

	// operator[] (map only)
	typename T1::mapped_type m = t1A[typename T1::key_type(0)];
	EATEST_VERIFY(m == typename T1::mapped_type(0));

	m = t1A[typename T1::key_type(999)];
	EATEST_VERIFY(m == typename T1::mapped_type(999));

	m = t1A[typename T1::key_type(10000000)]; // Test the creation of an element that isn't present.
	EATEST_VERIFY(m == typename T1::mapped_type(0)); // Test for 0 because the default ctor for our test objects assigns 0 to the object.

	return nErrorCount;
}


template <typename T1>
int TestMapSpecific(T1& t1A, eastl::true_type) // true_type means this is a multimap and not a map.
{
	int nErrorCount = 0;

	// equal_range_small (multimap only)
	eastl::pair<typename T1::iterator, typename T1::iterator> er = t1A.equal_range_small(typename T1::key_type(499));
	EATEST_VERIFY(er.first->first  == typename T1::key_type(499));
	EATEST_VERIFY(er.second->first == typename T1::key_type(501));

	er = t1A.equal_range_small(typename T1::key_type(-1));
	EATEST_VERIFY(er.first == er.second);
	EATEST_VERIFY(er.first == t1A.begin());

	return nErrorCount;
}


// Just for the purposes of the map::find_as test below, we declare the following.
// The map::find_as function searches a container of X for a type Y, where the user 
// defines the equality of X to Y. The purpose of TMapComparable is to be a generic type Y
// that can be used for any X. We need to make this generic because the whole TestMapSearch
// function below is templated on type T1 and so we don't know what T1 is ahead of time.

template <typename T>
struct TMapComparable
{
	T b;

	TMapComparable() : b() { }
	TMapComparable(const T& a) : b(a){ }
	const TMapComparable& operator=(const T& a) { b = a; return *this; }
	const TMapComparable& operator=(const TMapComparable& x) { b = x.b; return *this; }
	operator const T&() const { return b; }
};


///////////////////////////////////////////////////////////////////////////////
// TestMapSearch
//
// This function is designed to work with map, fixed_map (and not hash containers).
// Requires a container that can hold at least 1000 items.
//
template <typename T1, bool bMultimap>
int TestMapSearch()
{
	int nErrorCount = 0;

	TestObject::Reset();

	{  // Test find, lower_bound, upper_bound, etc..
		eastl::unique_ptr<T1> pt1A(new T1); // We use a pointers instead of concrete object because it's size may be huge.
		T1& t1A = *pt1A;
		int i, iEnd;
		typename T1::iterator it;

		// Set up an array of values to randomize / permute.
		eastl::vector<typename T1::key_type> valueArrayInsert;

		for(i = 0; i < 1000; i++)
			valueArrayInsert.push_back(typename T1::key_type(i));

		EASTLTest_Rand rng(EA::UnitTest::GetRandSeed());
		eastl::random_shuffle(valueArrayInsert.begin(), valueArrayInsert.end(), rng);


		// insert
		for(i = 0, iEnd = (int)valueArrayInsert.size(); i < iEnd; i++)
		{
			typename T1::key_type k(i);
			t1A.insert(typename T1::value_type(k, k));

			it = t1A.find(k);
			EATEST_VERIFY(it != t1A.end());
		}


		// find
		for(i = 0; i < 1000; i++)
		{
			typename T1::key_type k(i);
			it = t1A.find(k);

			EATEST_VERIFY(it != t1A.end());
			EATEST_VERIFY(it->first  == k);
			EATEST_VERIFY(it->second == k);
		}

		it = t1A.find(typename T1::key_type(-1));
		EATEST_VERIFY(it == t1A.end());

		it = t1A.find(typename T1::key_type(1001));
		EATEST_VERIFY(it == t1A.end());


		// find_as
		typedef TMapComparable<typename T1::key_type> TC;

		// Normally we use find_as to find via a different type, but we can test it here like this.
		for(i = 0; i < 1000; i++)
		{
			TC k = typename T1::key_type(i);
			it = t1A.find_as(k, eastl::less<>());

			EATEST_VERIFY(it != t1A.end());
			EATEST_VERIFY(it->first  == k);
			EATEST_VERIFY(it->second == k);
		}

		it = t1A.find_as(TC(typename T1::key_type(-1)), eastl::less<>());
		EATEST_VERIFY(it == t1A.end());

		it = t1A.find_as(TC(typename T1::key_type(1001)), eastl::less<>());
		EATEST_VERIFY(it == t1A.end());


		// lower_bound
		it = t1A.lower_bound(typename T1::key_type(0));
		EATEST_VERIFY(it == t1A.begin());

		it = t1A.lower_bound(typename T1::key_type(-1));
		EATEST_VERIFY(it == t1A.begin());

		it = t1A.lower_bound(typename T1::key_type(1001));
		EATEST_VERIFY(it == t1A.end());

		t1A.erase(typename T1::key_type(500));
		it = t1A.lower_bound(typename T1::key_type(500));
		EATEST_VERIFY(it->first  == typename T1::key_type(501));


		// upper_bound
		it = t1A.upper_bound(typename T1::key_type(-1));
		EATEST_VERIFY(it == t1A.begin());

		it = t1A.upper_bound(typename T1::key_type(499));
		EATEST_VERIFY(it->first  == typename T1::key_type(501));

		it = t1A.upper_bound(typename T1::key_type(-1));
		EATEST_VERIFY(it->first  == typename T1::key_type(0));

		it = t1A.upper_bound(typename T1::key_type(1000));
		EATEST_VERIFY(it == t1A.end());


		// count
		typename T1::size_type n = t1A.count(typename T1::key_type(-1));
		EATEST_VERIFY(n == 0);

		n = t1A.count(typename T1::key_type(0));
		EATEST_VERIFY(n == 1);

		n = t1A.count(typename T1::key_type(500)); // We removed 500 above.
		EATEST_VERIFY(n == 0);

		n = t1A.count(typename T1::key_type(1001));
		EATEST_VERIFY(n == 0);


		// equal_range
		eastl::pair<typename T1::iterator, typename T1::iterator> er = t1A.equal_range(typename T1::key_type(200));
		EATEST_VERIFY(er.first->first == typename T1::key_type(200));
		EATEST_VERIFY(er.first->second == typename T1::key_type(200));

		er = t1A.equal_range(typename T1::key_type(499));
		EATEST_VERIFY(er.first->first == typename T1::key_type(499));
		EATEST_VERIFY(er.second->first == typename T1::key_type(501));

		er = t1A.equal_range(typename T1::key_type(-1));
		EATEST_VERIFY(er.first == er.second);
		EATEST_VERIFY(er.first == t1A.begin());


		// Some tests need to be differently between map and multimap.
		nErrorCount += TestMapSpecific(t1A, eastl::integral_constant<bool, bMultimap>());
	}

	EATEST_VERIFY(TestObject::IsClear());
	TestObject::Reset();

	return nErrorCount;
}

///////////////////////////////////////////////////////////////////////////////
// TestMapCpp11
//
// This function is designed to work with map, fixed_map, hash_map, fixed_hash_map.
//
template <typename T1>
int TestMapCpp11()
{
	int nErrorCount = 0;

	// template <class... Args>
	// insert_return_type emplace(Args&&... args);
	//
	// template <class... Args> 
	// iterator emplace_hint(const_iterator position, Args&&... args);
	//
	// insert_return_type insert(value_type&& value);
	// iterator insert(const_iterator position, value_type&& value);
	// void insert(std::initializer_list<value_type> ilist);
	TestObject::Reset();

	typedef T1 TOMap;
	typedef typename TOMap::value_type value_type;
	typename TOMap::insert_return_type toMapInsertResult;
	typename TOMap::iterator toMapIterator;

	TOMap      toMap;
	TestObject to0(0);
	TestObject to1(1);

	toMapInsertResult = toMap.emplace(value_type(0, to0));
	EATEST_VERIFY(toMapInsertResult.second == true);
	//EATEST_VERIFY((TestObject::sTOCopyCtorCount == 2) && (TestObject::sTOMoveCtorCount == 1));  // Disabled until we can guarantee its behavior and deal with how it's different between compilers of differing C++11 support.

	toMapInsertResult = toMap.emplace(value_type(1, eastl::move(to1)));
	EATEST_VERIFY(toMapInsertResult.second == true);

	// insert_return_type t1A.emplace(value_type&& value);
	TestObject to4(4);
	value_type value40(4, to4);
	EATEST_VERIFY(toMap.find(4) == toMap.end());
	EATEST_VERIFY(value40.second.mX == 4); // It should change to 0 below during the move swap.
	toMapInsertResult = toMap.emplace(eastl::move(value40));
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMap.find(4) != toMap.end());
	EATEST_VERIFY(value40.second.mX == 0);

	value_type value41(4, TestObject(41));
	toMapInsertResult = toMap.emplace(eastl::move(value41));
	EATEST_VERIFY(toMapInsertResult.second == false);
	EATEST_VERIFY(toMapInsertResult.first->second.mX == 4);
	EATEST_VERIFY(toMap.find(4) != toMap.end());

	// iterator t1A.emplace_hint(const_iterator position, value_type&& value);
	TestObject to5(5);
	value_type value50(5, to5);
	EATEST_VERIFY(toMap.find(5) == toMap.end());
	toMapInsertResult = toMap.emplace(eastl::move(value50));
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMap.find(5) != toMap.end());

	value_type value51(5, TestObject(51));
	toMapIterator = toMap.emplace_hint(toMapInsertResult.first, eastl::move(value51));
	EATEST_VERIFY(toMapIterator->first == 5);
	EATEST_VERIFY(toMapIterator->second.mX == 5);
	EATEST_VERIFY(toMap.find(5) != toMap.end());

	TestObject to6(6);
	value_type value6(6, to6);
	EATEST_VERIFY(toMap.find(6) == toMap.end());
	toMapIterator = toMap.emplace_hint(toMap.begin(), eastl::move(value6)); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 6);
	EATEST_VERIFY(toMap.find(6) != toMap.end());
		
	TestObject to2(2);
	EATEST_VERIFY(toMap.find(2) == toMap.end());
	toMapInsertResult = toMap.emplace(value_type(2, to2));
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMap.find(2) != toMap.end());
	toMapInsertResult = toMap.emplace(value_type(2, to2));
	EATEST_VERIFY(toMapInsertResult.second == false);
	EATEST_VERIFY(toMap.find(2) != toMap.end());

	// iterator t1A.emplace_hint(const_iterator position, const value_type& value);
	TestObject to7(7);
	value_type value70(7, to7);
	EATEST_VERIFY(toMap.find(7) == toMap.end());
	toMapInsertResult = toMap.emplace(value70);
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMap.find(7) != toMap.end());

	value_type value71(7, TestObject(71));
	toMapIterator = toMap.emplace_hint(toMapInsertResult.first, value71);
	EATEST_VERIFY(toMapIterator->first == 7);
	EATEST_VERIFY(toMapIterator->second.mX == 7);
	EATEST_VERIFY(toMap.find(7) != toMap.end());

	TestObject to8(8);
	value_type value8(8, to8);
	EATEST_VERIFY(toMap.find(8) == toMap.end());
	toMapIterator = toMap.emplace_hint(toMap.begin(), value8); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 8);
	EATEST_VERIFY(toMap.find(8) != toMap.end());

	// insert_return_type t1A.insert(value_type&& value);
	TestObject to3(3);
	EATEST_VERIFY(toMap.find(3) == toMap.end());
	toMapInsertResult = toMap.insert(value_type(3, to3));
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMap.find(3) != toMap.end());
	toMapInsertResult = toMap.insert(value_type(3, to3));
	EATEST_VERIFY(toMapInsertResult.second == false);
	EATEST_VERIFY(toMap.find(3) != toMap.end());


	// iterator t1A.insert(const_iterator position, value_type&& value);
	TestObject to9(9);
	value_type value90(9, to9);
	EATEST_VERIFY(toMap.find(9) == toMap.end());
	toMapInsertResult = toMap.emplace(eastl::move(value90));
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMap.find(9) != toMap.end());

	value_type value91(9, TestObject(91));
	toMapIterator = toMap.insert(toMapInsertResult.first, eastl::move(value91));
	EATEST_VERIFY(toMapIterator->first == 9);
	EATEST_VERIFY(toMapIterator->second.mX == 9);
	EATEST_VERIFY(toMap.find(9) != toMap.end());

	TestObject to10(10);
	value_type value10(10, to10);
	EATEST_VERIFY(toMap.find(10) == toMap.end());
	toMapIterator = toMap.insert(toMap.begin(), eastl::move(value10)); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 10);
	EATEST_VERIFY(toMap.find(10) != toMap.end());

	// insert_return_type t1A.emplace(Args&&... args);
	TestObject to11(11);
	EATEST_VERIFY(toMap.find(11) == toMap.end());
	toMapInsertResult = toMap.emplace(11, to11);
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMapInsertResult.first->first == 11);
	EATEST_VERIFY(toMap.find(11) != toMap.end());

	TestObject to111(111);
	toMapInsertResult = toMap.emplace(11, to111);
	EATEST_VERIFY(toMapInsertResult.second == false);
	EATEST_VERIFY(toMapInsertResult.first->first == 11);
	EATEST_VERIFY(toMapInsertResult.first->second.mX == 11);
	EATEST_VERIFY(toMap.find(11) != toMap.end());

	TestObject to12(12);
	EATEST_VERIFY(toMap.find(12) == toMap.end());
	toMapInsertResult = toMap.emplace(12, eastl::move(to12));
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMapInsertResult.first->first == 12);
	EATEST_VERIFY(toMap.find(12) != toMap.end());

	TestObject to121(121);
	toMapInsertResult = toMap.emplace(12, eastl::move(to121));
	EATEST_VERIFY(toMapInsertResult.second == false);
	EATEST_VERIFY(toMapInsertResult.first->first == 12);
	EATEST_VERIFY(toMapInsertResult.first->second.mX == 12);
	EATEST_VERIFY(toMap.find(12) != toMap.end());

	EATEST_VERIFY(toMap.find(13) == toMap.end());
	toMapInsertResult = toMap.emplace(eastl::piecewise_construct, eastl::make_tuple(13), eastl::make_tuple(1, 2, 10)); // 1 + 2 + 10 = 13
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMapInsertResult.first->first == 13);
	EATEST_VERIFY(toMap.find(13) != toMap.end());

	toMapInsertResult = toMap.emplace(eastl::piecewise_construct, eastl::make_tuple(13), eastl::make_tuple(1, 30, 100)); // 1 + 30 + 100 = 131
	EATEST_VERIFY(toMapInsertResult.second == false);
	EATEST_VERIFY(toMapInsertResult.first->first == 13);
	EATEST_VERIFY(toMapInsertResult.first->second.mX == 13);
	EATEST_VERIFY(toMap.find(13) != toMap.end());

	// iterator t1A.emplace_hint(const_iterator position, Args&&... args);
	TestObject to14(14);
	EATEST_VERIFY(toMap.find(14) == toMap.end());
	toMapInsertResult = toMap.emplace(14, to14);
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMap.find(14) != toMap.end());

	TestObject to141(141);
	toMapIterator = toMap.emplace_hint(toMapInsertResult.first, 14, to141);
	EATEST_VERIFY(toMapIterator->first == 14);
	EATEST_VERIFY(toMapIterator->second.mX == 14);
	EATEST_VERIFY(toMap.find(14) != toMap.end());

	TestObject to15(15);
	EATEST_VERIFY(toMap.find(15) == toMap.end());
	toMapIterator = toMap.emplace_hint(toMap.begin(), 15, to15); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 15);
	EATEST_VERIFY(toMap.find(15) != toMap.end());

	TestObject to16(16);
	EATEST_VERIFY(toMap.find(16) == toMap.end());
	toMapInsertResult = toMap.emplace(16, eastl::move(to16));
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMap.find(16) != toMap.end());

	TestObject to161(161);
	toMapIterator = toMap.emplace_hint(toMapInsertResult.first, 16, eastl::move(to161));
	EATEST_VERIFY(toMapIterator->first == 16);
	EATEST_VERIFY(toMapIterator->second.mX == 16);
	EATEST_VERIFY(toMap.find(16) != toMap.end());

	TestObject to17(17);
	EATEST_VERIFY(toMap.find(17) == toMap.end());
	toMapIterator = toMap.emplace_hint(toMap.begin(), 17, eastl::move(to17)); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 17);
	EATEST_VERIFY(toMap.find(17) != toMap.end());

	EATEST_VERIFY(toMap.find(18) == toMap.end());
	toMapInsertResult = toMap.emplace(eastl::piecewise_construct, eastl::make_tuple(18), eastl::make_tuple(3, 5, 10)); // 3 + 5 + 10 = 18
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMap.find(18) != toMap.end());

	toMapIterator = toMap.emplace_hint(toMapInsertResult.first, eastl::piecewise_construct, eastl::make_tuple(18), eastl::make_tuple(1, 80, 100)); // 1 + 80 + 100 = 181
	EATEST_VERIFY(toMapIterator->first == 18);
	EATEST_VERIFY(toMapIterator->second.mX == 18);
	EATEST_VERIFY(toMap.find(18) != toMap.end());

	EATEST_VERIFY(toMap.find(19) == toMap.end());
	toMapIterator = toMap.emplace_hint(toMap.begin(), eastl::piecewise_construct, eastl::make_tuple(19), eastl::make_tuple(4, 5, 10)); // 4 + 5 + 10 = 19 // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 19);
	EATEST_VERIFY(toMap.find(19) != toMap.end());

	// iterator t1A.insert(const_iterator position, const value_type& value);
	TestObject to20(20);
	value_type value20(20, to20);
	EATEST_VERIFY(toMap.find(20) == toMap.end());
	toMapInsertResult = toMap.emplace(value20);
	EATEST_VERIFY(toMapInsertResult.second == true);
	EATEST_VERIFY(toMap.find(20) != toMap.end());

	value_type value201(20, TestObject(201));
	toMapIterator = toMap.insert(toMapInsertResult.first, value201);
	EATEST_VERIFY(toMapIterator->first == 20);
	EATEST_VERIFY(toMapIterator->second.mX == 20);
	EATEST_VERIFY(toMap.find(20) != toMap.end());

	TestObject to21(21);
	value_type value21(21, to21);
	EATEST_VERIFY(toMap.find(21) == toMap.end());
	toMapIterator = toMap.insert(toMap.begin(), value21); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 21);
	EATEST_VERIFY(toMap.find(21) != toMap.end());

	// void insert(std::initializer_list<value_type> ilist);
	toMap.insert({ value_type(22, TestObject(22)), value_type(23, TestObject(23)), value_type(24, TestObject(24)) });
	EATEST_VERIFY(toMap.find(22) != toMap.end());
	EATEST_VERIFY(toMap.find(23) != toMap.end());
	EATEST_VERIFY(toMap.find(24) != toMap.end());

	return nErrorCount;
}

struct NonCopyable
{
	NonCopyable() : mX(0) {}
	NonCopyable(int x) : mX(x) {}

	int mX;

	EA_NON_COPYABLE(NonCopyable)
};

inline bool operator<(const NonCopyable& a, const NonCopyable& b) { return a.mX < b.mX; }

template <typename T>
int TestMapCpp11NonCopyable()
{
	int nErrorCount = 0;

	// Verify that operator[]() can be called for a type that is default constructible but not copy constructible. C++11
	// relaxed the requirements on operator[]() and so this should compile.
	T ncMap;
	ncMap[1].mX = 1;
	EATEST_VERIFY(ncMap[1].mX == 1);

	return nErrorCount;
}

///////////////////////////////////////////////////////////////////////////////
// TestMultimapCpp11
//
// This function is designed to work with multimap, fixed_multimap, hash_multimap, fixed_hash_multimap
//
// This is similar to the TestSetCpp11 function, with some differences related
// to handling of duplicate entries.
//
template <typename T1>
int TestMultimapCpp11()
{
	int nErrorCount = 0;

	// template <class... Args>
	// insert_return_type emplace(Args&&... args);
	//
	// template <class... Args> 
	// iterator emplace_hint(const_iterator position, Args&&... args);
	//
	// insert_return_type insert(value_type&& value);
	// iterator insert(const_iterator position, value_type&& value);
	// void insert(std::initializer_list<value_type> ilist);
	TestObject::Reset();

	typedef T1 TOMap;
	typedef typename TOMap::value_type value_type;
	typename TOMap::iterator toMapIterator;

	TOMap      toMap;
	TestObject to0(0);
	TestObject to1(1);

	toMapIterator = toMap.emplace(value_type(0, to0));
	EATEST_VERIFY(toMapIterator->first == 0);
	//EATEST_VERIFY((TestObject::sTOCopyCtorCount == 2) && (TestObject::sTOMoveCtorCount == 1));  // Disabled until we can guarantee its behavior and deal with how it's different between compilers of differing C++11 support.

	toMapIterator = toMap.emplace(value_type(1, eastl::move(to1)));
	EATEST_VERIFY(toMapIterator->first == 1);

	// insert_return_type t1A.emplace(value_type&& value);
	TestObject to4(4);
	value_type value40(4, to4);
	EATEST_VERIFY(toMap.find(4) == toMap.end());
	EATEST_VERIFY(value40.second.mX == 4); // It should change to 0 below during the move swap.
	toMapIterator = toMap.emplace(eastl::move(value40));
	EATEST_VERIFY(toMapIterator->first == 4);
	EATEST_VERIFY(toMap.find(4) != toMap.end());
	EATEST_VERIFY(value40.second.mX == 0);

	value_type value41(4, TestObject(41));
	toMapIterator = toMap.emplace(eastl::move(value41));
	EATEST_VERIFY(toMapIterator->first == 4);
	EATEST_VERIFY(toMapIterator->second.mX == 41);
	EATEST_VERIFY(toMap.count(4) == 2);

	// iterator t1A.emplace_hint(const_iterator position, value_type&& value);
	TestObject to5(5);
	value_type value50(5, to5);
	EATEST_VERIFY(toMap.find(5) == toMap.end());
	toMapIterator = toMap.emplace(eastl::move(value50));
	EATEST_VERIFY(toMapIterator->first == 5);
	EATEST_VERIFY(toMap.find(5) != toMap.end());

	value_type value51(5, TestObject(51));
	toMapIterator = toMap.emplace_hint(toMapIterator, eastl::move(value51));
	EATEST_VERIFY(toMapIterator->first == 5);
	EATEST_VERIFY(toMapIterator->second.mX == 51);
	EATEST_VERIFY(toMap.count(5) == 2);

	TestObject to6(6);
	value_type value6(6, to6);
	EATEST_VERIFY(toMap.find(6) == toMap.end());
	toMapIterator = toMap.emplace_hint(toMap.begin(), eastl::move(value6)); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 6);
	EATEST_VERIFY(toMap.find(6) != toMap.end());
		
	TestObject to2(2);
	EATEST_VERIFY(toMap.find(2) == toMap.end());
	toMapIterator = toMap.emplace(value_type(2, to2));
	EATEST_VERIFY(toMapIterator->first == 2);
	EATEST_VERIFY(toMap.find(2) != toMap.end());
	toMapIterator = toMap.emplace(value_type(2, to2));
	EATEST_VERIFY(toMapIterator->first == 2);
	EATEST_VERIFY(toMap.find(2) != toMap.end());

	// iterator t1A.emplace_hint(const_iterator position, const value_type& value);
	TestObject to7(7);
	value_type value70(7, to7);
	EATEST_VERIFY(toMap.find(7) == toMap.end());
	toMapIterator = toMap.emplace(value70);
	EATEST_VERIFY(toMapIterator->first == 7);
	EATEST_VERIFY(toMap.find(7) != toMap.end());

	value_type value71(7, TestObject(71));
	toMapIterator = toMap.emplace_hint(toMapIterator, value71);
	EATEST_VERIFY(toMapIterator->first == 7);
	EATEST_VERIFY(toMapIterator->second.mX == 71);
	EATEST_VERIFY(toMap.count(7) == 2);

	TestObject to8(8);
	value_type value8(8, to8);
	EATEST_VERIFY(toMap.find(8) == toMap.end());
	toMapIterator = toMap.emplace_hint(toMap.begin(), value8); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 8);
	EATEST_VERIFY(toMap.find(8) != toMap.end());

	// insert_return_type t1A.insert(value_type&& value);
	TestObject to3(3);
	EATEST_VERIFY(toMap.find(3) == toMap.end());
	toMapIterator = toMap.insert(value_type(3, to3));
	EATEST_VERIFY(toMapIterator->first == 3);
	EATEST_VERIFY(toMap.find(3) != toMap.end());
	toMapIterator = toMap.insert(value_type(3, to3));
	EATEST_VERIFY(toMapIterator->first == 3);
	EATEST_VERIFY(toMap.find(3) != toMap.end());


	// iterator t1A.insert(const_iterator position, value_type&& value);
	TestObject to9(9);
	value_type value90(9, to9);
	EATEST_VERIFY(toMap.find(9) == toMap.end());
	toMapIterator = toMap.emplace(eastl::move(value90));
	EATEST_VERIFY(toMapIterator->first == 9);
	EATEST_VERIFY(toMap.find(9) != toMap.end());

	value_type value91(9, TestObject(91));
	toMapIterator = toMap.insert(toMapIterator, eastl::move(value91));
	EATEST_VERIFY(toMapIterator->first == 9);
	EATEST_VERIFY(toMapIterator->second.mX == 91);
	EATEST_VERIFY(toMap.count(9) == 2);

	TestObject to10(10);
	value_type value10(10, to10);
	EATEST_VERIFY(toMap.find(10) == toMap.end());
	toMapIterator = toMap.insert(toMap.begin(), eastl::move(value10)); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 10);
	EATEST_VERIFY(toMap.find(10) != toMap.end());

	// iterator t1A.emplace(Args&&... args);
	TestObject to11(11);
	EATEST_VERIFY(toMap.find(11) == toMap.end());
	toMapIterator = toMap.emplace(11, to11);
	EATEST_VERIFY(toMapIterator->first == 11);
	EATEST_VERIFY(toMap.find(11) != toMap.end());

	TestObject to111(111);
	toMapIterator = toMap.emplace(11, to111);
	EATEST_VERIFY(toMapIterator->first == 11);
	EATEST_VERIFY(toMapIterator->second.mX == 111);
	EATEST_VERIFY(toMap.count(11) == 2);

	TestObject to12(12);
	EATEST_VERIFY(toMap.find(12) == toMap.end());
	toMapIterator = toMap.emplace(12, eastl::move(to12));
	EATEST_VERIFY(toMapIterator->first == 12);
	EATEST_VERIFY(toMap.find(12) != toMap.end());

	TestObject to121(121);
	toMapIterator = toMap.emplace(12, eastl::move(to121));
	EATEST_VERIFY(toMapIterator->first == 12);
	EATEST_VERIFY(toMapIterator->second.mX == 121);
	EATEST_VERIFY(toMap.count(12) == 2);

	EATEST_VERIFY(toMap.find(13) == toMap.end());
	toMapIterator = toMap.emplace(eastl::piecewise_construct, eastl::make_tuple(13), eastl::make_tuple(1, 2, 10)); // 1 + 2 + 10 = 13
	EATEST_VERIFY(toMapIterator->first == 13);
	EATEST_VERIFY(toMap.find(13) != toMap.end());

	toMapIterator = toMap.emplace(eastl::piecewise_construct, eastl::make_tuple(13), eastl::make_tuple(1, 30, 100)); // 1 + 30 + 100 = 131
	EATEST_VERIFY(toMapIterator->first == 13);
	EATEST_VERIFY(toMapIterator->second.mX == 131);
	EATEST_VERIFY(toMap.count(13) == 2);

	// iterator t1A.emplace_hint(const_iterator position, Args&&... args);
	TestObject to14(14);
	EATEST_VERIFY(toMap.find(14) == toMap.end());
	toMapIterator = toMap.emplace(14, to14);
	EATEST_VERIFY(toMap.find(14) != toMap.end());

	TestObject to141(141);
	toMapIterator = toMap.emplace_hint(toMapIterator, 14, to141);
	EATEST_VERIFY(toMapIterator->first == 14);
	EATEST_VERIFY(toMapIterator->second.mX == 141);
	EATEST_VERIFY(toMap.count(14) == 2);

	TestObject to15(15);
	EATEST_VERIFY(toMap.find(15) == toMap.end());
	toMapIterator = toMap.emplace_hint(toMap.begin(), 15, to15); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 15);
	EATEST_VERIFY(toMap.find(15) != toMap.end());

	TestObject to16(16);
	EATEST_VERIFY(toMap.find(16) == toMap.end());
	toMapIterator = toMap.emplace(16, eastl::move(to16));
	EATEST_VERIFY(toMap.find(16) != toMap.end());

	TestObject to161(161);
	toMapIterator = toMap.emplace_hint(toMapIterator, 16, eastl::move(to161));
	EATEST_VERIFY(toMapIterator->first == 16);
	EATEST_VERIFY(toMapIterator->second.mX == 161);
	EATEST_VERIFY(toMap.count(16) == 2);

	TestObject to17(17);
	EATEST_VERIFY(toMap.find(17) == toMap.end());
	toMapIterator = toMap.emplace_hint(toMap.begin(), 17, eastl::move(to17)); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 17);
	EATEST_VERIFY(toMap.find(17) != toMap.end());

	EATEST_VERIFY(toMap.find(18) == toMap.end());
	toMapIterator = toMap.emplace(eastl::piecewise_construct, eastl::make_tuple(18), eastl::make_tuple(3, 5, 10)); // 3 + 5 + 10 = 18
	EATEST_VERIFY(toMap.find(18) != toMap.end());

	toMapIterator = toMap.emplace_hint(toMapIterator, eastl::piecewise_construct, eastl::make_tuple(18), eastl::make_tuple(1, 80, 100)); // 1 + 80 + 100 = 181
	EATEST_VERIFY(toMapIterator->first == 18);
	EATEST_VERIFY(toMapIterator->second.mX == 181);
	EATEST_VERIFY(toMap.count(18) == 2);

	EATEST_VERIFY(toMap.find(19) == toMap.end());
	toMapIterator = toMap.emplace_hint(toMap.begin(), eastl::piecewise_construct, eastl::make_tuple(19), eastl::make_tuple(4, 5, 10)); // 4 + 5 + 10 = 19 // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 19);
	EATEST_VERIFY(toMap.find(19) != toMap.end());

	// iterator t1A.insert(const_iterator position, const value_type& value);
	TestObject to20(20);
	value_type value20(20, to20);
	EATEST_VERIFY(toMap.find(20) == toMap.end());
	toMapIterator = toMap.emplace(value20);
	EATEST_VERIFY(toMap.find(20) != toMap.end());

	value_type value201(20, TestObject(201));
	toMapIterator = toMap.insert(toMapIterator, value201);
	EATEST_VERIFY(toMapIterator->first == 20);
	EATEST_VERIFY(toMapIterator->second.mX == 201);
	EATEST_VERIFY(toMap.count(20) == 2);

	TestObject to21(21);
	value_type value21(21, to21);
	EATEST_VERIFY(toMap.find(21) == toMap.end());
	toMapIterator = toMap.insert(toMap.begin(), value21); // specify a bad hint. Insertion should still work.
	EATEST_VERIFY(toMapIterator->first == 21);
	EATEST_VERIFY(toMap.find(21) != toMap.end());

	// void insert(std::initializer_list<value_type> ilist);
	toMap.insert({ value_type(22, TestObject(22)), value_type(23, TestObject(23)), value_type(24, TestObject(24)), value_type(24, TestObject(241)) });
	EATEST_VERIFY(toMap.find(22) != toMap.end());
	EATEST_VERIFY(toMap.find(23) != toMap.end());
	EATEST_VERIFY(toMap.count(24) == 2);

	return nErrorCount;
}


///////////////////////////////////////////////////////////////////////////////
// TestMapCpp17
//
// This function is designed to work with map, fixed_map, hash_map, fixed_hash_map, unordered_map.
//
template <typename T1>
int TestMapCpp17()
{

	int nErrorCount = 0;

	TestObject::Reset();

	typedef T1 TOMap;
	typedef typename TOMap::mapped_type mapped_type;
	typename TOMap::iterator toMapIterator;


	{
		// pair<iterator, bool> try_emplace (const key_type& k, Args&&... args);
		// pair<iterator, bool> try_emplace (key_type&& k, Args&&... args);
		// iterator             try_emplace (const_iterator hint, const key_type& k, Args&&... args);
		// iterator             try_emplace (const_iterator hint, key_type&& k, Args&&... args);

		TOMap toMap;

		{ // do initial insert
			auto result = toMap.try_emplace(7, 7); // test fwding to conversion-ctor
			VERIFY(result.second);
			VERIFY(result.first->second == mapped_type(7));
			VERIFY(toMap.size() == 1);
		}

		auto ctorCount = TestObject::sTOCtorCount;

		{ // verify duplicate not inserted
			auto result = toMap.try_emplace(7, mapped_type(7)); // test fwding to copy-ctor
			VERIFY(!result.second);
			VERIFY(result.first->second == mapped_type(7));
			VERIFY(toMap.size() == 1);

			// we explicitly constructed an element for the parameter
			// and one for the VERIFY check
			ctorCount += 2;
			VERIFY(ctorCount == TestObject::sTOCtorCount);
		}

		{ // verify duplicate not inserted
			auto hint = toMap.find(7);
			auto result = toMap.try_emplace(hint, 7, 7); // test fwding to conversion-ctor
			VERIFY(result->first == 7);
			VERIFY(result->second == mapped_type(7));
			VERIFY(toMap.size() == 1);
			// we explicitly constructed an element for the VERIFY check
			++ctorCount;
			VERIFY(ctorCount == TestObject::sTOCtorCount);
		}

		{ // verify duplicate not inserted
			auto hint = toMap.find(7);
			auto result = toMap.try_emplace(hint, 7, mapped_type(7)); // test fwding to copy-ctor
			VERIFY(result->first == 7);
			VERIFY(result->second == mapped_type(7));
			VERIFY(toMap.size() == 1);

			// we explicitly constructed an element for the parameter
			// and one for the VERIFY check
			ctorCount += 2;
			VERIFY(ctorCount == TestObject::sTOCtorCount);
		}

		{
			{
				auto result = toMap.try_emplace(8, 8);
				// emplacing a new value should call exactly one constructor,
				// when the value is constructed in place inside the container.
				++ctorCount;
				VERIFY(result.second);
				VERIFY(result.first->second == mapped_type(8));
				// One more constructor for the temporary in the VERIFY
				++ctorCount;
				VERIFY(toMap.size() == 2);
				VERIFY(ctorCount == TestObject::sTOCtorCount);
			}
			{
				auto result = toMap.try_emplace(9, mapped_type(9));
				VERIFY(result.second);
				VERIFY(result.first->second == mapped_type(9));
				VERIFY(toMap.size() == 3);
				// one more constructor for the temporary argument,
				// one for moving it to the container, and one for the VERIFY
				ctorCount += 3;
				VERIFY(ctorCount == TestObject::sTOCtorCount);

			}
		}
	}

	{
		// eastl::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
		// eastl::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);
		// iterator                    insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
		// iterator                    insert_or_assign(const_iterator hint, key_type&& k, M&& obj);

		TOMap toMap;

		{
			// initial rvalue insert
			auto result = toMap.insert_or_assign(3, mapped_type(3));
			VERIFY(result.second);
			VERIFY(toMap.size() == 1);
			VERIFY(result.first->first == 3);
			VERIFY(result.first->second == mapped_type(3));

			// verify rvalue assign occurred
			result = toMap.insert_or_assign(3, mapped_type(9));
			VERIFY(!result.second);
			VERIFY(toMap.size() == 1);
			VERIFY(result.first->first == 3);
			VERIFY(result.first->second == mapped_type(9));
		}

		{
			mapped_type mt5(5);
			mapped_type mt6(6);
			mapped_type mt7(7);

			{
				// initial lvalue insert
				auto result = toMap.insert_or_assign(5, mt5);
				VERIFY(result.second);
				VERIFY(toMap.size() == 2);
				VERIFY(result.first->first == 5);
				VERIFY(result.first->second == mt5);
			}

			{
				// verify lvalue assign occurred
				auto result = toMap.insert_or_assign(5, mt7);
				VERIFY(!result.second);
				VERIFY(toMap.size() == 2);
				VERIFY(result.first->first == 5);
				VERIFY(result.first->second == mt7);
			}

			{
				// verify lvalue hints
				auto hint = toMap.find(5);
				auto result = toMap.insert_or_assign(hint, 6, mt6);
				VERIFY(result != toMap.end());
				VERIFY(toMap.size() == 3);
				VERIFY(result->first == 6);
				VERIFY(result->second == mt6);
			}

			{
				// verify rvalue hints
				auto hint = toMap.find(6);
				auto result = toMap.insert_or_assign(hint, 7, mapped_type(7));
				VERIFY(result != toMap.end());
				VERIFY(toMap.size() == 4);
				VERIFY(result->first == 7);
				VERIFY(result->second == mapped_type(7));
			}
		}
	}

	EATEST_VERIFY(TestObject::IsClear());
	TestObject::Reset();

	return nErrorCount;
}

///////////////////////////////////////////////////////////////////////////////
// TestMapAccess
//
// This function is designed to work with map, fixed_map, hash_map, fixed_hash_map, unordered_map.
//
// Tests for element access: operator[] and at()
template <typename T1>
int TestMapAccess()
{
	int nErrorCount = 0;

	typedef T1 TOMap;
	typedef typename TOMap::key_type key_type;
	typedef typename TOMap::mapped_type mapped_type;

	TOMap map1;
	map1[key_type(1)] = mapped_type(1);
	map1[key_type(3)] = mapped_type(3);

#if EASTL_EXCEPTIONS_ENABLED
	EATEST_VERIFY_THROW(map1.at(key_type(0)));
	EATEST_VERIFY_THROW(map1.at(key_type(2)));
	EATEST_VERIFY_THROW(map1.at(key_type(4)));
#endif
	map1[key_type(0)] = mapped_type(1);
#if EASTL_EXCEPTIONS_ENABLED
	EATEST_VERIFY_NOTHROW(map1.at(key_type(0)));
	EATEST_VERIFY_NOTHROW(map1.at(key_type(1)));
	EATEST_VERIFY_NOTHROW(map1.at(key_type(3)));
#endif
	EATEST_VERIFY(map1.at(key_type(0)) == mapped_type(1));
	EATEST_VERIFY(map1.at(key_type(1)) == mapped_type(1));
	EATEST_VERIFY(map1.at(key_type(3)) == mapped_type(3));

	const TOMap map2;
	const TOMap map3(map1);

#if EASTL_EXCEPTIONS_ENABLED
	EATEST_VERIFY_THROW(map2.at(key_type(0)));
	EATEST_VERIFY_NOTHROW(map3.at(key_type(0)));
#endif
	EATEST_VERIFY(map3.at(key_type(0)) == mapped_type(1));

	return nErrorCount;
}


template<typename HashContainer>
struct HashContainerReserveTest
{
	int operator()()
	{
		int nErrorCount = 0;

		HashContainer hashContainer;

		const typename HashContainer::size_type reserve_sizes[] = {16, 128, 4096, 32768};
		for (auto& reserve_size : reserve_sizes)
		{
			hashContainer.reserve(reserve_size);

			// verify bucket count and hashtable load_factor requirements
			VERIFY(hashContainer.bucket_count() >= reserve_size);
			VERIFY(hashContainer.load_factor() <= ceilf(reserve_size / hashContainer.get_max_load_factor()));
		}

		return nErrorCount;
	}
};





