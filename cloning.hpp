#ifndef resplunk_util_Cloneable_HeaderPlusPlus
#define resplunk_util_Cloneable_HeaderPlusPlus

#include <memory>
#include <type_traits>

namespace resplunk
{
	namespace util
	{
		struct Cloneable
		{
			Cloneable() = default;
			Cloneable(Cloneable const &) = default;
			Cloneable &operator=(Cloneable const &) = delete;
			Cloneable(Cloneable &&) = default;
			Cloneable &operator=(Cloneable &&) = delete;
			virtual ~Cloneable() = default;

			template<template<typename...> typename Wrapper = std::unique_ptr, typename... Args>
			static auto Clone(Cloneable const &c) noexcept
			-> Wrapper<Cloneable, Args...>
			{
				return Wrapper<Cloneable, Args...>{c.clone()};
			}

		private:
			template<typename DerivedT>
			friend struct CloneImplementor;
			virtual Cloneable *clone() const noexcept = 0;
		};
		template<typename DerivedT>
		struct CloneImplementor
		: virtual Cloneable
		{
			using Cloneable_t = DerivedT;
			using CloneImplementor_t = CloneImplementor;

			template<template<typename...> typename Wrapper = std::unique_ptr, typename... Args>
			static auto Clone(Cloneable_t const &ct) noexcept
			-> Wrapper<Cloneable_t, Args...>
			{
				Cloneable const &c = ct;
				return Wrapper<Cloneable_t, Args...>{dynamic_cast<Cloneable_t *>(c.clone())};
			}

		private:
			CloneImplementor() = default;
			friend Cloneable_t;
		};

		template<typename CloneableT, template<typename...> typename Wrapper = std::unique_ptr, typename... Args>
		struct ClonePtr final
		: Wrapper<CloneableT, Args...>
		{
			static_assert(std::is_base_of<Cloneable, CloneableT>::value);
			using Cloneable_t = CloneableT;
			using ClonePtr_t = ClonePtr;
			using Wrapper_t = Wrapper<CloneableT, Args...>;
			using Wrapper_t::Wrapper_t;
			ClonePtr(ClonePtr const &from) noexcept
			: Wrapper_t{std::move(Cloneable_t::template Clone<Wrapper, Args...>(*from))}
			{
			}
			ClonePtr &operator=(ClonePtr const &from) noexcept
			{
				return Wrapper_t::operator=(std::move(Cloneable_t::Clone<Wrapper, Args...>(*from)));
			}
			ClonePtr(Cloneable_t const &from) noexcept
			: Wrapper_t{std::move(Cloneable_t::Clone<Wrapper, Args...>(from))}
			{
			}
			ClonePtr &operator=(Cloneable_t const &from) noexcept
			{
				return Wrapper_t::operator=(std::move(Cloneable_t::Clone<Wrapper, Args...>(from)));
			}
		};
	}
}

#endif
