#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

#include "Job.h"
#include "Command.h"
#include "Misc.h"

using namespace std;

int main( int argc, char* argv[])
{
	string in;
	bool flag = 0;
	Job job;
	vector<Job> jobDump;
	vector<Job> runningJobs;
	vector<Job>::size_type i;
	const char* user_cstr = getenv( "USER");
	string prompt = (user_cstr) ? user_cstr : ""; prompt += "$ "; 

	//*
	while( !flag)
	{
		printf("%s", prompt.c_str());
		getline( cin, in);
		in = trim( in);

		// wait for jobs in runningJobs //
		for( i=0; i<runningJobs.size(); i++) 
		{
			if( runningJobs[i].bgWait())
			{
				printf( "[%d]+ Done %s\n", runningJobs[i].id, runningJobs[i].toString().c_str());
				jobDump.push_back( runningJobs[i]);
				runningJobs.erase( runningJobs.begin()+i);
				i--;
			}
		}

		if( in == "exit") 
			flag = 1;
		else
		{
			job = in;

			if( job.getNCommands())
			{
				job.execute();

				if( job.inbg())
				{
					job.id = runningJobs.size();
					printf( "[%d]\n", job.id);
					runningJobs.push_back( job);
				}
				else
				{
					job.forceWait();
					jobDump.push_back( job);
				}
			}
		}
	}	
	//*/

	/*
	for( i=0; i<runningJobs.size(); i++);
	{
		runningJobs.forceWait()
	}
	//*/
	
	for( i=0; i<jobDump.size(); i++)
		printf("%s\n", jobDump[i].toString().c_str());


	return 0;

}



