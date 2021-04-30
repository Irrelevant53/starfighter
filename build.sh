export LD_LIBRARY_PATH=/home/mx/Desktop/starfighter/lib:$LD_LIBRARY_PATH

gcc -o starfighter -Wall -Wextra /home/mx/Desktop/starfighter/src/animations.c /home/mx/Desktop/starfighter/src/enemy.c /home/mx/Desktop/starfighter/src/particles.c /home/mx/Desktop/starfighter/src/bullet.c /home/mx/Desktop/starfighter/src/files.c /home/mx/Desktop/starfighter/src/player.c /home/mx/Desktop/starfighter/src/ui.c /home/mx/Desktop/starfighter/src/game.c /home/mx/Desktop/starfighter/src/camera.c  -L/home/mx/Desktop/starfighter/lib -lraylib -lpthread -lm -ldl -I/home/mx/Desktop/starfighter/include
./starfighter
