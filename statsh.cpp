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

void printStats( const vector<Job>&, const vector<Job>&);


/***************************************************************************************
* ENTRY POINT
***************************************************************************************/
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

	printf( "Author: Santiago Herrera\n");
	printf( "ID/email: sherre9@uic.edu\n");
	printf( "Class: CS 385, University of Illinois at Chicago\n\n");
 

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
		else if( in == "stats")
			printStats( jobDump, runningJobs);
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

	printStats( jobDump, runningJobs);
	return 0;

}

/***************************************************************************************
* Prints time statistics out to the screen
***************************************************************************************/
void printTitle( const char* title, int width)
{
	int t, aux1, aux2;
	int titlelen = strlen( title);

	printf("+"); for( t=0;t<width-2;t++)printf("-");printf("+\n");
	aux1 = width-2-titlelen;
	aux2 = aux1/2+aux1%2; aux1 = aux1/2;
	printf( "|");for( t=0; t<aux1; t++)printf(" ");
	printf( title);
	for( t=0; t<aux2; t++)printf(" "); printf("|\n");
	printf("+"); for( t=0;t<width-2;t++)printf("-");printf("+\n");
	
}

/***************************************************************************************
* Prints time statistics out to the screen
***************************************************************************************/
void printStats( const vector<Job>& completed, const vector<Job>& running)
{	
	const int row_width = 75;
	const char* running_title = "RUNNING JOBS";
	const char* completed_title = "COMPLETED JOBS";

	vector<Job>::size_type i;
	vector<Job>::size_type ncompleted = completed.size();
	vector<Job>::size_type nrunning = running.size();

	printTitle( running_title, row_width);
	
	if( !running.size()) 
		printf( "No jobs running in the background\n");
	else
		for( i=0; i<nrunning; i++)
			printf( "Job [%s]\n", running[i].toString().c_str());

	printf( "\n");

	printTitle( completed_title, row_width);
	if( !completed.size())
		printf( "No jobs executed\n");
	else
		for( i=0; i<ncompleted; i++)
			printf("Job [%s]\n\tUser time [%.2f seconds]\n\tSystem time [%.2f seconds]\n\n",
				completed[i].toString().c_str(), 
				completed[i].getTime(USER),
				completed[i].getTime(SYSTEM)
			);
			
}










