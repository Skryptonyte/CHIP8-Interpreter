#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include "chip8core.h"
#include <curses.h>
#include <signal.h>
#include <pthread.h>

void* delayHandler(void* meh){
while (1){
if (dt > 0){
dt--;
}
if (st > 0){
st--;
}
usleep(16666);
}
return NULL;
}
void* inputHandler(void* meh){
while (0){
nodelay(stdscr,TRUE);
unsigned short x= getch();
x = x -'0';
if ( 0 <= x && x <= 9){
        keyb = x;
     lastkey = x;
}
usleep(20);
}
}
void handler(int sig){
sleep(1);
endwin();
puts("Invalid Memory Access..");
printf("Offending OpCode: %x\nProgram Counter: %x\n",current_opcode, pc);
puts("Registers: ");
for (int i = 0; i <= 0xf; i++){
printf("V%x: %d\n",i,v[i]);
}
printf("I: %x, PC = %x\n", i, pc);
exit(sig);
}
void initialize(){
//puts("Initializing system..");
memset(v,0, 17*sizeof(unsigned char));
pc = 0x200;
sp = 0;
current_opcode = 0;
i = 0;
dpflag = 0;
keyb = 0;

dt = 0;
st = 0;
}

void loadGame(char* c){
FILE* fp = fopen(c,"rb");

if (!fp){
	endwin();
	puts("Game not found... exitting");
	exit(1);
	return;
}
unsigned char ch = getc(fp);
int size = 0;
for (int i = 512; i < 4096; i++){
	memory[i] = ch;
	size += 1;
	ch = getc(fp);
}
}

void emulateCycle(){
unsigned short fetch = (memory[pc] << 8) | memory[pc+1];
decodeOp(fetch);
pc += 2;
}
void displayScreen(){; 
refresh();
for (int j = 0; j < display_y; j++){

for (int i = 0; i < display_x; i++){
mvprintw(j,i*2,"%lc%lc",states[display[i][j] % 2],states[display[i][j]%2]);
}
}
refresh();
}

int main(int argc, char *argv[]){
srand(time(0));
signal(SIGSEGV, handler);
setlocale(LC_CTYPE,"");

initscr();
start_color();
WINDOW* win = newwin(15,51,0, display_x*2 +3);

init_pair(1,COLOR_GREEN, COLOR_BLACK);
wattron(stdscr,COLOR_PAIR(1));

initialize();
fillFont();
loadGame(argv[1]);
pthread_t pid, pid2;
pthread_create(&pid,NULL,delayHandler,NULL);
pthread_create(&pid2,NULL,inputHandler,NULL);
int cycle = 1;

if ((memory[pc] << 8 | memory[pc+1]) == 0x1260){
display_x = 64;
display_y = 64;
pc = 0x2c0 - 2;
}

for (;;){
nodelay(stdscr,TRUE);

/*
unsigned short x= getch();
x = x -'0';
if ( 0 <= x && x <= 9){
	keyb = x;
     lastkey = x;
}
}
*/
emulateCycle();
if (dpflag){
displayScreen();
dpflag ^= 1;
}
werase(win);
mvwin(win,0,display_x*2+3);
box(win,0,0);
mvwprintw(win,0,3,"Registers");
for (int i =0; i <= 7; i++){
mvwprintw(win,i+2,1,"V%x : %d",i,v[i]);
mvwprintw(win,i+2, 20 ,"V%x : %d",8+i,v[8+i]);
}
mvwprintw(win,11,1,"Key: %d",lastkey);
mvwprintw(win,12,1,"Program Counter: %p   I: %p  Opcode: %p",pc, i,current_opcode);
mvwprintw(win,13,1,"DT: %d    ST: %d",dt,st);
wrefresh(win);
usleep(1000);

}
}
