# Weather-dl

A simple weather info downloader implemented in C++ to demonstrate multithreading

## Getting Started

This project is created as an exercise for self learning C++.
It makes use of ```std::thread```s, mutexes, [libcurl](https://curl.haxx.se/libcurl/) (for http requests) and file manipulation.
The main purpose of the application is to load weather info (as JSON strings) for a specific set of cities read from a raw text file, 
from an open web api ([metaweather](https://www.metaweather.com/api/)). 
The requests for each city is sent using one or several threads specified by the user.
The whole process and the overall time is printed to the console so that the impact of threads are clearly demonstrated to the user.

Fetching with 2 threads (32 sec)                                             |  Fetching with 5 threads (13 sec)
:---------------------------------------------------------------------------:|:-------------------------:
![](https://github.com/janithjeewantha/cpp_threads/blob/master/img/two.gif)  |  ![](https://github.com/janithjeewantha/cpp_threads/blob/master/img/three.gif)

### Prerequisites

Since I have used ```std::thread``` class instead of ```pthread```, you will need a c++11 compatible compiler.
Then install libcurl development libraries. This step may vary according to your distro.

On Ubuntu,

```sh
sudo apt install libcurl-openssl-dev
```

That's all you need to compile and run the program

### Usage

You can compile the program by

```sh
g++ weather-dl.cpp -std=c++11 -lcurl -lpthread -o weather-dl
```

```-std=c++11``` is needed since std::thread class is introduced in C++ 11. The linking of curl and threads with the os libraries are 
done with ```-lcurl -lpthread``` arguments.

If it compiles successfuly, run using,

```sh
./weather-dl
```

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
