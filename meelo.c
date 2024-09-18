//***INCLUDES
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
//***DEFINES
#define CTRL_KEY(k) ((k) & 0x1f) // doing bitwise AND with 0x1f which is equivalent to(00011111. This is done to clear the 8th bit added 
//to the original ASCII value of key when Ctrl key is pressed (The 8th bit represents Ctrl key bit.) 
//This ensures to keep the ASCII of last 5 bits which also supports older systems

//COOKED MODE : The input is only sent to the program when the user presses "Enter" key. (also called canonical mode)
//***DATA
struct editorConfig{
    struct termios orig_termios;
};

struct editorConfig E;
//***TERMINAL
void die(char *s){
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(s);
    exit(1); 
}
char editorReadKey(){
    int nread;
    char c;
    while((nread = read(STDIN_FILENO, &c, 1)) != 1){
        if(nread == -1 && errno != EAGAIN) die("read");
    }
    return c;
}
void disableRawMode(){
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, & E.orig_termios) == -1){
        die("tcsetattr");
    }
}
void enableRawMode(){
    struct termios raw; //this declares a raw struct that stores terminal settings
    if(tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");// gets the current attributes of the terminal settings
    atexit(disableRawMode);
     
    raw = E.orig_termios;
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | ISTRIP | INPCK);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 5;
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

// *** INPUT ****
void editorProcessKeyPress(){
    char c = editorReadKey();
    switch(c){
        case CTRL_KEY('q'): 
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0); 
        break;
    }
}
// ****OUTPUT
void editorDrawRows(){
    for(int y = 0; y < 24; y++){
        write(STDOUT_FILENO, "~\r\n", 3);
    }
}
void editorRefreshScreen(){
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    editorDrawRows();
    write(STDOUT_FILENO, "\x1b[H", 3);
}
//***INIT
int main(){
    enableRawMode();
    char c; 
    while(1){
        editorRefreshScreen();
        editorProcessKeyPress();
    } 
    // quits when the 'q' character is read.
    // read function takes 3 args. an int type args which is stdin_fileno, stdout_fileno and error,
    //second argument is the void *buf which is the buffer to store data (address of the variable to store data)
    //third argument is the size_t that defines the number of bytes to read.

    return 0;
}
// Tildes