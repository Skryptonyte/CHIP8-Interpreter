#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned char keyb = 0xfe;
unsigned char lastkey;
unsigned char key = 255;

int current_opcode;

unsigned char dpflag = 0;

unsigned char v[17];
unsigned char rpl[8];

unsigned short i;
unsigned short pc;

short int display_x = 64;
short int display_y = 32;

int y_wrap = 1;
unsigned char fontset[0x5 * 16] = {0xf0, 0x90, 0x90, 0x90, 0xf0,   //0
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
unsigned char hfontset[0x10 * 10] = {
0x7C, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7C, 0x00,
0x08, 0x18, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3C, 0x00,
0x7C, 0x82, 0x02, 0x02, 0x04, 0x18, 0x20, 0x40, 0xFE, 0x00,
0x7C, 0x82, 0x02, 0x02, 0x3C, 0x02, 0x02, 0x82, 0x7C, 0x00,
0x84, 0x84, 0x84, 0x84, 0xFE, 0x04, 0x04, 0x04, 0x04, 0x00,
0xFE, 0x80, 0x80, 0x80, 0xFC, 0x02, 0x02, 0x82, 0x7C, 0x00,
0x7C, 0x82, 0x80, 0x80, 0xFC, 0x82, 0x82, 0x82, 0x7C, 0x00,
0xFE, 0x02, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20, 0x20, 0x00,
0x7C, 0x82, 0x82, 0x82, 0x7C, 0x82, 0x82, 0x82, 0x7C, 0x00,
0x7C, 0x82, 0x82, 0x82, 0x7E, 0x02, 0x02, 0x82, 0x7C, 0x00,
0x10, 0x28, 0x44, 0x82, 0x82, 0xFE, 0x82, 0x82, 0x82, 0x00,
0xFC, 0x82, 0x82, 0x82, 0xFC, 0x82, 0x82, 0x82, 0xFC, 0x00,
0x7C, 0x82, 0x80, 0x80, 0x80, 0x80, 0x80, 0x82, 0x7C, 0x00,
0xFC, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0xFC, 0x00,
0xFE, 0x80, 0x80, 0x80, 0xF8, 0x80, 0x80, 0x80, 0xFE, 0x00,
0xFE, 0x80, 0x80, 0x80, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x00
};
unsigned char memory[4096];

unsigned short stack[16];
unsigned short sp;

wchar_t states[2] = {' ',0x2588};
unsigned char display[128][64];

unsigned char dt;
unsigned char st;
void fillFont(){
for (int i = 0; i < 0x200; i++){
memory[i] = fontset[i];
}


for (int i = 0; i < 0x200; i++){
memory[i + 0x5*16] = hfontset[i];
}
}
void decodeOp(unsigned short fetchedOpcode){
current_opcode = fetchedOpcode;

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

switch (opcode){
	case 0x0:
		switch (seclastbyte){
		case 0xc:
		for (int j = display_y-1; j >= lastbyte; j--)
		{
			for (int i = 0; i < display_x; i++)
			{
				display[i][j] = display[i][j-lastbyte];

			}
		}

		for (int j = 0; j < lastbyte; j++)
		{
			for (int i =0; i < display_x; i++)
			{
				display[i][j] = 0;
			}
		}

			return;
		}
		{
		switch (secondhalf){
			case 0xe0:
				for (int i = 0; i < display_x; i++){
					for (int j = 0; j < display_y; j++){
						display[i][j] = 0;
					}
				}
				dpflag ^= 1;
				return;
			case 0xee:
				pc = stack[sp-1];
				sp--;
				return;
			case 0xfb:

					for (int i = display_x-1;i >= 4; i--){
						for (int j = 0; j < display_y; j++){
						display[i][j] = display[i-4][j];
						}
					}

					for (i = 0; i < 4; i++){
						for (int j = 0; j < display_y; j++)
						{
							display[i][j] = 0;
						}
					}
					return;
			case 0xfc:
					for (int i = 0; i < display_x-4; i++)
					{
						for (int j = 0; j < display_y; j++)
						{
							display[i][j] = display[i+4][j];
						}
					}


					for (int i =display_x-4-1; i < display_x;i++)
					{
						for (int j = 0; j < display_y; j++)
						{
							display[i][j] = 0;
						}
					}
					return;
			case 0xfd:

				puts("Exit requested by program..");
				exit(0);
				return;
			case 0xfe:
				display_x = 64;
				display_y = 32;
				return;
			case 0xff:
				display_x = 128;
				display_y = 64;
				return;
			default:
				return;
		}
		}
	case 0x1:
		pc = lastthreebyte - 0x2;
		return;
	case 0x2:
		sp++;
		stack[sp-1] = pc;
		pc = lastthreebyte - 0x2;
		return;
	case 0x3:
		if (v[secbyte] == secondhalf){
			pc += 2;
		}
		return;
	case 0x4:
		if ((v[secbyte] ^ secondhalf)){
			pc += 2;
		}
		return;
	case 0x5:
		if (lastbyte == 0){
		if (v[secbyte] == v[seclastbyte]){
			pc += 2;
		}
		}
		return;
	case 0x6:
		v[secbyte] = secondhalf;
		return;
	case 0x7:
		v[secbyte] += secondhalf;
		return;
	case 0x8:
		switch (lastbyte){
			case 0x0:
				v[secbyte] = v[seclastbyte];
				return;
			case 0x1:
				v[secbyte] |= v[seclastbyte];
				return;
			case 0x2:
				v[secbyte] &= v[seclastbyte];
				return;
			case 0x3:
				v[secbyte] ^= v[seclastbyte];
				return;
			case 0x4:
				if (v[secbyte] + v[seclastbyte] > 255){
					v[0xf] = 1;
				}
				v[secbyte] += v[seclastbyte];
				return;
			case 0x5:
				if (v[seclastbyte] > v[secbyte]){
					v[0xf] = 0x0;
				}
				else {
					v[0xf] = 0x1;
				}
				v[secbyte] -= v[seclastbyte];
				return;
			case 0x6:
				v[0xf] = v[secbyte] & 1;
				v[secbyte] >>= 1;
				return;
			case 0x7:
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
				v[secbyte] = v[secbyte] << 1;
				return;
				}
	case 0x9:
		if (v[secbyte] != v[seclastbyte]){
			pc += 2;
		}
		return;
	case 0xa:
		i = lastthreebyte;
		return;
	case 0xb:
		pc = v[0] + lastthreebyte - 0x2;
		return;
	case 0xc:
		v[secbyte] = rand() % 0xff;
		v[secbyte] &= secondhalf;
		return;
	case 0xd:
		v[0xf] = 0x0;
		unsigned char bitcount = 7;
		unsigned char inc = 1;
		if (lastbyte == 0){
			bitcount = 15;
			lastbyte = 16;
		}
		int ycount = 0;
		for (unsigned char y = 0; y < lastbyte; y++){
			unsigned short sprite = (memory[i +ycount] << 8)|memory[i+ycount+1];
			int count = 0;
			for (char x = bitcount; x >=0; x--){
				int y_coordinate = v[seclastbyte] + y;
				if (y_wrap)
				{
				   y_coordinate = y_coordinate%display_y;
			  }
				else{
						y_coordinate = y_coordinate <0 ? (y_coordinate >= display_y?display_y-1:y_coordinate):0;
				}
				unsigned char prev = display[((v[secbyte])+count)%display_x][y_coordinate];
				unsigned char* ptr = &display[((v[secbyte])+count)%display_x][y_coordinate];
				if (bitcount == 7){
				display[(v[secbyte]+count)%display_x][y_coordinate] ^= ((memory[i+y]) >> (x)) & 1;
				}
				else if (bitcount == 15){
				unsigned short b = (sprite >> x) & 1;
				display[(v[secbyte]+count)%display_x][y_coordinate] ^= b;
				}
				count++;
				if (prev != *ptr && prev == 1){
					v[0xf] = 1;
				}
			}
			ycount += 2;
		}
		dpflag ^= 1;
		return;
	case 0xe:
		switch (secondhalf){
			case 0x9e:
				{
				if (v[secbyte] == keyb){
					pc += 2;
				}
				return;
				}
			case 0xa1:
				{
				if (v[secbyte] != keyb){
					pc += 2;
				}
				return;
				}
	}
	case 0xf:
		switch (secondhalf){
			case 0x07:
				v[secbyte] = dt;
				return;
			case 0xa:

				glfwWaitEvents();

				
				if (keyb >= 0 && keyb <= 15){
					v[secbyte] = keyb;
					break;
				}
				

				return;
			case 0x15:
				dt = v[secbyte];
				return;
			case 0x18:
				st = v[secbyte];
				return;
			case 0x1e:
				if (i + v[secbyte] > 0xFFF){
					v[0xf] = 0x1;
				}
				else{
					v[0xf] = 0x0;
				}
				i += v[secbyte];
				return;
			case 0x29:
				i = (5 * v[secbyte]);
				return;
			case 0x30:
				i = ((16*0x5)+ 10 * v[secbyte]);
				return;
			case 0x33:
				memory[i] = (v[secbyte] / 100);
				memory[i+1] = (v[secbyte] % 100);
				memory[i+1] /= 10;
				memory[i+2] = (v[secbyte] % 10);
				return;
			case 0x55:
				for (int V = 0; V <= secbyte; V++){
				memory[i+V] = v[V];
				}
				return;
			case 0x65:
				for (int V = 0; V <= secbyte; V++){
				v[V] = memory[i + V];
				}
				return;
			case 0x75:
				for (int V = 0; V <= secbyte && V <= 7; V++){
					rpl[V] = v[V];
				}
				return;
			case 0x85:
				for (int V = 0; V <= secbyte && V <= 7; V++){
					v[V] = rpl[V];
				}
				return;
			default:
				return;
			}
		}

}
