#include <iostream>
#include <memory>

#include "cli.h"
#include "scanner.h"
#include "fs_adapter.h"
#include "fs.h"

using namespace std;

int main(int argc, char** argv) {
    try {
        Cli()
            .onHelp([](const string& helpMsg) {
                cout << helpMsg << endl;
            })
            .onParse([](const Opts& opts) {
                unique_ptr<IFS> pFS(new FS());
                const unique_ptr<IFSAdapter> pFsAdapter(new FSAdapter(pFS.get()));

                Scanner(pFsAdapter.get())
                    .onDuplicates([](const vector<filesystem::path>& duplicates) {
                        for (const string& line : duplicates) {
                            cout << line << endl;
                        }
                        cout << endl;
                    })
                    .scan(opts);
            })
            .parse(argc, argv);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
