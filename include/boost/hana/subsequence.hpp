/*!
@file
Defines `boost::hana::subsequence`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SUBSEQUENCE_HPP
#define BOOST_HANA_SUBSEQUENCE_HPP

#include <boost/hana/fwd/subsequence.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/value.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename Indices>
    constexpr auto subsequence_t::operator()(Xs&& xs, Indices&& indices) const {
        using S = typename datatype<Xs>::type;
        using Subsequence = BOOST_HANA_DISPATCH_IF(subsequence_impl<S>,
            _models<Sequence, S>::value &&
            _models<Foldable, typename datatype<Indices>::type>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Sequence, S>::value,
        "hana::subsequence(xs, indices) requires 'xs' to be a Sequence");

        static_assert(_models<Foldable, typename datatype<Indices>::type>::value,
        "hana::subsequence(xs, indices) requires 'indices' to be Foldable");
    #endif

        return Subsequence::apply(static_cast<Xs&&>(xs),
                                  static_cast<Indices&&>(indices));
    }
    //! @endcond

    template <typename S, bool condition>
    struct subsequence_impl<S, when<condition>> : default_ {
        //! @todo
        //! Since we have the right to specify the same index more than once,
        //! we can't move from the element at that index because that would
        //! cause a double move. However, it should be fairly compile-time
        //! efficient to check at which indices we can move because all the
        //! indices are Constants anyway.
        template <typename Xs, typename ...N>
        static constexpr auto
        subsequence_helper(Xs&& xs, tuple<N...> const&) {
            return hana::make<S>(hana::at_c_t<hana::value<N>()>{}(xs)...);
        }

        template <typename Xs, typename Indices>
        static constexpr auto apply(Xs&& xs, Indices const& indices) {
            return subsequence_impl::subsequence_helper(
                static_cast<Xs&&>(xs),
                hana::to<Tuple>(indices)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SUBSEQUENCE_HPP
