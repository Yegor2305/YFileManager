#include "YPanel.h"
#include "YApplication.h"

int main(void)
{

    YApplication app;
    YPanel panel(120, 30, 1, 1, 1, 1, true);
    app.AddWidget(&panel);
    app.Run();
    //app.PrintColorPalete(30);

    return 1;
}
