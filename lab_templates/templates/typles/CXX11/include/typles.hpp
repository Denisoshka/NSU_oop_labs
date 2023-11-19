#pragma once
#include <ostream>
#include <tuple>

namespace typles {
  template<typename Ch, typename Tr, typename Tuple, size_t index>
  class Printer {
    static void printTuple(std::basic_ostream<Ch, Tr>& os, const Tuple& tuple) {
      Printer<Ch, Tr, Tuple, index - 1>::printTuple(os, tuple);
      os << ", " << std::get<index - 1>(tuple);
    }
  };
  template<typename Ch, typename Tr, typename Tuple>
  class Printer<Ch, Tr, Tuple, 0> {
    static void printTuple(std::basic_ostream<Ch, Tr>& os, const Tuple& tuple) {
      os << std::get<0>(tuple);
    }
  };

  template<typename Ch, typename Tr, typename... Args>
  auto& operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
    os<<'(';
    Printer< decltype(t), sizeof...(Args)>::printTuple(t);
    return os;
  }
}// namespace typles
