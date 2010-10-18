/*
 * This is meant to be used in a separate `screen' than Irssi
 *
 * After ran, this infinitely loops checking the "fnotify" log
 * produced by the `fnotify.pl' script for Irssi notifications.
 * If `fnotify.pl' isn't present in Irssi's script directories,
 * this program will provide a suggestion for downloading via `wget'.
 *
 * By the way, this is my first _real_ attempt at any C/C++ programs...
 * I figure why not start learning a complex language by rewriting scripts
 * you already know and understand? So yeah, this may be overkill for this
 * (considering the bash script was like < 50 lines long) but I need the
 * practice =-D
 *
 * natediddy
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <vector>
#include "unistd.h"
#include "dirent.h"

#define IRSSI_CONFIG_DIR "/.irssi"

using namespace std;

struct Vars {
    string home;
    string irssipath;
    string fnOne;
    string fnTwo;
    string fnLog;
    string fnLogAll;
};

struct Vars pa, pb;

int existPath(const char* p)
{
    int ret = chdir(p);
    return (ret == 0 ? 0 : 1);
}

int existFile(string filename)
{   
    ifstream c;
     
    c.open(filename.c_str(), ifstream::in);
    c.close();
    
    return (c.fail() ? 1 : 0);
}

int isFnotify(void)
{
    struct Vars *i = &pa;

    i->home = getenv("HOME");
    i->irssipath = i->home + IRSSI_CONFIG_DIR;
    i->fnOne = i->irssipath + "/scripts/fnotify.pl";
    i->fnTwo = i->irssipath + "/scripts/autorun/fnotify.pl";

    if (existFile(i->fnOne) != 0) {
        if (existFile(i->fnTwo) != 0) {
            cout << "Fnotify script not found in Irssi scripts directory!" << endl;
            cout << "To obtain a copy, try something like:" << endl;
            cout << "\twget http://www.leemhuis.info/files/fnotify/fnotify -O "
                 << i->fnTwo << endl;
            return 1;
        }
    }
 
    return 0;
}

void showTimeAndDate(void)
{
    time_t t;
    time(&t);

    cout << ctime(&t) << endl;
}

void displayNotifications(string fn, int n)
{
    char* filename = (char*)fn.c_str();
    ifstream In(filename);
    vector<string> ln;
    string Out;

    while (getline(In, Out)) {
        ln.push_back(Out);
    }

	for (int i = 0; i < ln.size(); i++) {
		cout << n << ") => ! => " << ln[i] << endl;
	}
}

void refreshLog(string x, string y)
{
    char* xs = (char*)x.c_str();
    char* ys = (char*)y.c_str();
    ifstream In(xs);
    ofstream Out(ys);
    string c;

    while (getline(In, c)) {
        Out << c << "\n";
    }
}

int main(void)
{
    struct Vars *j = &pb;

    j->home = getenv("HOME");
    j->irssipath = j->home + IRSSI_CONFIG_DIR;
    j->fnLog = j->irssipath + "/fnotify";
    j->fnLogAll = j->irssipath + "/fnotify.log.all";

	if (0 != existPath((char*)j->irssipath.c_str())) {
		cout << "Irssi config directory not located" << endl;
		return 1;
	}

	if (0 != isFnotify()) {
        return 1;
	}

	int n = 0;

    while (true) {
        if (existFile(j->fnLog) == 0) {
            showTimeAndDate();
			displayNotifications(j->fnLog, n++);
            refreshLog(j->fnLog, j->fnLogAll);
            remove((char*)j->fnLog.c_str());
        }
        sleep(300);
    }

    return 0;
}
