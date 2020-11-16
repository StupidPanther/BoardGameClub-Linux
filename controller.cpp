#include "controller.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

Controller::Controller()
{
	cur_game = IDLE;
	cur_user = EMPTY;
}

Controller::~Controller()
{
}

int Controller::Exec()
{
	// get user choose (gobang/flipchess/quit)
	gametype_t user_choose;
	vector<string> options;
	options.clear();
	options.push_back("Gobang");
	options.push_back("Flip chess");
	options.push_back("Quit Club");
	unsigned int choose=PrintDialog("Choose game or quit club", options);
	if (choose == 0)
		user_choose = GOBANG;
	else if (choose == 1) 
		user_choose = FLIPCHESS;
	else
		user_choose = IDLE;
	while (user_choose != IDLE)
	{
		cur_game = user_choose;
		bool whether_to_goon = true;
		while (whether_to_goon == true)
		{
			// initiate a blank board
			board.Init();
			if (cur_game == FLIPCHESS)
			{
				flipchess.Init(board.SetPlainBoard());
			}
			cur_user = BLACK;
			// get archives
			string fromarchive;
			vector<string> archives = GetArchives();
			if (!archives.empty())
			{
				options.clear();
				options.push_back("YES");
				options.push_back("NO ");
				unsigned int ch = PrintDialog("Continue to play with archive ?", options);
				if (ch == 0)
				{
					options.clear();
					for (auto i = archives.begin(); i < archives.end(); i++)
					{
						options.push_back(*i);
					}
					unsigned int num = PrintDialog("Choose a archive you want to continue", options);
					fromarchive = archives[num];
					Load(archives[num]);
				}
			}
			// one round
			char keyevent = 'n';
			while (keyevent != 'p')
			{
				// move the cursor
				while (keyevent != 'e' && keyevent != '0' && keyevent != 'E' && keyevent != 'p' && keyevent != 'P')
				{

					if (keyevent == 'w' || keyevent == 'W')
					{
						board.MoveCursor(UP);
					}
					else if (keyevent == 's' || keyevent == 'S')
					{
						board.MoveCursor(DOWN);
					}
					else if (keyevent == 'a' || keyevent == 'A')
					{
						board.MoveCursor(LEFT);
					}
					else if (keyevent == 'd' || keyevent == 'D')
					{
						board.MoveCursor(RIGHT);
					}
					if (cur_game == GOBANG)
						PrintBoard("[p]quit  [e/0]move");
					if (cur_game == FLIPCHESS)
						PrintBoard(flipchess.CurStatus(board.GetPlainBoard()) + "  [p]quit [e/0]move");
					// is a win board?
					if (cur_game == GOBANG)
					{
						if (gobang.IfStop(board.GetPlainBoard()) == true)
						{
							//DeleteArchive(fromarchive);
							string who = "Nobody";
							if (gobang.WhoWin(board.GetPlainBoard()) == BLACK)
								who = "BLACK ●";
							else if (gobang.WhoWin(board.GetPlainBoard()) == WHITE)
								who = "WHITE ○";
							PrintBoard(who + " win! Press any key to continue...");
							mygetch();
							break;
						}
					}
					else if (cur_game == FLIPCHESS)
					{
						if (flipchess.IfStop(board.GetPlainBoard()) == true)
						{
							//DeleteArchive(fromarchive);
							string who = "Nobody";
							if (flipchess.WhoWin(board.GetPlainBoard()) == BLACK)
								who = "BLACK ●";
							else if (flipchess.WhoWin(board.GetPlainBoard()) == WHITE)
								who = "WHITE ○";
							PrintBoard(who + " win! Press any key to continue...");
							mygetch();
							break;
						}
					}
					keyevent = mygetch();
				}
				// move
				if (keyevent == 'e' || keyevent == 'E' || keyevent == '0')
				{
					movestatus_t move_valid;
					if (cur_game == GOBANG)
					{
						move_valid = gobang.IfMoveValid(board.GetPlainBoard(), cur_user, board.GetCursorPosition());
					}
					else if (cur_game == FLIPCHESS)
					{
						move_valid = flipchess.IfMoveValid(board.GetPlainBoard(), cur_user, board.GetCursorPosition());
					}
					if (move_valid == SUCCESS)
					{
						board.Move(cur_user);
						cur_user++;
						if (cur_game == FLIPCHESS)
						{
							if (flipchess.CouldMove(board.GetPlainBoard(), cur_user) == false)
								cur_user++;
							flipchess.Flip(board.SetPlainBoard(), board.GetCursorPosition());
						}
						if (board.GetCursorPosition().column == COLUMN_COUNT - 1)
							board.SetCursorPosition()--;
						else
							board.SetCursorPosition()++;
						if (cur_game == GOBANG)
							PrintBoard("[p]quit [e/0]move [u]undo");
						if (cur_game == FLIPCHESS)
							PrintBoard(flipchess.CurStatus(board.GetPlainBoard()) + "  [p]quit [e/0]move [u]undo");
						// one win
						if (cur_game == GOBANG)
						{
							if (gobang.IfStop(board.GetPlainBoard()) == true)
							{
								//DeleteArchive(fromarchive);
								string who = "Nobody";
								if (gobang.WhoWin(board.GetPlainBoard()) == BLACK)
									who = "BLACK ●";
								else if (gobang.WhoWin(board.GetPlainBoard()) == WHITE)
									who = "WHITE ○";
								PrintBoard(who + " win! Press any key to continue...");
								mygetch();
								break;
							}
						}
						else if (cur_game == FLIPCHESS)
						{
							if (flipchess.IfStop(board.GetPlainBoard()) == true)
							{
								//DeleteArchive(fromarchive);
								string who = "Nobody";
								if (flipchess.WhoWin(board.GetPlainBoard()) == BLACK)
									who = "BLACK ●";
								else if (flipchess.WhoWin(board.GetPlainBoard()) == WHITE)
									who = "WHITE ○";
								PrintBoard(who + " win! Press any key to continue...");
								mygetch();
								break;
							}
						}

					}
					else
					{
						if (cur_game == GOBANG)
							PrintBoard("Invalid move!  [p]quit [e/0]move");
						if (cur_game == FLIPCHESS)
							PrintBoard(flipchess.CurStatus(board.GetPlainBoard()) + "  Invalid move!  [p]quit [e/0]move");
					}
					keyevent = mygetch();
					// undo
					if (move_valid == SUCCESS)
						if (keyevent == 'u' || keyevent == 'U')
						{
							board.Regret();
							cur_user++;
							if (cur_game == GOBANG)
								PrintBoard("[p]quit [e/0]move");
							if (cur_game == FLIPCHESS)
								PrintBoard(flipchess.CurStatus(board.GetPlainBoard()) + "  [p]quit [e/0]move");
							keyevent = mygetch();
						}
				}
				// is a win board?
				if (cur_game == GOBANG)
				{
					if (gobang.IfStop(board.GetPlainBoard()) == true)
					{
						break;
					}
				}
				else if (cur_game == FLIPCHESS)
				{
					if (flipchess.IfStop(board.GetPlainBoard()) == true)
					{
						break;
					}
				}
				while (keyevent == 'p')
				{
					options.clear();
					options.push_back("YES");
					options.push_back("NO ");
					if (PrintDialog("Are you sure to quit ?", options) == 0)
						break;
					if (cur_game == GOBANG)
						PrintBoard("[p]quit [e/0]move");
					if (cur_game == FLIPCHESS)
						PrintBoard(flipchess.CurStatus(board.GetPlainBoard()) + "  [p]quit [e/0]move");
					keyevent = mygetch();
				}
			}
			//if (keyevent == 'p')
			{
				//ask whether to save board
				options.clear();
				options.push_back("YES");
				options.push_back("NO ");
				unsigned int save = PrintDialog("Save your progress ?", options);
				if (save == 0)
				{
					string archievename = PrintEdit("Enter archieve name (no space) :");
					Store(archievename);
				}
			}
			// get whether to go on this type
			options.clear();
			options.push_back("YES");
			options.push_back("NO ");
			unsigned int goon = PrintDialog("Start a new round ?", options);
			if (goon != 0)
			{
				break;
			}
		}
		// get user choose (gobang/flipchess/quit)
		options.clear();
		options.push_back("Gobang");
		options.push_back("Flip chess");
		options.push_back("Quit Club");
		unsigned int choose = PrintDialog("Choose game or quit club", options);
		if (choose == 0)
			user_choose = GOBANG;
		else if (choose == 1)
			user_choose = FLIPCHESS;
		else
			user_choose = IDLE;
	}
	// say goodbye
	//cout << "Bye bye!" << endl;
	PrintBye();
	sleep(6);
	system("clear");
	cout << "\033[?25h\033[0m" << endl;
	system("clear");
	return 0;
}

