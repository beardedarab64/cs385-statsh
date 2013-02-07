#include "Job.h"

/***************************************************************************************
* AWESOME CONSTRUCTORS
***************************************************************************************/
Job::Job():
	commands(),
	inBackground(0),
	id(0)
{
}

Job::Job( const string& line):
	commands(),
	inBackground(0),
	id(0)
{
	operator =( line);
}

/***************************************************************************************
* Executes each command setting up the input/output of each according to piping/redire
* ction requirements
***************************************************************************************/
void Job::execute( )
{
	size_type i;
	int piperead;
	int input, output;
	int pipes[2];
	size_type ncommands = commands.size();
	pipes[0] = pipes[1] = 0;
	piperead = 0;
	FILE* f;
	
	// EXECUTE COMMANDS IN ORDER
	for( i=0; i<ncommands; i++)
	{
		cPipe( pipes);
		Command& cmnd = commands[ i];
		
		// DEFINE INPUT
		if( cmnd.readFilename().size())
		{
			// open file and set input
			f = fopen( cmnd.readFilename().c_str(), "r");
			if( !f)
			{
				perror( "fopen() failed");
				input = -1;
			}
			else
				input = fileno( f);
		}
		else if( i==0)
			input = STDIN_FILENO;
		else
			input = piperead;

		// DEFINE OUTPUT
		if( cmnd.writeFilename().size())
		{
			f = fopen( cmnd.writeFilename().c_str(), "w");
			if( !f)
			{
				perror( "fopen() failed");
				output = -1;
			}
			else
				output = fileno( f);
		}
		else if( i == ncommands-1)
			output = STDOUT_FILENO;
		else
			output = pipes[1];

		// EXECUTE IT
		if( input>=0 && output>=0)
			cmnd.execute( input, output);

		// CLOSE INPUT, OUTPUT, WRITE END, AND PREP NEXT ITERATION
		if( input>=0 && input != STDIN_FILENO) cClose( input);
		if( output>=0 && output != STDOUT_FILENO) cClose( output);

		if( piperead && piperead != input) cClose( piperead);
		if( pipes[1] != output) cClose( pipes[1]);
		
		piperead = pipes[0];

	}
	
	if( pipes[0]>0) cClose( pipes[0]);

}

/***************************************************************************************
* Returns:	1: all commands are reaped or otherwise not running
*			0: there are commands that are still running
***************************************************************************************/
bool Job::bgWait()
{
	size_type ncommands = commands.size();
	size_type i;
	bool ret = 1;

	for( i=0; i<ncommands; i++) if( commands[i].isRunning())
	{
		if( !commands[i].bgWait()) ret = 0;
	}

	return ret;
}

/***************************************************************************************
* Blocks to wait for each process to finish 
***************************************************************************************/
bool Job::forceWait()
{
	size_type ncommands = commands.size();
	for( size_type i=0; i<ncommands; i++) if( commands[i].isRunning())
		commands[i].fgWait();
	return 1;
}


/***************************************************************************************
* parses <pstr> to populate members <commands> and <inBackground>
***************************************************************************************/
Job& Job::operator =( const string& pstr)
{
	commands.clear();
	inBackground = 0;
	string str = pstr;

	str = trim( str);
	if( str.size() && str[str.size()-1]=='&')
	{
		inBackground = 1;
		str = trim( str.substr(0, str.size()-1));
	}

	vector<string> strCommands = tokenize( str, '|');
	unsigned int ncmds = strCommands.size();

	for( unsigned int i=0; i<ncmds; i++)
	{
		strCommands[i] = trim( strCommands[i]);

		if( strCommands[i].size())
			commands.push_back( strCommands[ i]);
	}
	
	return *this;
}

/***************************************************************************************
* Resturns a readable string representation of the object
***************************************************************************************/
string Job::toString() const
{
	string ret;
	size_type ncommands = commands.size();

	for(size_type i=0; i<ncommands; i++)
	{
		ret += commands[i].toString();
		if( i<ncommands-1) ret += " | ";
	}

	return ret;
}

/***************************************************************************************
* Returns the total time taken by its processes
***************************************************************************************/
double Job::getTime( TimeType type) const
{
	double ret = 0;
	for( size_type i=0; i<commands.size(); i++) ret += commands[i].getTime( type);
	return ret;
}
















