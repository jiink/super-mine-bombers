gcc main.c roundState.c matchState.c roundRender.c matchRender.c inputState.c hex.c -o main.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm && main.exe