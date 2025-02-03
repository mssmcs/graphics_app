#include "graphics.h"

using namespace std;
using namespace mssm;

int main()
{
    Graphics g("Testing", 1024, 768);

    while (g.draw()) {
        if (!g.isDrawable()) {
            continue;
        }

        g.line({100,100},{200,300}, GREEN);
        g.ellipse({400,400}, 30, 60, YELLOW);
    }

    return 0;
}


