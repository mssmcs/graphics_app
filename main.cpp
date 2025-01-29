
#include "graphics.h"

using namespace std;
using namespace mssm;
using namespace fmt;

int main()
{
    Graphics g("Testing", 1024, 768);

    while (g.draw()) {
        if (!g.isDrawable()) {
            continue;
        }
        g.line({300,300},{200,100}, GREEN);
        g.text({100,100}, 20, "hello");
    }

    return 0;
}
