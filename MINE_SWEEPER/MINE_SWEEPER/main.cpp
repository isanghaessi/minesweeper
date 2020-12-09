#include <SFML/Graphics.hpp>
#include <vector>

#define WIDTH 16
#define HEIGHT 16
#define BOMBS 4

#define BLACK 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define BOMB 9
#define WHITE 10
#define FLAG 11

using namespace sf;
using namespace std;

struct pos{
    int row;
    int column;
};

int imagePixel=32;
int board[WIDTH+2][HEIGHT+2];
int boardUser[WIDTH+2][HEIGHT+2];

int isEnd=0;

void Init_bomb(){
    static int bombs=0;
    if(bombs<BOMBS){
        int x = rand() % (WIDTH+1);
        int y = rand() % (HEIGHT+1);
        if(board[x][y]!=BOMB && x>0 && y>0 ){
            board[x][y]=BOMB;
            bombs++;
        }
        Init_bomb();
    }
}

void Init_board(){
    Init_bomb();
    for (int i = 1; i < WIDTH+1; i++){
        for (int j = 1; j < HEIGHT+1; j++){
            if (board[i][j] != BOMB){
                int count = 0;
                for (int p=i-1; p<=i+1; p++) {
                    for (int q=j-1; q<=j+1; q++) {
                        if(board[p][q]==BOMB){
                            count++;
                        }
                    }
                }
                board[i][j] = count;
            }
        }
    }
}

void Init_boardUser(){
    for(int i = 1; i < WIDTH+1; i++){
        for (int j = 1; j < HEIGHT+1; j++){
            boardUser[i][j] = WHITE;
        }
    }
}


void End_game(){
    for (int i=1; i<WIDTH+1; i++) {
        for (int j=1; j<WIDTH+1; j++) {
            boardUser[i][j]=board[i][j];
        }
    }
    isEnd++;
}

void Check(int _x, int _y){
    vector<pair<int, int>> toCheck;
    for (int i=-1; i<=1; i+=2) {
        if(_x+i>0 && _x+i<WIDTH+1 && _y>0 && _y<=HEIGHT+1){
            if(boardUser[_x+i][_y]==WHITE){
                boardUser[_x+i][_y]=board[_x+i][_y];
                if(boardUser[_x+i][_y]==BLACK){
                    toCheck.push_back(make_pair(_x+i,_y));
                }
            }
        }
        if(_x>0 && _x<WIDTH+1 && _y+i>0 && _y+i<=HEIGHT+1){
            if(boardUser[_x][_y+i]==WHITE){
                boardUser[_x][_y+i]=board[_x][_y+i];
                if(boardUser[_x][_y+i]==BLACK){
                    toCheck.push_back(make_pair(_x,_y+i));
                }
            }
        }
    }
    int length=toCheck.size();
    for (int i=0; i<length; i++) {
        Check(toCheck[i].first,toCheck[i].second);
    }
}

void Click_left(int _x, int _y){
    if(boardUser[_x][_y]==WHITE){
        if(board[_x][_y]==BOMB){
            End_game();
        }else{
            if(board[_x][_y]==BLACK){
                boardUser[_x][_y]=board[_x][_y];
                Check(_x,_y);
            }else{
                boardUser[_x][_y]=board[_x][_y];
            }
        }
    }
}

void Click_right(int _x, int _y){
    if(boardUser[_x][_y]==WHITE){
        boardUser[_x][_y]=FLAG;
    }else{
        boardUser[_x][_y]=WHITE;
    }
}

int main(){
    RenderWindow app(VideoMode((WIDTH+2) * imagePixel, (HEIGHT+2) * imagePixel), "MINE_SWEEPER");

    Texture texture;
    texture.loadFromFile("/Users/hongseungyong/Dropbox/Xcode/MINE_SWEEPER/images/tiles.jpg");
    Texture chicken;
    chicken.loadFromFile("/Users/hongseungyong/Dropbox/Xcode/MINE_SWEEPER/images/win.jpg");

    Sprite s(texture);
    
    Init_board();
    Init_boardUser();

    while (app.isOpen()){
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / imagePixel;
        int y = pos.y / imagePixel;
        Event event;
        while (app.pollEvent(event)){
            if (event.type == Event::Closed){
                app.close();
            }
            if (event.type == Event::MouseButtonPressed){
                if (event.key.code == Mouse::Left){
                    Click_left(x,y);
                }
                else if (event.key.code == Mouse::Right){
                    Click_right(x,y);
                }
            }
        }
        int count=WIDTH*HEIGHT;
        for (int i=1; i<WIDTH+1; i++) {
            for (int j=1; j<HEIGHT+1; j++) {
                if(boardUser[i][j]!=WHITE && boardUser[i][j]!=FLAG && board[i][j]!=BOMB){
                    count--;
                }
            }
        }
        app.clear(Color::White);
        if (count <= BOMBS && isEnd==0){
            Sprite win(chicken);
            win.setTexture(chicken);
            app.draw(win);
        }else{
            for (int i = 1; i < WIDTH+1; i++){
                for (int j = 1; j < HEIGHT+1; j++){
                    s.setTextureRect(IntRect(boardUser[i][j] * imagePixel, 0, imagePixel, imagePixel));
                    s.setPosition(i * imagePixel, j * imagePixel);
                    app.draw(s);
                }
            }
        }
        app.display();
    }
}
