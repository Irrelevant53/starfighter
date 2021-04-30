export LD_LIBRARY_PATH=/home/mx/Desktop/Starfighter/lib:$LD_LIBRARY_PATH

gcc -o starfighter -Wall -Wextra /home/mx/Desktop/Starfighter/src/animations.c /home/mx/Desktop/Starfighter/src/enemy.c /home/mx/Desktop/Starfighter/src/particles.c /home/mx/Desktop/Starfighter/src/bullet.c /home/mx/Desktop/Starfighter/src/files.c /home/mx/Desktop/Starfighter/src/player.c /home/mx/Desktop/Starfighter/src/ui.c /home/mx/Desktop/Starfighter/src/game.c /home/mx/Desktop/Starfighter/src/camera.c  -L/home/mx/Desktop/Starfighter/lib -lraylib -lpthread -lm -ldl -I/home/mx/Desktop/Starfighter/include
./starfighter
