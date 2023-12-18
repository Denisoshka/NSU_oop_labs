#pragma once

#include "csv_parser_exceptions.hpp"
#include "tuple_cxx20.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include "iostream"

namespace parser {
  template<class Arg,
           class = decltype(std::declval<std::istringstream>() >> std::declval<Arg&>()) *>
  struct isConvertible {
    static constexpr bool value = true;
  };

  template<class Arg>
  struct isConvertible<Arg> {
    static constexpr bool value = false;
  };


  template<class... Args>
  struct typesCheck;

  template<class Arg, class... Args>
  struct typesCheck<Arg, Args...> {
//    static_assert(isConvertible<Arg>::value, "noooot all types convertible from string");

    /*    static constexpr bool value = isConvertible<Arg>::value && */
    static constexpr bool value = typesCheck<Args...>::value;
  };

  template<>
  struct typesCheck<> {
    static constexpr bool value = true;
  };

  template<template<typename> class Strategy, typename... Types>
  class CSVParser {
    //    static_assert(typesCheck<Types...>::value, "not all types convertible from string");
  private:
    std::istream& Ifs_;
    size_t LineOffset_;
    char ColumnDelim_;
    char RowDelim_;
    char EscapeSym_;
    size_t CurrentLine_;

    std::string GetRow() {
      std::string row;
      std::getline(Ifs_, row, RowDelim_);

      ++CurrentLine_;
      return row;
    }

    std::vector<std::string> tokenizeRow(const std::string& row) {
      std::regex column_delim_regex({ColumnDelim_});
      std::vector<std::string> cells(
              std::sregex_token_iterator(row.begin(), row.end(), column_delim_regex, -1),
              std::sregex_token_iterator());

      if( cells.size() < sizeof...(Types) )
        throw CSVParserExceptions::FewColumnsException(CurrentLine_, cells.size() + 1);

      return cells;
    }

    void makeOffset() {
      for( size_t line = 0; line < LineOffset_; ++line ) {
        if( !Ifs_.ignore(std::numeric_limits<std::streamsize>::max(), Ifs_.widen(RowDelim_)) ) {
          throw CSVParserExceptions::FewRowsException(
                  line - 1, "File does not contain " + std::to_string(LineOffset_) + " lines");
        }
      }
    }

  public:
    class InputIterator;

    CSVParser(std::istream& ifs, size_t lineOffset = 0, char columnDelim = ',',
              char rowDelim = '\n', char escapeSym = '\"')
        : Ifs_(ifs)
        , LineOffset_(lineOffset)
        , ColumnDelim_(columnDelim)
        , RowDelim_(rowDelim)
        , EscapeSym_(escapeSym)
        , CurrentLine_(0) {
      if( !Ifs_ ) {
        throw std::ifstream::failure("File is not open");
      }
      Ifs_.exceptions(std::ifstream::badbit);
    }

    CSVParser(const CSVParser<Strategy, Types...>& src) = delete;

    class InputIterator {
    public:
      using value_type = std::tuple<Types..., std::vector<std::string>>;
      using reference = std::tuple<Types..., std::vector<std::string>>&;
      using pointer = std::shared_ptr<std::tuple<Types..., std::vector<std::string>>>;

      enum Mode {
        ekBegin,
        ekEnd
      };

      InputIterator(CSVParser<Strategy, Types...>& parent, Mode mode)
          : Parser_(parent) {
        switch( mode ) {
          case Mode::ekBegin:
            updatePtr();
            break;
          case Mode::ekEnd:
            ItPtr_ = nullptr;
            break;
        }
      }

      reference operator*() const {
        return *ItPtr_;
      }

      const InputIterator operator++() {
        updatePtr();
        return *this;
      }

      const InputIterator operator++(int) {
        const InputIterator tmp = *this;
        updatePtr();
        return tmp;
      }

      bool operator==(const InputIterator& a) const {
        return a.ItPtr_ == ItPtr_;
      }

      bool operator!=(const InputIterator& a) const {
        return a.ItPtr_ != ItPtr_;
      };


    private:
      pointer ItPtr_;
      CSVParser<Strategy, Types...>& Parser_;

      void updatePtr() {
        std::string row = Parser_.GetRow();

        if( row.empty() ) {
          ItPtr_ = nullptr;
        }
        else {
          try {
            auto tuple =
                    tuple_cxx20::getResultCVSTuple<Strategy, Types...>(Parser_.tokenizeRow(row));
            ItPtr_ = std::make_shared<value_type>(tuple);
          } catch( const tupleCXX20Exceptions::tupleCXX20InvalidConversion& e ) {
            throw CSVParserExceptions::TypeMismatchException(Parser_.CurrentLine_, e.getColum(),
                                                             e.what());
          }
        }
      }
    };

    InputIterator begin() {
      Ifs_.clear();
      makeOffset();
      return InputIterator(*this, InputIterator::Mode::ekBegin);
    }

    InputIterator end() {
      return InputIterator(*this, InputIterator::Mode::ekEnd);
    }
  };
}// namespace parser