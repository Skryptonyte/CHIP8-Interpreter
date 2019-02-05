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
//puts("Loading into memory");
unsigned char ch = getc(fp);
int size = 0;
for (int i = 512; i < 4096; i++){
	memory[i] = ch;
	//printf("%c",memory[i+512]);
	size += 1;
	ch = getc(fp);
}
//printf("Read game into memory, Size: %d bytes\n",size);
//puts(memory+512);
}

void emulateCycle(){
//Fetch opcode at program counter
unsigned short fetch = (memory[pc] << 8) | memory[pc+1];
//Decode opcode
//printf("Memory: %p, Opcode: %p, Decoded Instruction: ",pc, fetch);
FILE* f = fopen("new.txt","a");
decodeOp(fetch, f);
#ifdef DEBUG
fclose(f);
#endif
pc += 2;
}
void displayScreen(){;
//static unsigned long int counter = 0;
//clear() 
refresh();
for (int j = 0; j < display_y; j++){

for (int i = 0; i < display_x; i++){
mvprintw(j,i*2,"%lc%lc",states[display[i][j] % 2],states[display[i][j]%2]);
//mvprintw(j, i+1,"%lc",states[display[i][j] % 2],states[display[i][j]%2]);
//mvprintw(j, i+2,"%lc",states[display[i][j] % 2],states[display[i][j]%2]);
}
//sleep(1);
}
refresh();
//sleep(1);
}
int main(int argc, char *argv[]){
for (int i =2 ; i < argc; i++){
if (!strcmp(argv[i],"--hires")){
display_y = 64;
}
if (!strcmp(argv[i],"--schip8")){
display_x = 128;
display_y = 64;
}
}
signal(SIGSEGV, handler);
//signal(SIGINT, handler);
setlocale(LC_CTYPE,"");
initscr();
WINDOW* win = newwin(15,48,0, display_x*2 +3);
//box(win,0,0);
beep();
sleep(1);
getch();
//resize_term(1920,1080);
//puts("WIP CHIP8 emulator");
initialize();;
fillFont();
if (argc >= 2){
	loadGame(argv[1]);
}
else{
	//puts("No game specified...");
}
int cycle = 1;
//processInput();
for (;;){
emulateCycle();
//decodeOp(0xA00a);
//decodeOp(0xD005);
if (dpflag){
displayScreen();
dpflag ^= 1;
}
if (dt > 0 && (cycle % 60 == 0)){
	dt--;
}
if (st > 0 && (cycle % 60 == 0)){
	st--;
}
cycle++;
werase(win);
box(win,0,0);
mvwprintw(win,0,3,"Registers");
for (int i =0; i <= 7; i++){
mvwprintw(win,i+2,1,"V%x : %d",i,v[i]);
mvwprintw(win,i+2, 20 ,"V%x : %d",8+i,v[8+i]);
}
mvwprintw(win,12,1,"Program Counter: %p   I: %p",pc, i);
mvwprintw(win,13,1,"DT: %d    ST: %d",dt,st);
wrefresh(win);
usleep(1000);
}
}
