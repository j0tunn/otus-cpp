## ДЗ

### Структура репозитория
- projects/<NN> - директории с проектами ДЗ
- current-project - symlink на текущий проект
- tools - вспомогательные инструменты

### Проверка под Linux
```
$ cd tools/ubuntu-img
$ make build # достаточно сделать один раз
$ make start
#/ wget https://github.com/j0tunn/otus-cpp/releases/download/3/helloworld-0.0.3-Linux.deb
#/ apt install ./helloworld-0.0.3-Linux.deb
#/ helloworld_cli
#/ exit
```
