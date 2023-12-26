# Rush Hour Solver

Dependencies:
```
ncurses
```

To build:
```
g++ -std=c++20 -o rush_hour *.cpp -I. -lncurses
```

Then, run with
```
./rush_hour
```

Also try compiling with `-O3` optimization option,
```
g++ -std=c++20 -O3 -o rush_hour *.cpp -I. -lncurses
```
for a nice speedup (~2.5x on my machine).

