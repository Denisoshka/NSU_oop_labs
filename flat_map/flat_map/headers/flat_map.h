#ifndef NSU_OOP_LABS_FLAT_MAP_H
#define NSU_OOP_LABS_FLAT_MAP_H

#include <iostream>

class FlatMap {
  private:
  double resizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
  size_t startSize = 2;
  typedef struct pair_ {
    std::string key;
    std::string value;
  } pair_;

  pair_* array_;
  size_t curSize_;
  size_t maxSize_;

  size_t getIndex(const std::string& key) const;
  void resize(size_t new_size);

  public:
  // стандартный конструктор
  explicit FlatMap();

  // конструктор копирования
  FlatMap(const FlatMap& otherMap);

  // деструктор
  ~FlatMap();

  // оператор присваивания
  FlatMap& operator=(const FlatMap& otherMap);

  // получить количество элементов в таблице
  [[nodiscard]] std::size_t size() const;

  // доступ / вставка элемента по ключу
  std::string& operator[](const std::string& key);

  // возвращает true, если запись с таким ключом присутствует в таблице
  [[nodiscard]] bool contains(const std::string& key) const;

  // удаление элемента по ключу, возвращает количество удаленных элементов (0 или 1)
  [[nodiscard]] std::size_t erase(const std::string& key);

  // очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ
  void clear();


  // Получить итератор на первый элемент
   iterator begin();

  // Получить итератор на элемент, следующий за последним
   iterator end();

};

// Получить итератор на первый элемент
// iterator begin();

// Получить итератор на элемент, следующий за последним
// iterator end();

// очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ


#endif// NSU_OOP_LABS_FLAT_MAP_H
