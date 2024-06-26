# Desription
This is a simple mathematical expression parser written for the purpose of learning how an LL parser works. An example of how this parser works:
```
$ ./build/parser 
(10 + 5) * (20 / 4) - (8 + 2) * 3
Parsing was finished
└──-
    ├──*
    │   ├──+
    │   │   ├──10
    │   │   └──5
    │   └──/
    │       ├──20
    │       └──4
    └──*
        ├──+
        │   ├──8
        │   └──2
        └──3
The result is: 45
```

# How to build
To build this program, just run `runBuild.sh`
```
chmod +x runBuild.sh
./rubBuild.sh
```

# How to run
There are two possible ways for running it. First one:
```
./build/parser 1+2
```
Second:
```
./parser/parser
```
And then you need to enter your expression.
