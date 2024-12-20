/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <EASTL/internal/config.h>
#include <EASTL/type_traits.h>

namespace eastl
{
	namespace internal
	{

		//////////////////////////////////////////////////////////////////////
		// is_null
		//
		template <typename T>
		bool is_null(const T&)
		{
			return false;
		}

		template <typename Result, typename... Arguments>
		bool is_null(Result (*const& function_pointer)(Arguments...))
		{
			return function_pointer == nullptr;
		}

		template <typename Result, typename Class, typename... Arguments>
		bool is_null(Result (Class::*const& function_pointer)(Arguments...))
		{
			return function_pointer == nullptr;
		}

		template <typename Result, typename Class, typename... Arguments>
		bool is_null(Result (Class::*const& function_pointer)(Arguments...) const)
		{
			return function_pointer == nullptr;
		}

	} // namespace internal
} // namespace eastl