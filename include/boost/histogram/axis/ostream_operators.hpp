// Copyright 2015-2017 Hans Dembinski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// String representations here evaluate correctly in Python.

#ifndef _BOOST_HISTOGRAM_AXIS_OSTREAM_OPERATORS_HPP_
#define _BOOST_HISTOGRAM_AXIS_OSTREAM_OPERATORS_HPP_

#include <boost/histogram/axis/interval_view.hpp>
#include <boost/histogram/axis/types.hpp>
#include <boost/histogram/axis/value_view.hpp>
#include <boost/histogram/detail/utility.hpp>
#include <ostream>

namespace boost {
namespace histogram {
namespace axis {

namespace detail {
string_view to_string(const transform::identity&) { return {}; }
string_view to_string(const transform::log&) { return {"_log", 4}; }
string_view to_string(const transform::sqrt&) { return {"_sqrt", 5}; }
} // namespace detail

template <typename T>
std::ostream& operator<<(std::ostream& os, const interval_view<T>& i) {
  os << "[" << i.lower() << ", " << i.upper() << ")";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const value_view<T>& i) {
  os << i.value();
  return os;
}

template <typename RealType, typename Transform>
std::ostream& operator<<(std::ostream& os,
                         const regular<RealType, Transform>& a) {
  os << "regular" << detail::to_string(Transform()) << "(" << a.size() << ", "
     << a[0].lower() << ", " << a[a.size()].lower();
  if (!a.label().empty()) {
    os << ", label=";
    ::boost::histogram::detail::escape(os, a.label());
  }
  if (!a.uoflow()) { os << ", uoflow=False"; }
  os << ")";
  return os;
}

template <typename RealType>
std::ostream& operator<<(std::ostream& os,
                         const regular<RealType, axis::transform::pow>& a) {
  os << "regular_pow(" << a.size() << ", " << a[0].lower() << ", "
     << a[a.size()].lower() << ", " << a.transform().power;
  if (!a.label().empty()) {
    os << ", label=";
    ::boost::histogram::detail::escape(os, a.label());
  }
  if (!a.uoflow()) { os << ", uoflow=False"; }
  os << ")";
  return os;
}

template <typename RealType>
std::ostream& operator<<(std::ostream& os, const circular<RealType>& a) {
  os << "circular(" << a.size();
  if (a.phase() != 0.0) { os << ", phase=" << a.phase(); }
  if (a.perimeter() != RealType(::boost::histogram::detail::two_pi)) {
    os << ", perimeter=" << a.perimeter();
  }
  if (!a.label().empty()) {
    os << ", label=";
    ::boost::histogram::detail::escape(os, a.label());
  }
  os << ")";
  return os;
}

template <typename RealType>
std::ostream& operator<<(std::ostream& os, const variable<RealType>& a) {
  os << "variable(" << a[0].lower();
  for (int i = 1; i <= a.size(); ++i) { os << ", " << a[i].lower(); }
  if (!a.label().empty()) {
    os << ", label=";
    ::boost::histogram::detail::escape(os, a.label());
  }
  if (!a.uoflow()) { os << ", uoflow=False"; }
  os << ")";
  return os;
}

template <typename IntType>
std::ostream& operator<<(std::ostream& os, const integer<IntType>& a) {
  os << "integer(" << a[0].lower() << ", " << a[a.size()].lower();
  if (!a.label().empty()) {
    os << ", label=";
    ::boost::histogram::detail::escape(os, a.label());
  }
  if (!a.uoflow()) { os << ", uoflow=False"; }
  os << ")";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const category<T>& a) {
  os << "category(";
  for (int i = 0; i < a.size(); ++i) {
    os << a[i] << (i == (a.size() - 1) ? "" : ", ");
  }
  if (!a.label().empty()) {
    os << ", label=";
    ::boost::histogram::detail::escape(os, a.label());
  }
  os << ")";
  return os;
}

template <>
std::ostream& operator<<(std::ostream& os, const category<std::string>& a) {
  os << "category(";
  for (int i = 0; i < a.size(); ++i) {
    ::boost::histogram::detail::escape(os, a.value(i));
    os << (i == (a.size() - 1) ? "" : ", ");
  }
  if (!a.label().empty()) {
    os << ", label=";
    ::boost::histogram::detail::escape(os, a.label());
  }
  os << ")";
  return os;
}

} // namespace axis
} // namespace histogram
} // namespace boost

#endif
