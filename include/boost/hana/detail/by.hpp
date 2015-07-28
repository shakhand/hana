/*!
@file
Defines `boost::hana::detail::by`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_BY_HPP
#define BOOST_HANA_DETAIL_BY_HPP

#include <boost/hana/detail/by_fwd.hpp>

#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/partial.hpp>


namespace boost { namespace hana { namespace detail {
    //! @cond
    template <typename Algorithm>
    template <typename Predicate, typename Object>
    constexpr decltype(auto) by_t<Algorithm>::
    operator()(Predicate&& predicate, Object&& object) const
    {
        return Algorithm{}(static_cast<Object&&>(object),
                           static_cast<Predicate&&>(predicate));
    }

    template <typename Algorithm>
    template <typename Predicate>
    constexpr decltype(auto)
    by_t<Algorithm>::operator()(Predicate&& predicate) const
    {
        return hana::partial(hana::flip(Algorithm{}),
                             static_cast<Predicate&&>(predicate));
    }
    //! @endcond
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_BY_HPP
