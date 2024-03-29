# search_engine_project
Итоговый проект "Поисковый движок"
# Структура программы:
Для реализации нужны все указанные директории и файлы(кроме .gitignore и ReadME.md и, возможно, тестовых файлов).

1) Логика движка состоит из трёх классов, что находятся в заголовочных файлах и их реализациях в cpp: ConverterJSON, InvertedIndex, SearchServer

![image](https://user-images.githubusercontent.com/105357210/188262702-468e53ab-abc5-420d-979e-c4c1420e5ba3.png)
2) json.hpp - библиотека для сериализации и десереализации данных JSON

![image](https://user-images.githubusercontent.com/105357210/188262748-2704dae8-f42c-4bbf-adb3-58dbbb4f630b.png)
3) Файл BreakText.h содержит вспомагательную функцию, которая разделяет строки на отдельные слова.
4) Файл main.cpp - основной файл, в котором описана логика взаимодействия с движком.

# Установка:
1) Скачиваем zip-архив с репозитория
![image](https://user-images.githubusercontent.com/105357210/188262954-ad316ce5-8ee7-4e56-a2a3-520942b4f025.png)

2) Открываем скачанный проект в редакторе кода(в примере работа ведётся в CLION).
![image](https://user-images.githubusercontent.com/105357210/188263013-4f806881-aab0-44e7-9a01-43e65ffbdc2e.png)

# Работа с программой :
Перед началом работы убедитесь, что в проекте присутсвуют файлы config.json и requests.json. При необходимости можно
воспользоваться тестовыми файлами:
![image](https://user-images.githubusercontent.com/105357210/188263215-a8e8c9bd-c94b-415d-837f-ee47d44e2894.png)

Если отсутствует хотя бы один из них, при запуске программы будет выброшено соответсвующее исключение об их отсутствии. 
В config.json убедитесь, что присутсвуют такие параметры, как указаны на картинке. В поле files запишите пути к файлам, которым будет осуществляться
поиск(при необходимости модно использовать готовые test_file):

![image](https://user-images.githubusercontent.com/105357210/168249658-606442ef-2bc9-4df1-bbf9-054841a8dcd8.png)

Поле version должно совпадать по значению с переменной версии приложения в main.cpp. Иначе будет брошено исключение.

![image](https://user-images.githubusercontent.com/105357210/168250564-841db19e-7e6f-4a19-98a8-2ec4cded817e.png)  ![image](https://user-images.githubusercontent.com/105357210/168250662-3a41d53a-abfd-4d8c-813a-99c7f5b51307.png)

![image](https://user-images.githubusercontent.com/105357210/168251354-3efe9225-bef9-4759-aa81-44cc60323c7b.png)

Файл answers.json необязателен: он создаётся автоматически при запуске программы. Если он уже есть, то он очищается от всех прежних данных.

Если программа запустилась успешно, значит она готова к работе. В движке реализованы три команды:
# 1. update 
Команда обновляет частотный словарь, проще говоря наш список данных в файлах указанных в config.json в files. С этим списком будет работать поисковый сервер.

# 2. search
Непосредственно сам поиск. В результате, в файл answers.json выходят все запросы и ответы на них.


если совпадения есть в result выводится true (иначе false)

![image](https://user-images.githubusercontent.com/105357210/188263490-b5042090-2954-4675-a9f0-88d2db01dbdf.png)

Если документов с совпадениями больше одного вместо одного пары docid и rank выводится вектор этих пар(пример на картинке)

![image](https://user-images.githubusercontent.com/105357210/188263608-cc7b7839-7ae4-4709-a1a9-e55caeeae8af.png)


