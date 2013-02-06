#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <unistd.h>

#include "CFuncs.h"
#include "Misc.h"

using namespace std;

class Command
{
private:
	string err;
	pid_t pid;
	vector<string> args;// args[0]: command
	string readFile;
	string writeFile;
	bool running; // 1: forked and not waited for. 0: otherwise
	struct rusage processInfo; // valid iff !running && pid

	void reset();

public:
	Command();
	Command( const string&);

	pid_t execute( int, int);
	void set( const string&);
	Command& operator =( const string&);
	bool bgWait(); // background wait
	bool fgWait(); // foreground wait

	string readFilename() const;
	string writeFilename() const;
	const string& getError() const;
	string toString() const;
	operator pid_t() const;
	bool isRunning() const;

};

inline bool Command::isRunning() const { return running; }

#endif //COMMAND_H

