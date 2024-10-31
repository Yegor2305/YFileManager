#include "Panel.h"
#include "YApplication.h"

int main(void)
{

    YApplication app;
    Panel panel(120, 30);
    app.AddWidget(&panel);
    app.Run();
    //app.PrintColorPalete(10);

    return 1;
}
