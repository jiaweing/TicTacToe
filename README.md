# Compile

### Windows

```
gcc -std=c17 ttt.c -IC:\Users\User\Desktop\Desktop\SIT\CSC1103\Project\SDL2\include -LC:\Users\User\Desktop\Desktop\SIT\CSC1103\Project\SDL2\lib -Wall -lmingw32 -lSDL2main -lSDL2 -o ttt
```

### Mac

```
gcc -o ttt ttt.c -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -rpath /Library/Frameworks
```

### Linux

```
gcc -o ttt ttt.c -lSDL2
```
