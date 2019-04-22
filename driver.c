#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include "chip8core.h"
#include <curses.h>
#include <signal.h>
#include <pthread.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void windcall(GLFWwindow* win, int width, int height){
	glViewport(0,0,width,height);
	puts("Window size changing");
}

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

void displayScreen(int VBO, int prog){
float xoffset = 2.0/((float) display_x);
float yoffset = 2.0/((float) display_y);

float vertices[] = {-1.0f,1.0f,
		    -1.0f+xoffset, 1.0f,
		    -1.0f, 1.0f-yoffset,
		     -1.0f+xoffset, 1.0f-yoffset};

int y = 0;
while (y < display_y){
int x = 0;
while (x < display_x){
if (display[x][y] == 1){
glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STREAM_DRAW);
glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}
int i = 0;
while (i < 8){
vertices[i] += xoffset;
i += 2;
}
x++;
}

int i = 1;
while (i < 8){
vertices[i] = vertices[i] - yoffset;
i += 2;
}

vertices[0] = -1.0;
vertices[2] = -1.0+xoffset;
vertices[4] = -1.0;
vertices[6] = -1.0+xoffset;
y++;
}
}
void getSource(char* filename, int shader){
	FILE* f = fopen(filename,"r");
	if (!f){
		printf("Missing file: %s",filename);
		exit(-1);
	}
	char buffer[512];
	char ch = getc(f);
	int index = 0;
	while (ch != EOF){
		buffer[index] = ch;
		ch = getc(f);
		index++;
	}
	buffer[index] = 0;
	const char* data = buffer;
	glShaderSource(shader,1,&data,NULL);
}

void compileSource(int shader){
glCompileShader(shader);

int successStatus, shaderType;
glGetShaderiv(shader,GL_COMPILE_STATUS,&successStatus);
glGetShaderiv(shader,GL_SHADER_TYPE,&shaderType);

if (successStatus == GL_FALSE){
	puts("Shader Compilation failed..");
	exit(-1);
}
}
int main(int argc, char *argv[]){

puts("Initializing emulated CPU");

srand(time(0));
signal(SIGSEGV, handler);

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

puts("Initializing OpenGL Renderer");

glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);

GLFWwindow* win = glfwCreateWindow(1920,1080,"CHIP8 Emulator",NULL, NULL);
glfwMakeContextCurrent(win);

glewInit();

glfwSetWindowSizeCallback(win, windcall);

int vertexShader = glCreateShader(GL_VERTEX_SHADER);
int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

getSource("vertex.glsl",vertexShader);
getSource("fragment.glsl",fragmentShader);

compileSource(vertexShader);
compileSource(fragmentShader);

int prog = glCreateProgram();
glAttachShader(prog,vertexShader);
glAttachShader(prog,fragmentShader);

glLinkProgram(prog);
int linkStatus;
glGetProgramiv(prog,GL_LINK_STATUS, &linkStatus);

if (linkStatus == GL_FALSE){
	puts("Linking failed..");
	char data[512];
	glGetProgramInfoLog(prog,512,NULL,data);
	puts("INFO LOG: ");
	printf("%s\n",data);
	return -1;
}
puts("Shaders compiled and linked");

int VBO;
glGenBuffers(1,&VBO);
glBindBuffer(GL_ARRAY_BUFFER,VBO);

int VAO;
glGenVertexArrays(1,&VAO);
glBindVertexArray(VAO);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, sizeof(float) *2, (void*)0);

glUseProgram(prog);
glfwSetWindowSizeCallback(win, windcall);

while (1){
emulateCycle();

glClearColor(0.1,0.1,0.1,1.0);
glClear(GL_COLOR_BUFFER_BIT);
//if (dpflag){
displayScreen(VBO,prog);
glfwSwapBuffers(win);
//}


}
//usleep(2000);
}