void Controller::Store(string archivename)
{
	if (access("./archive/", 0) == -1)
		mkdir("./archive/", 0755);
	string dir;
	if (cur_game == GOBANG)
		dir = "./archive/gobang/";
	else if (cur_game == FLIPCHESS)
		dir = "./archive/flipchess/";
	if (access(dir.c_str(), 0) == -1)
		mkdir(dir.c_str(), 0755);
	ofstream fout(dir + archivename + ".arcv", ios::binary);
	if (!fout)
	{
		cout << "Fail to store " + archivename << endl;
		return;
	}
	const plainboard_t &plainboard = board.GetPlainBoard();
	const position_t pos = board.GetCursorPosition();
	const user_t usr = cur_user;
	fout.write(reinterpret_cast<const char *>(plainboard), sizeof(plainboard));
	fout.write(reinterpret_cast<const char *>(&pos), sizeof(pos));
	fout.write(reinterpret_cast<const char *>(&usr), sizeof(usr));
	fout.close();
}

void Controller::Load(string archivename)
{
	string path;
	if (cur_game == GOBANG)
		path = "./archive/gobang/" + archivename;
	else if (cur_game == FLIPCHESS)
		path = "./archive/flipchess/" + archivename;
	ifstream fin(path, ios::binary);
	if (!fin)
	{
		cout << "Fail to load " + archivename << endl;
		return;
	}
	fin.read(reinterpret_cast<char *>(board.SetPlainBoard()), sizeof(board.GetPlainBoard()));
	fin.read(reinterpret_cast<char *>(&board.SetCursorPosition()), sizeof(board.GetCursorPosition()));
	fin.read(reinterpret_cast<char *>(&cur_user), sizeof(cur_user));
	fin.close();
}

