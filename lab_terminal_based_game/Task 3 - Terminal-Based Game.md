
# Задача 3. Terminal-Based Game

Напишите небольшую terminal-based игру, то есть такую, которая использует возможности терминала (консоли) вместо графической системы. На выбор предлагаются следующие жанры и примеры (но вы ими не ограничены):

1. Фиксированный шутер наподобие [Space Invaders](https://ru.wikipedia.org/wiki/Space_Invaders).
2. Скролл-шутер (вертикальный или горизонтальный), пример: [Gradius](https://ru.wikipedia.org/wiki/Gradius).
3. Лабиринт, пример: [Pac-Man](https://ru.wikipedia.org/wiki/Pac-Man), [Bomberman](https://ru.wikipedia.org/wiki/Bomberman_(%D0%B8%D0%B3%D1%80%D0%B0,_1983)), ["Танчики"](https://ru.wikipedia.org/wiki/Battle_City).
4. Ваш вариант.

Вы можете реализовать любую игру, удовлетворяющую следующим условиям:

1. Есть управляемый игроком персонаж и другие объекты, с которыми он взаимодействует (враги, стены, пули, и т.д.).
2. Игра в реальном времени: даже если игрок ничего не делает, на экране что-то происходит (летят пули, двигаются враги, и т.д.).
3. Есть система подсчета очков: количество убитых врагов, собранных предметов и т.д.
4. Должно быть условие окончания игры (проигрыша и/или выигрыша), например столкновение с врагом, уменьшение здоровья до 0, сбор всех предметов на экране, и т.д.

Технические условия:

1. Достаточно самой схематичной "графики". Например, игрок - это один символ, враги - другой символ, пули - точки, и т.д.
2. Должно быть как минимум 3 различных игровых объекта (например, "игрок", "враг", "пуля").
3. Все игровые объекты должны быть собраны в одну иерархию классов с единым корнем.
4. Поведение игровых объектов должно быть инкапсулировано в их классах. Игровой движок должен единообразно работать со всеми объектами. Желательно избегать явных проверок на тип объекта, вместо этого разное поведение разных объектов должно реализовываться средствами ООП.

Стандартных возможностей С++ недостаточно для гибкого управления терминалом. Вы можете воспользоваться [системными вызовами](https://learn.microsoft.com/ru-ru/windows/console/console-reference), библиотеками ncurses/[pdcurses](https://pdcurses.org/), или другими средствами. Пример инициализации и использования PDcurses доступен [здесь](../cpp-examples/terminal-game).

## Альтернативный UI

Если у вас есть желание, вместо консольного интерфейса можете сделать графический. "Графика" при этом может быть такой же схематичной - прямоугольники, круги и т.д. Если соберетесь делать графический интерфейс - известите меня заранее, перед началом работы.

## Дополнительно

Дополнительные возможности, предлагаемые для реализации тем, кто сделает основное:

1. Ведение таблицы рекордов (по очкам), возможность её просмотра из игры.
2. Сохранение и загрузка игры.
3. Более продвинутая графика: элементы пседвографики, игровые объекты - уже не один символ, а набор символов, и т.д.
4. Анимация.
5. Более продвинутый искусственный интеллект врагов.
6. Звуки и/или музыка.
7. Ваши идеи?
