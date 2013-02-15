/**************************************************************************************************
* Author: Santiago Herrera
* Email: sherre9@uic.edu
* Date: 02/2013
* Couse: CS385 - Operating Systems
* University: University of Illinois at Chicago
**************************************************************************************************/

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
void printJobStat( const Job&);

/***************************************************************************************
* ENTRY POINT
***************************************************************************************/
int main( int argc, char* argv[])
{
	string in;
	bool flag = 0;
	Job job;
	vector<Job> jobDump; //completed jobs
	vector<Job> runningJobs;//running jobs
	vector<Job>::size_type i;
	const char* user_cstr = getenv( "USER");
	string prompt = (user_cstr) ? user_cstr : ""; prompt += "$ ";

	printf( "Author: Santiago Herrera\n");
	printf( "ID/email: sherre9@uic.edu\n");
	printf( "Class: CS 385, University of Illinois at Chicago\n\n");
 
	while( !flag)
	{
		// prompt and get line
		printf("%s", prompt.c_str());
		getline( cin, in);
		in = trim( in);

		// wait for jobs in runningJobs //
		for( i=0; i<runningJobs.size(); i++) 
		{
			if( runningJobs[i].bgWait())
			{
				printf( "[%d]+ Done [%s]\n", runningJobs[i].id, runningJobs[i].toString().c_str());
				printJobStat( runningJobs[i]);
				jobDump.push_back( runningJobs[i]);
				runningJobs.erase( runningJobs.begin()+i);
				i--;
			}
		}
		
		job = in;

		if( in == "exit") 
			flag = 1;
		else if( in == "stats")
			printStats( jobDump, runningJobs);
		else if( job.getNCommands())
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
				printJobStat( job);
			}
		}
	}	

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
	vector<Job>::size_type i;
	vector<Job>::size_type ncompleted = completed.size();
	vector<Job>::size_type nrunning = running.size();

	// Print running and completed job stats
	printTitle( "RUNNING JOBS", row_width);
	
	if( !running.size()) 
		printf( "No jobs running in the background\n");
	else
		for( i=0; i<nrunning; i++)
			printf( "Job [%s]\n", running[i].toString().c_str());

	printf( "\n");

	printTitle( "COMPLETED JOBS", row_width);
	if( !completed.size())
		printf( "No jobs executed\n");
	else
		for( i=0; i<ncompleted; i++) printJobStat( completed[i]);

	printf( "\n");

	// Print current time
	//int getrusage(int who, struct rusage *usage);
	printTitle( "STATSH STATISTICS", row_width);
	struct rusage usage; memset( &usage, 0, sizeof( struct rusage));
	double sys_time, usr_time;
	getrusage( RUSAGE_CHILDREN, &usage);
	sys_time = (double)usage.ru_stime.tv_sec + usage.ru_stime.tv_usec/(double)1000000;
	usr_time = (double)usage.ru_utime.tv_sec + usage.ru_utime.tv_usec/(double)1000000;

	printf( "statsh User time [%.2f seconds]\nstatsh System time [%.2f seconds]\n\n"
			, usr_time, sys_time);


}

/**************************************************************************************************
**************************************************************************************************/
void printJobStat( const Job& job)
{
	printf("Job [%s]\n\tUser time [%.2f seconds]\n\tSystem time [%.2f seconds]\n",
		job.toString().c_str(), 
		job.getTime(USER),
		job.getTime(SYSTEM));
}










