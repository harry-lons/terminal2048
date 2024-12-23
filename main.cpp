#include "board.cpp"
#include "main.h"
#include <iostream>
#include <stdio.h>
#include <termios.h>
using namespace std;

int getch(void){
    // referenced from quora to retrieve char from terminal
    struct termios old, newl;
    tcgetattr(0, &old);
    newl = old;
    newl.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newl);
    int ch = getchar(); 
    tcsetattr(0, TCSANOW, &old);
    return ch; 
}

int main(){
    cout << "Enter the desired # of columns:";
    int col, row;
    cin >> col ;
    cout << "Enter the desired # of rows:";
    cin >> row; 
    // get the junk first character then the actual one
    int c = getch();
    Board board(col, row); 
    while(!board.gameOver){ 
        cout << "Enter next move (w / a / s / d) or q to quit" << endl;
        c = getch();
        try{
            if(c == 'q' || c == 'Q') return 0;  
            board.makeMove(c);
        } 
        catch(exception e){
            return 0;
        }
    }
    return 0;
}