#ifndef JOB_H
#define JOB_H

#include <vector>
#include <sys/wait.h>
#include "Command.h"
#include "CFuncs.h"
#include "Misc.h"

using namespace std;

class Job
{
private:
	typedef vector<Command>::size_type size_type;

	vector<Command> commands;
	bool inBackground;

public:
	int id;

	Job();
	Job( const string&);

	void execute();
	Job& operator =( const string&);
	bool bgWait();
	bool forceWait();

	string toString() const;
	int getNCommands() const;
	bool inbg() const;
	double getTime( TimeType) const;
	


};

inline int Job::getNCommands() const { return (int)commands.size();}
inline bool Job::inbg() const { return inBackground;}

#endif //JOB_H









