#ifndef resplunk_util_Cloneable_HeaderPlusPlus
#define resplunk_util_Cloneable_HeaderPlusPlus

#include <memory>
#include <type_traits>

namespace resplunk
{
	namespace util
	{
		template<template<typename T, typename...> typename Wrapper = std::unique_ptr, typename... Args>
		struct Cloneable
		{
			using Wrapper_t = Wrapper<Cloneable, Args...>;
			Cloneable() = default;
			Cloneable(Cloneable const &) = default;
			Cloneable &operator=(Cloneable const &) = delete;
			Cloneable(Cloneable &&) = default;
			Cloneable &operator=(Cloneable &&) = delete;
			virtual ~Cloneable() = default;

			static auto Clone(Cloneable const &c) noexcept
			-> Wrapper_t
			{
				return c.clone();
			}

		private:
			virtual Cloneable *clone() const noexcept = 0;
		};
		template<typename DerivedT, template<typename T, typename...> typename Wrapper = std::unique_ptr, typename... Args>
		struct CloneImplementor
		: virtual Cloneable<Wrapper, Args...>
		{
			using C = DerivedT;
			using Wrapper_t = Wrapper<DerivedT, Args...>;
			using CloneImplementor_t = CloneImplementor;

			static auto Clone(C const &c) noexcept
			-> Wrapper_t
			{
				return Wrapper_t{dynamic_cast<C *>(c.clone())};
			}

		private:
			CloneImplementor() = default;
			friend DerivedT;

			virtual CloneImplementor_t *clone() const noexcept override = 0;
		};
	}
}

#endif
