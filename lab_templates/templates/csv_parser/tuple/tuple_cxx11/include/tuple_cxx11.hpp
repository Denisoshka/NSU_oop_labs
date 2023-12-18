#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace tuple_cxx11 {
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
}// namespace tuple_cxx11