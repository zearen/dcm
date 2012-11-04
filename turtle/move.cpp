#include <ncurses.h>

struct Point {
    int x;
    int y;

    Point(int iX=0, int iY=0) {
        x = iX;
        y = iY;
    }
};

Point inc(Point p, char dir) {
    switch (dir) {
    case 'w':
        return Point(p.x, p.y - 1);
    case 's':
        return Point(p.x, p.y + 1);
    case 'a':
        return Point(p.x - 1, p.y);
    case 'd':
        return Point(p.x + 1, p.y);
    }
    return Point();
}

Point loc;

int main() {
    int ch = 0;
    initscr();
    noecho();
    while (ch != 'q') {
        addch('o');
        move(loc.y, loc.x);
        ch = getch();
        addch(' ');
        loc = inc(loc, ch);
        move(loc.y, loc.x);
    }
    endwin();
}
