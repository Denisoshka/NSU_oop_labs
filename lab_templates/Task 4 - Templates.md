
# Задача 4. Шаблоны (templates) и элементы метапрограммирования (задача в разработке)

## Общие условия

Подзадача 1 (без допов) обязательна для всех. На "3" достаточно сделать (без допов) подзадачи 1 и 2. На "4" и "5" обязательно сделать все 3 подзадачи (их минимальные условия) + какие-то допы. Допы можно выбирать свободно из разных подзадач. Сделайте сначала минимальные условия **всех** подзадач, и только потом делайте допы - на сколько хватит времени.


## Подзадача 1. Кортежи (tuples)

Реализуйте оператор для печати `std::tuple`. Должен работать код наподобие следующего:
```C++
// кортеж может быть любым возможным, не обязательно 3 этих типа
std::tuple<int, std::string, double> t = {5, "abcd", 3.14};
// выводит (5, abcd, 3.14). Вместо std::cout может быть файловый поток (ofstream)
std::cout << t; 
```
Вы ограничены возможностями стандарта С++11. Это означает, что нельзя, например, использовать `if constexpr`, `std::integer_sequence/std::index_sequence` и fold expressions, т.к. они появились в более поздних версиях.

**Дополнительно:** 

1. Сделайте другую реализацию, на этот раз используя `if constexpr`. Возможно, вы также найдете применение `std::integer_sequence/std::index_sequence` и fold expressions.
2. Реализуйте собственный вариант шаблонного класса `tuple`.

## Подзадача 2. Парсер CSV

Формат CSV: табличные данные могут быть представлены как текстовый файл с разделителем `\n` между строками и символом `,` для разделения ячеек внутри строки. Считаем, что данные символы не встречаются внутри данных.

Напишите класс, делающий возможным следующую потоковую работу с CSV:
```C++
int main()
{
   // может быть любое количество любых типов
   CsvParser<int, std::string, double> parser("test.csv", 0 /*skip first lines count*/);
   for (std::tuple<int, std::string, double> rs : parser) {
       std::cout << rs << "\n";
   }
}
```

Потоковая обработка подразумевает lazy (ленивое) чтение строк, то есть вы читаете строку только тогда, когда она реально нужна. Таким образом необходимо реализовать [InputIterator](https://en.cppreference.com/w/cpp/named_req/InputIterator) для чтения данных в CSV файле. 

При сдаче продемонстрировать:
1. Чтение нескольких валидных файлов и печать их содержимого в консоль.
2. Чтение нескольких ошибочных файлов и/или не подходящих по типам.

**Дополнительно:**
1. Обеспечьте возможность создания одновременно нескольких независимых итераторов от одного парсера и независимое чтение данных с их помощью.
2. Реализуйте проверку на этапе компиляции, что все шаблонные параметры парсера могут быть получены из строк (то есть, вы можете строку распарсить в этот тип). Если это не так, выведите ваше собственное понятное сообщение об ошибке.
3. Добавьте в конец кортежа, который возвращает итератор, параметр типа `std::vector<std::string>`, куда будут записаны все остальные колонки, если их в файле больше, чем шаблонных параметров парсера. То есть для примера выше итератор должен возвращать тип `std::tuple<int, std::string, double, std::vector<std::string>>`.
4. Выделите логику приведения строки из CSV-файла к нужному типу в отдельную **стратегию**. Для этого сделайте так, чтобы последним шаблонным параметром CsvParser можно было указать шаблонный функтор для приведения строки к нужному типу. Сделайте обычную реализацию этого функтора, которая использует `operator<<`, и другую реализацию, которая строку возвращает как есть, для числовых типов использует функции `std::sto*` (например, `std::stoi` для `int`), а остальные типы по умолчанию не поддерживает (при попытке их использовать возникает ошибка компиляции).

## Подзадача 3. Обобщённый Flat Map

Превратите ваш класс `FlatMap` из первой задачи в шаблонный класс, поддерживающий любые типы ключей (для которых определён `operator<`) и любые типы значений (для которых определён конструктор без параметров).

```C++
template <class Key, class Value>
class FlatMap {
    // ...
}
```

> [!NOTE]
> Если в первой задаче вы не сделали итераторы, реализуйте их сейчас. Итераторы - часть минимальных требований данной задачи. Если вы не делали первую задачу совсем, вам нужно сразу написать шаблонный класс FlatMap, удовлетворяющий минимальным условиям 1-й задачи + итераторы. Другие допы первой задачи делать не обязательно.

**Дополнительно:**

1. К шаблонным параметрам `Key` и `Value` добавьте `Compare` и `Allocator`, определяющие **стратегии** сравнения ключей и выделения памяти. 
    ```C++
    template <class Key,
              class Value,
              class Compare = std::less<Key>,
              class Allocator = std::allocator<...>
             >
    class FlatMap {
        // ...
    }
    ```
    Работайте с аллокатором не напрямую, а через шаблон `std::allocator_traits`.

    1.1. Продемонстрируйте, как можно использовать в качестве ключа тип, для которого не перегружены операторы сравнения (например, `<`).

3. Реализуйте собственный аллокатор, который выделяет большой кусок памяти (пул) один раз при создании. Его метод `allocate` не делает реального выделения памяти, а лишь возвращает указатель на свободный участок из этого пула, а метод `deallocate` просто ничего не делает. Убедитесь, что с вашим аллокатором корректно работает как ваш класс `FlatMap`, так и стандартные контейнеры (например, `std::vector` или `std::list`). Продемонстрируйте разницу в производительности контейнера `std::list` со стандартным аллокатором и с вашим.

    2.1. Реализуйте метод `deallocate` так, чтобы освобожденное место в пуле можно было использовать повторно.

4. Реализуйте проверку на этапе компиляции, что тип `Key` поддерживает сравнение по `operator<`, а тип `Value` имеет конструктор без параметров. Если это не так, выведите ваше собственное понятное сообщение об ошибке.

5. Сделайте специализацию таблицы для ключей целочисленных беззнаковых типов и значений типа `bool` (то есть, например, `Key = unsigned int, Value = bool`). Эта специализация для хранения данных должна использовать `std::vector<bool>` и не хранить дополнительно больше никаких данных. Из допов данной задачи эта специализация должна использовать **аллокаторы** и реализовывать **итераторы**, другие допы для неё делать не надо.

6. Реализуйте шаблонный метод `try_emplace`, который позволит вставлять элементы в таблицу следующим образом:
    ```C++
    class A {
    public:
        A(int x, double y, const std::string& z) {
            // ...
        }
    }
    FlatMap<std::string, A> map_of_a;
    map_of_a.try_emplace("key1", 3, 5.6, "test"); // создан и вставлен в таблицу объект A(3, 5.6, "test") по ключу "key1"
    ```
    Если такой ключ уже есть в таблице, метод ничего не делает.

7. Сделайте так, чтобы ваш шаблонный класс `FlatMap` корректно работал с типами ключей и значений, у которых запрещено копирование или перемещение (но не оба сразу). Например, у `std::unique_ptr` запрещено копирование.
