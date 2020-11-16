#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "ref.h"
#include "board.h"
#include "gobang.h"
#include "flipchess.h"
#include <string>
#include <vector>
using namespace std;

class Controller
{
private:
    gametype_t cur_game;
    user_t cur_user;
    Board board;
    Gobang gobang;
    Flipchess flipchess;
    
public:
    Controller();
    ~Controller();
    int Exec();

private:
    void Store(string archivename);
    void Load(string archivename);
    vector<string> GetArchives();
    void DeleteArchive(string archivename);
    void PrintBoard(string title="");
    unsigned int PrintDialog(string title, vector<string> options);
    string PrintEdit(string title);
    void PrintBye();
    char mygetch();
};

#endif