vector<string> Controller::GetArchives()
{
	class file_name_time
	{
	public:
		string name;
		time_t time;
	};

	string path;
	if (cur_game == GOBANG)
		path = "./archive/gobang/";
	else if (cur_game == FLIPCHESS)
		path = "./archive/flipchess/";
	struct dirent *entry = NULL;
	DIR *dir = opendir(path.c_str());
	if (dir == NULL)
		return vector<string>();
	vector<file_name_time> fnts;
	vector<string> ret;

	entry = readdir(dir);
	while (entry != NULL)
	{
		file_name_time item;
		struct stat s;
		item.name = entry->d_name;
		stat((path + item.name).c_str(), &s);
		item.time = s.st_mtim.tv_sec;
		if (item.name != "." && item.name != "..")
			fnts.push_back(item);
		entry = readdir(dir);
	}

	sort(fnts.begin(), fnts.end(), [](file_name_time a, file_name_time b) { return a.time > b.time; });
	for (auto i = fnts.begin(); i < fnts.end(); i++)
	{
		ret.push_back(i->name);
	}

	return ret;
}

void Controller::DeleteArchive(string archivename)
{
	string path;
	if (cur_game == GOBANG)
		path = "./archive/gobang/" + archivename;
	else if (cur_game == FLIPCHESS)
		path = "./archive/flipchess/" + archivename;
	remove(path.c_str());
}

