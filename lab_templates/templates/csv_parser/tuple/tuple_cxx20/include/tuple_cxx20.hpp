#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <typeinfo>
#include <vector>

#include "tuple_cxx20_exceptions.hpp"

namespace tuple_cxx20 {
  template<typename Ch, typename Tr>
  std::ostream& operator<<(std::basic_ostream<Ch, Tr>& ofs, const std::vector<std::string>& kVec) {
    for( const auto& cell: kVec ) {
      ofs << cell << " ";
    }
    return ofs;
  }

  template<class Ch, class Tr, class Tuple, std::size_t N>
  static void printTuple(std::basic_ostream<Ch, Tr>& out, const Tuple& tuple) {
    constexpr size_t index = N - 1;
    if constexpr( index > 0 ) {
      printTuple<Ch, Tr, Tuple, index>(out, tuple);
      out << " " << std::get<index>(tuple);
    }
    else {
      out << std::get<0>(tuple);
    }
  }

  template<typename Ch, typename Tr, typename... Types>
  std::ostream& operator<<(std::basic_ostream<Ch, Tr>& ofs, const std::tuple<Types...>& tuple) {
    printTuple<Ch, Tr, decltype(tuple), sizeof...(Types)>(ofs, tuple);
    return ofs;
  }

  template<template<typename> class Strategy, typename T>
  auto convert(const std::string& s, size_t N) {
    //    static_assert();
    Strategy<T> convert;
    try {
      return convert(s);
    } catch( ... ) {
      throw tupleCXX20Exceptions::tupleCXX20InvalidConversion(std::string(typeid(T).name()), N);
    }
  }

  template<template<typename> class Strategy, typename... Types, size_t... Indices>
  std::tuple<Types..., std::vector<std::string>> crutch(const std::vector<std::string>& v,
                                                        std::index_sequence<Indices...>) {
    auto tmp = std::tuple_cat(std::make_tuple(convert<Strategy, Types>(v[Indices], Indices)...),
                              std::make_tuple(std::vector<std::string>{}));
    for( std::size_t i = sizeof...(Types); i < v.size(); ++i ) {
      std::get<sizeof...(Types)>(tmp).push_back(v[i]);
    }
    return tmp;
  }

  template<template<typename> class Strategy, typename... Types>
  std::tuple<Types..., std::vector<std::string>> getResultCVSTuple(
          const std::vector<std::string>& v) {
    return crutch<Strategy, Types...>(v, std::make_index_sequence<sizeof...(Types)>{});
  }
}// namespace tuple_cxx20
