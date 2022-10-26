## ДЗ

### Структура репозитория
- projects/<NN> - директории с проектами ДЗ
- projects/current - symlink на текущий проект
- timus - исходники решенных задач на https://acm.timus.ru/
- tools - вспомогательные инструменты

### Проверка под Linux
Сборка образа с необходимыми зависимостями
```
$ cd tools/ubuntu-img
$ make build
```

Использование образа
```
$ cd tools/ubuntu-img && make start
```
или
```
$ docker run -it j0tunn/ubuntu
```
Для передачи файла с хоста в контейнер:
```
$ docker run -v `pwd`/02_homework-12995-fb7660/ip_filter.tsv:/ip_filter.tsv -it j0tunn/ubuntu
```
Внутри контейнера
```
#/ wget https://github.com/j0tunn/otus-cpp/releases/download/3/helloworld-0.0.3-Linux.deb
#/ apt install ./helloworld-0.0.3-Linux.deb
#/ helloworld_cli
#/ exit
```

### Сборка и проверка задач из папки timus
Пример:
```
$ clang++ -g 1787.cpp -o build/test && printf "5 3\n20 0 0\n" | build/test
```