void Controller::PrintBoard(string title)
{
	string whitepiece = "○━";
	string blackpiece = "●━";
	string whitepiecer = "○";
	string blackpiecer = "●";
	string block = "��";
	string block_b = "■━";
	string block_w = "□━";
	string block_br = "■";
	string block_wr = "□";
	string block_d = "��";
	string block_xl = "╳━";
	string block_xr = "╳";
	string line[ROW_COUNT][COLUMN_COUNT];
	string bar = "          \033[30;107m                      Board Game Club                      \033[0m";
	string tail = "          \033[97m___________________________________________________________\033[?25l\033[0m";
	line[0][0] = "┏━";
	for (unsigned int j = 1; j <= COLUMN_COUNT - 2; j++)
	{
		line[0][j] = "┳━";
	}
	line[0][COLUMN_COUNT - 1] = "┓";
	for (unsigned int i = 1; i <= ROW_COUNT - 2; i++)
	{
		line[i][0] = "┣━";
		for (unsigned int j = 1; j <= COLUMN_COUNT - 2; j++)
		{
			line[i][j] = "╋━";
		}
		line[i][COLUMN_COUNT - 1] = "┫";
	}
	line[ROW_COUNT - 1][0] = "┗━";
	for (unsigned int j = 1; j <= COLUMN_COUNT - 2; j++)
	{
		line[ROW_COUNT - 1][j] = "┻━";
	}
	line[ROW_COUNT - 1][COLUMN_COUNT - 1] = "┛";
	if (CMD_WIDTH - 21 <= title.length())
	{
		title = title.substr(0, CMD_WIDTH - 22);
	}

	system("clear");

	for (int i = 0; i < 3; i++)
		cout << endl;
	cout << bar << endl;
	for (int i = 0; i < (CMD_HEIGHT / 2 - 9) / 2; i++)
		cout << endl;
	string cusr;
	stringstream ss;
	ss << cur_user;
	ss >> cusr;
	cusr = "Now moving : " + cusr;
	for (unsigned int i = 0; i < 10 + (CMD_WIDTH - 21 - cusr.length() + 1) / 2; i++)
		cout << " ";
	cout << "\033[97m" + cusr + "\033[0m" << endl;
	for (int i = 0; i < (CMD_HEIGHT / 2 - 9) / 2; i++)
		cout << endl;
	for (unsigned int i = 0; i < ROW_COUNT; i++)
	{
		for (int k = 0; k < 10 + (CMD_WIDTH - 21 - 15) / 2 - 1; k++)
			cout << " ";
		for (unsigned int j = 0; j < COLUMN_COUNT; j++)
		{
			if (i == board.GetCursorPosition().row && j == board.GetCursorPosition().column)
			{
				if (board.GetPlainBoard()[i][j] == EMPTY)
				{
					if (j == COLUMN_COUNT - 1)
						cout << block_xr;
					else
						cout << block_xl;
				}
				else if (board.GetPlainBoard()[i][j] == BLACK)
					if (j == COLUMN_COUNT - 1)
						cout << block_br;
					else
						cout << block_b;
				else if (board.GetPlainBoard()[i][j] == WHITE)
					if (j == COLUMN_COUNT - 1)
						cout << block_wr;
					else
						cout << block_w;
			}
			else if (board.GetPlainBoard()[i][j] == EMPTY)
				cout << line[i][j];
			else if (board.GetPlainBoard()[i][j] == BLACK)
				if (j == COLUMN_COUNT - 1)
					cout << blackpiecer;
				else
					cout << blackpiece;
			else if (board.GetPlainBoard()[i][j] == WHITE)
				if (j == COLUMN_COUNT - 1)
					cout << whitepiecer;
				else
					cout << whitepiece;
		}
		cout << endl;
	}
	for (int i = 0; i < (CMD_HEIGHT / 2 - 9) / 2; i++)
		cout << endl;
	for (unsigned int i = 0; i < 10 + (CMD_WIDTH - 21 - title.length() + 1) / 2; i++)
		cout << " ";
	cout << "\033[97m" + title + "\033[0m" << endl;
	for (int i = 0; i < (CMD_HEIGHT / 2 - 9) / 2; i++)
		cout << endl;
	cout << tail << endl;
}

