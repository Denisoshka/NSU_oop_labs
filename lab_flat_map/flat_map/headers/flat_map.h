#ifndef FLAT_MAP_LAB_FLAT_MAP_H
#define FLAT_MAP_LAB_FLAT_MAP_H

#include <iostream>
template<typename keyT, typename valueT>
class FlatMap {
  private:
  const double resizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
  const size_t startSize = 2;

  struct pair_ {
    keyT key;
    valueT value;
  };

  pair_* array_;
  size_t curSize_;
  size_t maxSize_;

  void resize(size_t new_size);
  [[nodiscard]] size_t getIndex(const keyT& key) const;

  public:
  // стандартный конструктор
  explicit FlatMap();

  // конструктор копирования
  FlatMap(const FlatMap& otherMap);

  // конструктор перемещения
  FlatMap(FlatMap&& otherMap) noexcept;

  // деструктор
  ~FlatMap();

  // оператор присваивания
  FlatMap& operator=(const FlatMap& otherMap);

  // перемещающий operator=
  FlatMap& operator=(FlatMap&& otherMap) noexcept;

  // получить количество элементов в таблице
  [[nodiscard]] std::size_t size() const;

  // доступ / вставка элемента по ключу
  valueT& operator[](const keyT& key);

  // возвращает true, если запись с таким ключом присутствует в таблице
  [[nodiscard]] bool contains(const keyT& key) const;

  // удаление элемента по ключу, возвращает количество удаленных элементов (0 или 1)
  [[nodiscard]] std::size_t erase(const keyT& key);

  // очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ
  void clear();


  class iterator {
private:
    pair_* cur_;

public:
    explicit iterator(pair_* ptr);

    bool operator==(const iterator& other_it) const;

    bool operator!=(const iterator& other_it) const;

    iterator& operator++();

    std::string getKey() {
      return cur_->key;
    }
    std::string getValue() {
      return cur_->value;
    }
  };

  // Получить итератор на первый элемент
  [[nodiscard]] iterator begin();

  // Получить итератор на элемент, следующий за последним
  [[nodiscard]] iterator end();

  // Получить итератор на элемент по данному ключу, или на end(), если такого ключа нет.
  // В отличие от operator[] не создает записи для этого ключа, если её ещё нет

  [[nodiscard]] iterator find(const keyT& key);
};

#endif// NSU_OOP_LABS_FLAT_MAP_H
