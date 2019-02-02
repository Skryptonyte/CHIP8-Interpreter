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
	//puts("Unable to load game..");
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
decodeOp(fetch);

pc += 2;
}
void displayScreen(){;
//static unsigned long int counter = 0;
//clear() 
refresh();
for (int j = 0; j < 32; j++){

for (int i = 0; i < 64; i++){
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
signal(SIGSEGV, handler);
setlocale(LC_CTYPE,"");
initscr();
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
for (;;){
emulateCycle();
//decodeOp(0xA00a);
//decodeOp(0xD005);
usleep(2000);
if (dpflag){
displayScreen();
dpflag ^= 1;
}
if (dt > 0 && (cycle % 60 == 0)){
	dt--;
}
cycle++;
}
}
