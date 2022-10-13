## ДЗ

### Структура репозитория
- projects/<NN> - директории с проектами ДЗ
- current-project - symlink на текущий проект
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
