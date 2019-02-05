#include <curses.h>
#include <stdlib.h>
//#define DEBUG
int current_opcode; 
// 15 registers from V0 to VE
// 16th register is used as carry flag
// Address register I stored index. Used for memory operations
unsigned char key = 255;
unsigned char dpflag = 0;
unsigned char v[17];
unsigned short i;
unsigned short pc;

short int display_x = 64;
short int display_y = 32;
/*
4K memory available to CHIP8
Font data stored in 0x000 - 0x200
Programs tend to start from 0x200
 
0xF00- 0xFFF: Display Refresh
0xEA0- 0xEFF: Call stack
*/

unsigned char fontset[0x200] = {0xf0, 0x90, 0x90, 0x90, 0xf0,   //0
				0x20, 0x60, 0x20, 0x20, 0x70,	//1
				0xf0, 0x10,0xf0, 0x80, 0xf0,	//2
				0xf0, 0x10,0xf0, 0x10, 0xf0	//3
				,0x90, 0x90, 0xf0, 0x10, 0x10	//4
				,0xf0, 0x80, 0xf0, 0x10, 0xf0	//5
				,0xf0, 0x80, 0xf0, 0x90, 0xf0	//6
				,0xf0, 0x10, 0x20, 0x40, 0x40   //7
				,0xf0, 0x90, 0xf0, 0x90, 0xf0	//8
				,0xf0, 0x90, 0xf0, 0x10, 0xf0	//9
				,0xf0, 0x90, 0xf0, 0x90, 0x90,	//A
				0xe0, 0x90, 0xe0, 0x90, 0xe0,	//B
				0xf0, 0x80, 0x80, 0x80, 0xf0,	//C
				0xe0, 0x90, 0x90, 0x90, 0xe0,	//D
				0xf0, 0x80, 0xf0, 0x80, 0xf0,	//E
				0xf0, 0x80, 0xf0, 0x80, 0x80}	//F
				; 
unsigned char memory[4096];
 
//Stack
 
unsigned short stack[16];
unsigned short sp; 
//Display
// Has only two states: ON/OFF. Hence, it is black and white.
wchar_t states[2] = {' ',0x2588};
unsigned char display[128][64];

