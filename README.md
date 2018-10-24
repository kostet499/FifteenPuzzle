# FifteenPuzzle
 Написать алгоритм для решения игры в “пятнашки”.
 Решением задачи является приведение к виду: [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку.
 Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.

 Решение:
 Имеем граф состояний, вершина в графе состояния имеет своё уникальное поле - расстановка 16 чисел.
 Таким образом каждому состоянию ставится в соответствие уникальный ключ - 64 битное число.
 Мы имеем 16  чисел значениями до 16, то есть мы спокойно можем выделить 4 бита под каждое число в
 unsigned long long переменной. Грубо говоря, можно представить, что поле - 16-ричное число.

 Для решения использовался алгоритм А* c некоторыми модификациями:
 Вместо структуры set, использовался вектор стэков.
 За индекс в векторе отвечает целочисленное значение эвристической функции
 Таким образом, вершины, которые функцией оцениваются одинаково, мы складируем в один стэк.
 То что это стэк обусловлено тем, что таким образом алгоритм будет ближе к IDA. Если бы была очередь,
 то алгоритм был бы ближе к bfs. На практике оказалось, что стэк гораздо лучше очереди.

 Почему не использовался set?
 1. Вершины с одинаковыми оценками от эвристической функции нужно как-то отличать(а таких вершин много).
    Как вариант, сравнивать по ключу. Но это не совсем корректно, так как ключ не может оценить "хорошесть" нашой вершины.
 2. Вставка в сэт выполняется за логарифм, что не очень хорошо для времени.

 Эвристика:
 Манхэттенское расстояние + линейный конфликт.
 Важная оптимизация:
 Для начальной вершины нужно считать эвристику полностью.
 Для остальных достаточно её пересчитывать. Это экономит огромное количество времени.
 Мы перемещаем одну фишку, значит достаточно посчитать для неё манхэттенское расстояние на старой позиции и на новой,
 взять разницу этих двух значений(новое - старое) и прибавить к полной эвристике старой позиции.
 Разницу нового и старого линейных конфликтов для фишки аналогично нужно прибавить.
 Линейный конфликт для фишки - для каждого элемента из того же столбца или строки, где стоит наша фишка, проверяем
 необх. условия и сравниваем с нашей фишкой.

 Еще одна модификация:
 К эвристике в итоге прибляются еще и сделанные шаги, чтобы достить этого состояния.
 Но на практике получилось, что наиболее быстрые и хорошие по памяти решения получаются, если
 прибавить шаги с коэффициентов 3/4(сначала умножаем на 3, потом целочисленно челим на 4). Целочисленность
 деления обусловлена более долгими операциями по работе с float. Также по возможности в программе везде исопльзуются
 битовые операции, чтобы ускорить взятие остатка от деления или само деление.

 Отсюда получается, что достигнутое решение получается не всегда минимальным.
 Чтобы искать минимальное решение, нужно теперь к каждом состоянии в unordered map, дополнительно хранить еще и
 количество шагов(без коэффициентов), которыми мы дошли до этого состояния от начального. Если так получается, что
 мы доходим меньшим числом шагом, то обновляем это значение, и сново добавляем нашу вершину в соотвествующий новой
 оценке стэк.

 Поиск решения теперь ограничивается количеством итераций. Остается подобрать такое число, чтобы решение заходило в
 контест. Я такое число подобрал.
 (чем больше длина минимального решения, тем выше нужно ставить число итераций)
