#include <unistd.h>
#include <ctype.h>
#include<errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
//COOKED MODE : The input is only sent to the program when the user presses "Enter" key. (also called canonical mode)
struct termios original_termios;
void disableRawMode(){
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1){
        die("tcsetattr");
    }
}
void die(char *s){
   perror(s);
   ecit(1); 
}
void enableRawMode(){
    struct termios raw; //this declares a raw struct that stores terminal settings
    if(tcgetattr(STDIN_FILENO, &original_termios) == -1) die("tcgetattr");// gets the current attributes of the terminal settings
    atexit(disableRawMode);
     
    raw = original_termios;
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | ISTRIP | INPCK);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 10;
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
int main(){
    enableRawMode();
    char c; 
    while(1){
        char c = '\0';
        if(read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
        if(iscntrl(c)){
            printf("%d\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
        if(c == 'q'){
            break;
        }
    } 
    // quits when the 'q' character is read.
    // read function takes 3 args. an int type args which is stdin_fileno, stdout_fileno and error,
    //second argument is the void *buf which is the buffer to store data (address of the variable to store data)
    //third argument is the size_t that defines the number of bytes to read.

    return 0;
}