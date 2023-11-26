#pragma once
#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace {
  template<std::size_t>
  struct printInd {};

  template<typename Ch, typename Tr, class Tuple>
  std::basic_ostream<Ch, Tr>& printTuple(std::basic_ostream<Ch, Tr>& os, const Tuple& t,
                                         printInd<1>) {
    return os << std::get<std::tuple_size<Tuple>::value - 1>(t);
  }

  template<typename Ch, typename Tr, class Tuple>
  std::basic_ostream<Ch, Tr>& printTuple(std::basic_ostream<Ch, Tr>& os, const Tuple& t,
                                         printInd<0>) {
    return os;
  }

  template<typename Ch, typename Tr, class Tuple, size_t Pos>
  std::basic_ostream<Ch, Tr>& printTuple(std::basic_ostream<Ch, Tr>& os, const Tuple& t,
                                         printInd<Pos>) {
    os << std::get<std::tuple_size<Tuple>::value - Pos>(t) << ", ";
    return printTuple(os, t, printInd<Pos - 1>());
  }

  template<typename Type>
  Type readValue(const std::string& row) {
    std::istringstream iss(row);
    Type value;

    if( !(iss >> value) ) {
      throw std::invalid_argument("bad reading type of \"" + row + "\" at ");
    }

    return value;
  }

  template<>
  std::string readValue<std::string>(const std::string& row) {
    return row;
  }

  template<typename... Types>
  void fillTuple(std::tuple<Types...> *tp, const std::vector<std::string>& rowCells, printInd<1>) {
    try {
      std::get<0>(*tp) =
              readValue<typename std::tuple_element<0, std::tuple<Types...>>::type>(rowCells[0]);
    } catch( const std::exception& ex ) {
      std::string msg = ex.what();
      msg += std::to_string(1) + " column, ";

      throw std::invalid_argument(msg);
    }
  }

  template<typename... Types, size_t N>
  void fillTuple(std::tuple<Types...> *tp, const std::vector<std::string>& rowCells, printInd<N>) {
    try {
      std::get<N - 1>(*tp) =
              readValue<typename std::tuple_element<N - 1, std::tuple<Types...>>::type>(
                      rowCells[N - 1]);
    } catch( const std::invalid_argument& ex ) {
      std::string msg = ex.what();
      msg += std::to_string(N) + " column, ";

      throw std::invalid_argument(msg);
    }

    fillTuple<Types...>(tp, rowCells, printInd<N - 1>());
  }
}// namespace

template<typename Ch, typename Tr, typename... Types>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os,
                                       const std::tuple<Types...>& tp) {
  return printTuple(os, tp, printInd<sizeof...(Types)>());
}

template<typename... Types>
void makeTuple(std::tuple<Types...> *tp, const std::vector<std::string>& rowCells) {
  fillTuple(tp, rowCells, printInd<sizeof...(Types)>());
}