#include <vector>
#include <random>
using namespace std;

class Board {
    public:
        vector< vector<int> > cells;
        int n;
        int m;
        // TODO: How do we know when game is over? Idea: if there are two of the same next to each other anywhere

        bool gameOver;
        // random int util function (for placement of new tiles)
        int randi(int imin, int imax) {
            // mersenne twister random generator with uniform int distribution
            std::mt19937 generator((std::random_device())());
            std::uniform_int_distribution<int> distribution(imin, imax);
            return distribution(generator);
        }

        Board(int r, int c){
            // initialize with zeros everywhere and 2 random tiles
            cells = vector(r, vector(c, 0));
            this->n = r;
            this->m = c;
            addTile();
            addTile();
            gameOver = false;
            printBoard();
        }

        void printBoard(){
            for(vector<int> r : cells){
                for(int c : r){
                    printf("%d \t", c);
                }   
                printf("\n");
            }
        }

        void addTile() {
            // add a random 2 or 4 to an EMPTY square of the board
            // count the number of empty zeros
            unordered_map<int, pair<int, int>> zeroMap;
            int zeroCount = 0;
            for(int i = 0; i < n; i++){
                for(int j = 0; j < m; j++){
                    if(cells[i][j] == 0){
                        zeroMap[zeroCount] = make_pair(i, j);
                        zeroCount++;
                    }
                }
            }
            if(zeroCount == 0){ 
                // every square is filled. Do not add a tile 
                // but the user can maybe still make another move so game is not over
                return;
            }
            int placement = randi(0, zeroCount - 1);
            pair<int, int> rp = zeroMap[placement];
            // generate either a 2 or a 4 by making a 1 or a 2 and doubling it
            cells[rp.first][rp.second] = randi(1, 2) * 2;
            // printf("rando generated starting positions: %d, %d \n", rp.first, rp.second);
        }

        bool full(){
            unordered_map<int, pair<int, int>> zeroMap;
            int zeroCount = 0;
            for(int i = 0; i < n; i++){
                for(int j = 0; j < m; j++){
                    if(cells[i][j] == 0){
                        zeroMap[zeroCount] = make_pair(i, j);
                    }
                }
            }
            if(zeroCount == 0){ 
                // every square is filled. 
                return true;
            }
            return false;
        }

        // parse a move from character to function call
        void makeMove(char c){
            if(c == 'w' || c == 'W'){
                moveUp();
            }
            else if(c == 'a' || c == 'A'){
                moveLeft();
            }
            else if(c == 'd' || c == 'D'){
                moveRight();
            }
            else if(c == 's' || c == 'S'){
                moveDown();
            }
            else{
                // invalid character input
                printf("Invalid character detected! Please only enter w, a, s, or d \n");
                return ;
            }
            addTile();
            printBoard();
        }

        // handle n counterclockwise rotations of the board
        void CCW(int num){
            while(num-- > 0){
                // 1. switch rows for columns (transpose the matrix)
                vector<vector<int>> transpose = vector(m, vector(n, 0));
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        transpose[j][i] = cells[i][j];
                    }
                }
                // 2. reverse every column of the transpose
                reverse(transpose.begin(), transpose.end());
                cells = transpose;
            }
        }

        // every move function will just be a combination of 3 simple steps
        // 1. rotate the cells to align with an upward movement
        // 2. Call moveUp
        // 3. rotate back into place

        void moveLeft(){
            // rotate 270 degrees CCW
            CCW(3);
            printf("after rotation: \n");
            printBoard();
            moveUp();
            // revert to correct orientation
            printf("after combo: \n");
            printBoard();
            CCW(1);
            printf("after rotation back: \n");
            printBoard();
            printf("after random placement: \n");
        }
        void moveDown(){
            // rotate 180 degrees CCW
            CCW(2);
            moveUp();
            // revert to correct orientation
            CCW(2);
        }
        void moveRight(){
            // rotate 90 degrees CCW
            CCW(1);
            moveUp();
            // revert to correct orientation
            CCW(3);
        }
        void moveUp(){
            // scoot up anything that can be scooted up
            for(int i = 0; i < n ; i++){
                for(int j = 0; j < m; j++){
                    if(cells[i][j] != 0){
                        // do something
                        int tmp = i - 1;
                        // while the above is still a 0, shift up
                        while( tmp >= 0 && !cells[tmp][j] ){
                            cells[tmp][j] = cells[tmp+1][j];
                            cells[tmp+1][j] = 0;
                            tmp --;
                        }
                    }
                }
            }
            printf("after scoot: \n");
            printBoard();
            printf("\n");
            // combine things that can be combined, bottom-up

            // must keep track of which cells are results of combinations so we don't double-combine on same turn
            vector<vector<bool>> isCombo(n, vector<bool>(m, false));
            for(int i = n - 1; i >= 1; i--){
                for(int j = m - 1; j >= 0; j--){
                    if(cells[i][j] == 0){
                        continue;
                    }
                    else if(cells[i][j] == cells[i - 1][j] && !isCombo[i][j]){
                        printf("[%d, %d] = %d matches [%d, %d] \n", i, j,cells[i][j], i - 1, j);
                        // then we should combine these 2
                        cells[i - 1][j] *= 2;
                        cells[i][j] = 0;
                        isCombo[i-1][j] = true;
                    }
                }
            }
        }
};