unsigned char dt;
unsigned char st;
void fillFont(){
for (int i = 0; i < 0x200; i++){
memory[i] = fontset[i];
}
}
void decodeOp(unsigned short fetchedOpcode, FILE* f){
#ifdef DEBUG
//FILE* f = fopen("new.txt","a");
#endif
current_opcode = fetchedOpcode;
switch (fetchedOpcode){
	case 0x00e0:
		sleep(1);
		for (int i = 0; i < display_x; i++){
			for (int j = 0; j < display_y; j++){
				display[i][j] = 0;
			}
		}
		#ifdef DEBUG
		fprintf(f,"cls\n");
		#endif
		dpflag ^= 1;
		return;
	case 0x00ee:
		#ifdef DEBUG
		puts("ret\n");
		#endif
		pc = stack[sp-1];
		sp--;
		return;
	}
unsigned short opcode = fetchedOpcode >> 12;

unsigned short fh = fetchedOpcode >> 8;
unsigned short sh = fetchedOpcode << 8;
sh >>= 8;
unsigned char firsthalf = fh;
unsigned char secondhalf = sh;

unsigned char secbyte = (firsthalf << 4);
secbyte >>= 4;

unsigned char seclastbyte = secondhalf >> 4;
unsigned char lastbyte = secondhalf << 4;
lastbyte >>= 4;
unsigned short lastthreebyte = (fetchedOpcode << 4);
lastthreebyte >>= 4;
//fprintf(f,"Opcode (%x): ",fetchedOpcode); 
switch (opcode){
	case 0x1:
		//Jump to location NNN
		#ifdef DEBUG
		fprintf(f,"jp %p\n\n",lastthreebyte);
		#endif
		pc = lastthreebyte - 0x2;
		return;
	case 0x2:
		#ifdef DEBUG
		fprintf(f,"call %p\n\n",lastthreebyte);
		#endif
		sp++;
		stack[sp-1] = pc;
		pc = lastthreebyte - 0x2;
		return;
	case 0x3:
		#ifdef DEBUG
		fprintf(f,"se V%x, %p\n",secbyte,secondhalf);
		#endif
		if (v[secbyte] == secondhalf){
			pc += 2;
		}
		return;
	case 0x4:
		#ifdef DEBUG
		fprintf(f,"sne V%x, %p\n", secbyte, secondhalf);
		#endif
		if ((v[secbyte] ^ secondhalf)){
			pc += 2;
		}
		return;
	case 0x5:
		if (lastbyte == 0){
		#ifdef DEBUG
		fprintf(f,"se V%x, V%x",secbyte,seclastbyte);
		#endif
		if (v[secbyte] == v[seclastbyte]){
			pc += 2;
		}
		}
		return;
	case 0x6:
		#ifdef DEBUG
		fprintf(f,"ld V%x, %p\n",secbyte, secondhalf);
		#endif 
		v[secbyte] = secondhalf;
		return;
	case 0x7:
		#ifdef DEBUG
		fprintf(f,"add V%x, %p\n",secbyte, secondhalf);
		#endif
		v[secbyte] += secondhalf;
		return;
	case 0x8:
		switch (lastbyte){
			case 0x0:
				#ifdef DEBUG
				fprintf(f,"ld V%x,V%x\n",secbyte,seclastbyte);
				#endif
				v[secbyte] = v[seclastbyte];
				return;
			case 0x1:
				#ifdef DEBUG
				fprintf(f,"or V%x, V%x\n", secbyte, seclastbyte);
				#endif
				v[secbyte] |= v[seclastbyte];
				return;
			case 0x2:
				#ifdef DEBUG
				fprintf(f,"and V%x, V%x\n", secbyte, seclastbyte);
				#endif 
				v[secbyte] &= v[seclastbyte];
				return;
			case 0x3:
				#ifdef DEBUG
				fprintf(f,"xor V%x, V%x\n", secbyte, seclastbyte);
				#endif
				v[secbyte] ^= v[seclastbyte];
				return;
			case 0x4:
				#ifdef DEBUG
				fprintf(f,"add V%x, V%x\n",secbyte, seclastbyte);
				#endif
				v[secbyte] += v[seclastbyte];
				return;
			case 0x5:
				#ifdef DEBUG
				fprintf(f,"sub V%x, V%x\n",secbyte, seclastbyte);
				#endif
				if (v[seclastbyte] > v[secbyte]){
					v[0xf] = 0x0;
				}
				else {
					v[0xf] = 0x1;
				}
				v[secbyte] -= v[seclastbyte];
				return;
			case 0x6:
				#ifdef DEBUG
				fprintf(f,"shr V%x, V%x\n",secbyte,seclastbyte);
				#endif
				v[0xf] = v[secbyte] & 1;
				v[secbyte] >>= 1;
				return;
			case 0x7:
				#ifdef DEBUG
				fprintf(f,"subn V%x, V%x\n",secbyte, seclastbyte);
				#endif
				if (v[secbyte] > v[seclastbyte]){
					v[0xf] = 0x0;
				}
				else{
					v[0xf] = 0x1;
				}
				v[secbyte] = v[seclastbyte] - v[secbyte];
				return;
			case 0xe:
				v[0xf] = v[secbyte] >> 7;
				#ifdef DEBUG
				fprintf(f,"shl V%x, V%x\n",secbyte, seclastbyte);
				#endif
				v[secbyte] = v[secbyte] << 1;
				return;
				}
	case 0x9:
		#ifdef DEBUG
		fprintf(f,"sne V%x, V%x",secbyte, seclastbyte);
		#endif
		if (v[secbyte] != v[seclastbyte]){
			pc += 2;
		}
		return;
	case 0xa:
		#ifdef DEBUG
		fprintf(f,"ld I, %p\n",lastthreebyte);
		#endif
		i = lastthreebyte;
		return;
	case 0xb:
		#ifdef DEBUG
		fprintf(f,"jp V0, %x\n",lastthreebyte);
		#endif
		pc = v[0] + lastthreebyte - 0x2;	
		return;
	case 0xc:
		#ifdef DEBUG
		fprintf(f,"rnd V%x, %p\n",secbyte,secondhalf);
		#endif
		v[secbyte] = rand() % 0xff;
		v[secbyte] &= secondhalf;
		return;
	case 0xd:
		v[0xf] = 0x0;
		#ifdef DEBUG
		fprintf(f,"drw V%x, V%x, %p\n",secbyte, seclastbyte, lastbyte);
		#endif
		for (unsigned char y = 0; y < lastbyte; y++){
			int count = 0;
			for (unsigned char x = 7; x !=0; x--){
				unsigned char prev = display[((v[secbyte])+count)%display_x][(v[seclastbyte]+y)%display_y];
				unsigned char* ptr = &display[((v[secbyte])+count)%display_x][(v[seclastbyte]+y)%display_y];
				display[(v[secbyte]+count)%display_x][(v[seclastbyte] + y)%display_y] ^= ((memory[i+y]) >> (x)) & 1; 
				count++;
				if (prev != *ptr && prev == 1){
					v[0xf] = 1;
				}
			}
		}
		dpflag ^= 1;
		return;	
	case 0xe:
		switch (secondhalf){
			case 0x9e:
				{
				;
				nodelay(stdscr,TRUE);
				unsigned char x= getch();
				if (x - '0' == v[secbyte] || (x - 'a' + 0xa == v[secbyte])){
					pc += 2;
				}
				flushinp();
				nodelay(stdscr,FALSE);
				return;
				break;
				}
			case 0xa1:
				;
				{
				nodelay(stdscr, TRUE);
				unsigned char c= getch();
				if (c - '0' != v[secbyte]){
					pc += 2;
				}
				nodelay(stdscr,FALSE);
				return;
				}
		}	
	case 0xf:
		switch (secondhalf){
			case 0x07:
				#ifdef DEBUG
				fprintf(f,"ld V%x, DT\n",secbyte);
				#endif
				v[secbyte] = dt;
				return;
			case 0xa:
				#ifdef DEBUG
				fprintf(f,"ld V%x, K\n",secbyte);
				#endif
				flushinp();
				v[secbyte] = getch();
				if (v[secbyte] >= '0' && v[secbyte] <= '9'){
				v[secbyte] -= '0';
				}
				else{
				v[secbyte] -= 97;
				v[secbyte] += 0xa;
				}
				return;
			case 0x15:
				#ifdef DEBUG
				fprintf(f,"ld DT, V%x\n",secbyte);
				#endif
				dt = v[secbyte];
				return;
			case 0x18:
				#ifdef DEBUG
				fprintf(f,"ld ST, V%x\n",secbyte);
				#endif
				st = v[secbyte];
				return;
			case 0x1e:
				#ifdef DEBUG
				fprintf(f,"add I, V%x\n",secbyte);
				#endif
				if (i + v[secbyte] > 0xFFF){
					v[0xf] = 0x1;
				}
				else{
					v[0xf] = 0x0;
				}
				i += v[secbyte];
				return;
			case 0x29:
				#ifdef DEBUG
				fprintf(f,"ld f, V%x\n",secbyte);
				#endif
				i = (5 * v[secbyte]);
				return;	
			
			case 0x33:
				#ifdef DEBUG
				fprintf(f,"ld B, V%x\n", secbyte);
				#endif
				memory[i] = (v[secbyte] / 100);
				memory[i+1] = (v[secbyte] % 100);
				memory[i+1] /= 10;
				memory[i+2] = (v[secbyte] % 10);
				return;
			case 0x55:
				#ifdef DEBUG
				fprintf(f,"ld [I], V%x\n", secbyte);
				#endif
				for (int V = 0; V <= secbyte; V++){
				memory[i+V] = v[V];
				}
				return;
				//i = i + secbyte + 1;
			case 0x65:
				#ifdef DEBUG
				fprintf(f,"ld V%x, [I]\n",secbyte);
				#endif
				for (int V = 0; V <= secbyte; V++){
				v[V] = memory[i + V];
				}
				//i = i + secbyte + 1;
				return;
				}
		}
	
	endwin();
	printf("Unknown opcode: %p\n",fetchedOpcode);
	exit(1);
}