unsigned int Controller::PrintDialog(string title, vector<string> options)
{
	string bar = "          \033[30;107m                      Board Game Club                      \033[0m";
	string tail = "          \033[97m___________________________________________________________\033[?25l\033[0m";
	unsigned int picked = 0;
	unsigned int buttonwidth = 25;

	if (CMD_WIDTH - 21 <= title.length())
	{
		title = title.substr(0, CMD_WIDTH - 22);
	}

	for (auto i = options.begin(); i < options.end(); i++)
	{
		if (i->length() > buttonwidth)
			*i = i->substr(0, buttonwidth);
	}

	char input = 'n';
	while (input != 'y' && input != 13)
	{
		if (input == 'w' && picked > 0)
			picked--;
		else if (input == 'w' && picked == 0)
			picked = options.size() - 1;
		else if (input == 's' && picked < options.size() - 1)
			picked++;
		else if (input == 's' && picked == options.size() - 1)
			picked = 0;

		system("clear");

		for (unsigned int i = 0; i < (CMD_HEIGHT - (6 + options.size())) / 2; i++)
			cout << endl;
		cout << bar << endl
			 << endl;
		for (unsigned int i = 0; i < 10 + (CMD_WIDTH - 21 - title.length() + 1) / 2; i++)
			cout << " ";
		cout << "\033[97m" + title + "\033[0m" << endl
			 << endl;
		for (unsigned int index = 0; index < options.size(); index++)
		{
			if (index == picked)
			{
				for (unsigned int i = 0; i < 10 + (CMD_WIDTH - 21 - buttonwidth) / 2; i++)
					cout << " ";
				for (unsigned int i = 0; i < (buttonwidth - options[index].length()) / 2; i++)
					cout << "\033[30;47m \033[0m";
				cout << "\033[30;47m" + options[index] + "\033[0m";
				for (unsigned int i = 0; i < buttonwidth - options[index].length() - (buttonwidth - options[index].length()) / 2; i++)
					cout << "\033[30;47m \033[0m";
			}
			else
			{
				for (unsigned int i = 0; i < 10 + (CMD_WIDTH - 21 - buttonwidth) / 2; i++)
					cout << " ";
				for (unsigned int i = 0; i < (buttonwidth - options[index].length()) / 2; i++)
					cout << " ";
				cout << options[index];
				for (unsigned int i = 0; i < buttonwidth - options[index].length() - (buttonwidth - options[index].length()) / 2; i++)
					cout << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << tail << endl;
		for (unsigned int i = 0; i < (CMD_HEIGHT - (6 + options.size())) / 2 - 1; i++)
			cout << endl;

		input = mygetch();
	}

	return picked;
}

string Controller::PrintEdit(string title)
{
	string bar = "          \033[30;107m                      Board Game Club                      \033[0m";
	string tail = "          \033[97m___________________________________________________________\033[0m";
	string line = "                         \033[37m-----------------------------\033[0m";

	system("clear");

	for (unsigned int i = 0; i < (CMD_HEIGHT - 8) / 2; i++)
		cout << endl;
	cout << bar << endl
		 << endl;
	for (unsigned int i = 0; i < 10 + (CMD_WIDTH - 21 - title.length() + 1) / 2; i++)
		cout << " ";
	cout << "\033[97m" + title + "\033[0m" << endl
		 << endl
		 << endl;
	cout << "                           \033[s\033[0m" << endl;
	cout << line << endl;
	cout << tail << endl;
	cout << "\033[u\033[?25h\033[0m";
	string ret;
	cin >> ret;

	return ret;
}

void Controller::PrintBye()
{
	string bye = "                      ____                __               __\n"
				 "                     / __ )__  _____     / /_  __  _____  / /\n"
				 "                    / __  / / / / _ \\   / __ \\/ / / / _ \\/ / \n"
				 "                   / /_/ / /_/ /  __/  / /_/ / /_/ /  __/_/  \n"
				 "                  /_____/\\__, /\\___/  /_.___/\\__, /\\___(_)   \n"
				 "                        /____/              /____/           \n";
	system("clear");
	for (int i = 0; i < 8; i++)
		cout << endl;
	cout << bye << endl
		 << endl;
	cout << "                           Board Game Club (c) 2020" << endl;
	for (int i = 0; i < 7; i++)
		cout << endl;
	cout << "\033[?25l\033[0m";
}

char Controller::mygetch()
{
	char ch = _getch();
	if (ch != 27)
		return ch;
	else
	{
		ch = _getch();
		ch = _getch();
		if (ch == 65)
			return 'w';
		else if (ch == 66)
			return 's';
		else if (ch == 68)
			return 'a';
		else if (ch == 67)
			return 'd';
		else
			return 0;
	}
}
