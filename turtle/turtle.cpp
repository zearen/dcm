#include <ncurses.h>
#include <unistd.h>

#include "../Interpretter.h"
#include "../plugins/prelude.h"

struct Point {
    int x;
    int y;

    Point(int iX=0, int iY=0) {
        x = iX;
        y = iY;
    }
};

enum Direction {NORTH, SOUTH, EAST, WEST};

Point inc(Point p, Direction dir) {
    switch (dir) {
    case NORTH:
        return Point(p.x, p.y - 1);
    case SOUTH:
        return Point(p.x, p.y + 1);
    case EAST:
        return Point(p.x + 1, p.y);
    case WEST:
        return Point(p.x - 1, p.y);
    }
    return Point();
}

Point turtle();

Direction facing;



int main() {
    Interpretter interpretter;
    //interpretter.addPlugin(*turtlePlugin());
    interpretter.addPlugin(*preludePlugin());
    int ch;
    initscr();


    endwin();
}
