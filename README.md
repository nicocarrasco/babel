# babel

## Installation

Ajoutez les remotes de 3 repo conan :

conan-center repository :
```sh
conan remote add <REMOTE> https://api.bintray.com/conan/conan/conan-center 
```

epitech repository :
```sh
conan remote add <REMOTE> https://api.bintray.com/conan/epitech/public-conan 
```

bintray repository :

```sh
conan remote add <REMOTE> https://api.bintray.com/conan/bincrafters/public-conan 
```

## Pour build le projet :

```sh
mkdir build && cd build && conan install --build=missing .. && cmake .. -G "Unix Makefiles" && cmake –build .
```
## Pour lancer le babel :

1.Executer ./bin/babel_serv <port>

2.Executer ./bin/babel_client <ip_adress_of_babel_server> <port>
