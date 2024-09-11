#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
//COOKED MODE : The input is only sent to the program when the user presses "Enter" key. (also called canonical mode)
struct termios original_termios;
void disableRawMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}
void enableRawMode(){
    struct termios raw; //this declares a raw struct that stores terminal settings
    atexit(disableRawMode);
    tcgetattr(STDIN_FILENO, &original_termios); // gets the current attributes of the terminal settings
    raw = original_termios;
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
int main(){
    enableRawMode();
    char c; 
    while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
        if(iscntrl(c)){
            printf("%d\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
    } 
    // quits when the 'q' character is read.
    // read function takes 3 args. an int type args which is stdin_fileno, stdout_fileno and error,
    //second argument is the void *buf which is the buffer to store data (address of the variable to store data)
    //third argument is the size_t that defines the number of bytes to read.

    return 0;
}