#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

template<typename Ch, typename Tr>
std::ostream& operator<<(std::basic_ostream<Ch, Tr>& ofs, const std::vector<std::string>& kVec) {
  for( const auto& cell: kVec ) {
    ofs << cell << " ";
  }
  return ofs;
}

template<class Ch, class Tr, class Tuple, std::size_t N>
struct TuplePrinter {
  static void print(std::basic_ostream<Ch, Tr>& ofs, const Tuple& t) {
    TuplePrinter<Ch, Tr, Tuple, N - 1>::print(ofs, t);
    ofs << " " << std::get<N - 1>(t);
  }
};

template<class Ch, class Tr, class Tuple>
struct TuplePrinter<Ch, Tr, Tuple, 1> {
  static void print(std::basic_ostream<Ch, Tr>& ofs, const Tuple& t) {
    ofs << std::get<0>(t);
  }
};

template<typename Ch, typename Tr, typename... Args>
std::ostream& operator<<(std::basic_ostream<Ch, Tr>& ofs, const std::tuple<Args...>& tuple) {
  TuplePrinter<Ch, Tr, decltype(tuple), sizeof...(Args)>::print(ofs, tuple);
  return ofs;
}

/*
  template<typename T>
  T convert(const std::string& s, size_t N) {
    std::stringstream buff(s);

    T value;
    if( !(buff >> value) ) {
      throw tupleCXX20Exceptions::tupleCXX20InvalidConversion(std::string(typeid(T).name()), N);
    }

    return value;
  }

  template<typename... Types, size_t... Indices>
  std::tuple<Types..., std::vector<std::string>> crutch(const std::vector<std::string>& v,
                                                        std::index_sequence<Indices...>) {
    auto tmp = std::tuple_cat(std::make_tuple(convert<Types>(v[Indices], Indices)...),
                              std::make_tuple(std::vector<std::string>{}));
    for( std::size_t i = sizeof...(Types); i < v.size(); ++i ) {
      std::get<sizeof...(Types)>(tmp).push_back(v[i]);
    }
    return tmp;
  }

  template<typename... Types>
  std::tuple<Types..., std::vector<std::string>> getResultCVSTuple(
          const std::vector<std::string>& v) {
    return crutch<Types...>(v, std::make_index_sequence<sizeof...(Types)>{});
  }*/
// namespace tuple_cxx11
