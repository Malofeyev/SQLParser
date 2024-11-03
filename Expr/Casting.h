#pragma once

#include <type_traits>


template <typename To, typename From>
bool isa(From *p) noexcept {
    using ResultType = std::remove_cv_t< std::remove_reference_t<
        std::remove_pointer_t< std::remove_cv_t< std::remove_reference_t<To> > >
        > >;
    return ResultType::classof(p);
}

template <typename To, typename From>
auto dyn_cast(From *p) noexcept {
    using ResultType = std::remove_cv_t < std::remove_reference_t <
        std::remove_pointer_t< std::remove_cv_t< std::remove_reference_t<To> > >
        > >;
    return (isa<ResultType>(p)?static_cast<To>(p):nullptr);
}
