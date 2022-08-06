# The UDP chatter package

## Overview
This package provides a UDP based chat server-client on Linux. It uses the boost library to setup sockets to publish / receive messages across ports. This has been tested on Ubuntu 18.04.

## Features
Following features are available -
* Uses modern C++ (14)
* Separate IP addresses for sending and receiving messages
* Separate ports for listening and sending messages
* Command line tool for publishing messages to specified IP and port
    * Ability to set IP and port to publish to
    * Ability to publish a single message or continuously at specified rate

## Future work
* Command line tool for receiving messages
* GUI for chatting
* Documentation
* Extend to using as ROS pkg

## Installation / Compilation
This package is built on cmake, and has a `CmakeLists.txt` to build the executable. In order to compile and build the executable, run the following commands -
```bash
mkdir build; cd build
cmake ..
make
sudo make install  # Optional if you want to install to your lib path
```

## Usage
This section walks through using the tools available in this package.

### udp_sender
Use the command line tool, `udp_sender` to send udp messages. If no arguments are specified, a help message is displayed -
```bash
./udp_sender -h
Command line tool for sending udp messages, usage: ./udp_sender [args...] -- msg:
  -h [ --help ]                     Display this help message and exit
  -v [ --version ]                  Display the version number
  -r [ --rate ] arg                 Data publish rate in hz
  -i [ --ip_addr ] arg (=127.0.0.1) IP address to publish messages to
  -p [ --port ] arg (=10001)        Port to publish messages to
  -1 [ --once ] [=arg(=stdin)]      Publish one message and exit
  --msg arg                         Message to be published
```

To publish a message once, to the default ip and port (`127.0.0.1:10001`), run -
```bash
./udp_sender -1 -- hello world
```

To publish a message at a specified rate (in hz), to the default ip and port (`127.0.0.1:10001`), run -
```bash
./udp_sender -r 1 -- hello world
```

To publish a message to a different ip and port, run -
```bash
./udp_sender 1 -p 10002 -i 192.168.3.12  -- Hello world
```

The `udp_chatter` executable is a server / client used as a template.

## License
Copyright (c) 2021-2022 srsidd. Not to be distributed or used without prior approval.
