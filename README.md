# cellular_automata
An Object-Oriented implementation of cellular automata with
applications to vehicular flow and biological flow.

## Welcome!

## What the project is about?
A collection of tools (mostly based on cellular automata) to simulate
phenomena modeles as discrete events.

## Fast installation and starting up!!!

### What you need to have it working nicely?
* A C++ compiler - demo drivers and library built with version
  7.4.0. It may work with previous versions as well.
  
* CMake - to configure and install it. We tested with version 3.10.2.

* Python - to test output from demo drivers with validation files
  (also to produce nice plots). We tested with version 3.7.3 but it
  should work with any version >= 3

#### Optional

* Latex - to build documentation. Most documentation is generated from
  `.org` files.

* Doxygen - to build classes diagrams and useful documentation from
  the code.

* MPI support for parallel features - `not currently supported`.

### How to get your own copy of the project?
You need **git** installed in your computer, then type in a terminal

```shell
git clone https://github.com/tachidok/cellular_automata
cd cellular_automata
git checkout -b john
```

After executing the first line you will be prompted with your github
user name and your password. The third line generates your fully
customised branch and assumes your name is _john_

### How to configure it?
* In a terminal (shell command line) go into the `cellular_automata` folder
  then type

```shell
./autogen.sh
```

* Follow up the instructions on screen to configure your own copy of
the project.

* From here on you can continue with the `starting_up` document in the
`doc` folder.

**For full details and additional features please check the full
documentation in the `doc` folder.**

#### Documentation

If you got doxygen installed in your system then generate the
documentation by typing in a terminal,

```shell
./make_doc.sh
```

the documentation will be automatically generated into the
`doc/doxy_doc/html` folder, then open the `index.html` file in a
web-browser.

## Examples and test cases

Demos, examples or test cases are in the `demos` folder. These demos
are also used to self test the implementation. You should run all of
them to make sure nothing is broken. You can select to perform a full
test at installation time, otherwise type

```shell
./ctest
```
into the specified build folder (the default one is `build`).

A large number of examples is expected to live in the `demos` folder
so check there for any feature available in the project.

## How to contribute

Get your own copy and have fun. Do not hesitate to contact the
developers at :envelope: (jcp.sansalvador.at.inaoep.mx) :envelope:

### Notes for contributors
For full details on contributions please check the documentation in
the `doc` folder. Contact the developers for current in-development
features.

## Facts and curiosities

### How many developers are currently working on the library

At Monday, July/27, 2020 there are 6 developers working in this
project :smile:

:construction: :construction: :construction: :construction: :construction:

### When did this start?
This project was initially uploaded to GitHub on Wednesday, 29 May
2019 :smile:

## Have fun and enjoy .... !!!
