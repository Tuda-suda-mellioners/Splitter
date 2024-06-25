// Splitter //

Участники проекта(4.307-1):

1.Нечаева Дарья

2.Таловская Александра

3.Богданова Анастасия

//Цель проекта//

Разработать разделитель HTMLтекст исходного сообщения на фрагменты, не превышающие max_len и, для каждого полученного фрагмента сформировать отдельное сообщение в канал мессенджера, сохраняя разметку исходного сообщения.

// Задача //

Обеспечить корректное разделение исходного HTMLсообщения на фрагменты так, чтобы каждый из этих фрагментов содержал корректную структуру тегов и, соответственно, содержал корректный HTML


// Использование //

cd путь\к\папке\где\находятся\файлы\запуска

gcc -o splitter main.c html.c
splitter --max-len=<длина> input.html


