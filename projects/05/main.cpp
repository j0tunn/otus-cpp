#include <iostream>
#include <exception>
#include "controller.h"

using namespace std;

int main(int, char **) {
    Controller app;

    try {
        app.NewDocument();

        app.AddSquare(2);
        app.AddCircle(3);
        app.AddSquare(4);

        app.DeleteLastObject();

        app.ExportDocument("foo/bar.baz");

        app.ImportDocument("foo/bar.baz");
    } catch(exception& e) {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}
