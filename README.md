# tcp-udp-echo-client-server

Тестовое задание в НТЦ ПРОТЕЙ

## Сборка

В системе должен быть установлен *cmake* и *компилятор C++*

1. Склонировать репозиторий проекта

```bash
git clone <url>
```

2. Перейти в каталог с репозиторием

```bash
cd tcp-udp-echo-client-server
```

3. Выполнить команду

```bash
cmake -H. -Bbuild
```

а затем

```bash
cmake --build build
```

В каталоге *build/bin* появятся скомпилированные программы

## Запуск

### Сервер

Для запуска сервера выполнить команду

```bash
./build/bin/server <tcp_port> <udp_port>
```

где

- *tcp_port* - порт, на котором сервер будет слушать по **tcp**

- *udp_port* - порт, на котором сервер будет слушать по **udp**

### Клиент

Для запуска клиента выполнить команду

```bash
./build/bin/client <tcp|udp> <port>
```

где

- Первый аргумент название протокола, по которому клиент будет посылать сообщения на сервер

- Второй аргумент *порт*, по которому на соответствующем протоколе(**перый аргумент**) слушает сервер

#### Завершение работы

- Для завершения работы *клиента* ввести на клиенте сообщение **quit** при этом сервер **продолжит** работу

- Для завершения работы *сервера* ввести на клиенте сообщение **server power off** при этом и клиент и сервер завершат работу