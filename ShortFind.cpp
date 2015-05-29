/*	ShortFind
*	-Finds the shortest path to a deadlock state
*	-Use with "deadlock_paths.txt" from Tracer.cpp
*
*
*
*
*	Written for the UMDES TRW Project (2015)
*	Austin Chen
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <cstdlib>
using namespace std;


//REQUIRES: cstring str
//MODIFIES: n/a
//EFFECTS: returns a boolean value whether or not a cstring is a valid integer
bool is_num(char *str) {
	while (*str)
	{
		if (!isdigit(*str++))
		{
			return false;
		}
	}
	return true;
}

int main(int argc, char* argv[])
{
	//check inputs
	if(argc > 3)
	{
		cout<<"Too many input arguments. Enter no flags or -h for help. Exiting..."<<endl;
		return 1;
	}

	
	if(argc <= 1 || (argc == 2 && (strcmp(argv[1], "-h") == 0)) || (argc == 3 && !is_num(argv[2])))
	{
		cout<<"ShortFind: finds shortest path to a deadlock state."<<endl;
		cout<<"Intended for use with \"deadlock_paths.txt\" from Tracer.cpp"<<endl;
		cout<<"By default, minimum string length is set to 0. Enter a non-negative integer after filename to specify a value."<<endl;
		cout<<"Usage: <executable> [-h] <deadlock_paths.txt> <minimum string length>"<<endl;
		cout<<"Exiting..."<<endl;
		return 2;
	}
	
	//set minimum string length, if specified. otherwise set it to 0
	unsigned int placeholder = 0;
	if(argc == 3) placeholder = atoi(argv[2]);
	static const unsigned int MINIMUM_STRING_LENGTH = placeholder;

	//open file
	fstream infile(argv[1]);

	//Check if file has been opened
	cout<<"Opening file..."<<endl;
	if(!infile.is_open())
	{
		cout<<"Error: Can't open file " << argv[1] <<". Exiting..."<<endl;
		return 3;
	}
	cout<<"Success!"<<endl;
	


	//find state with shortest string
	cout<<"Finding deadlock state with shortest string..."<<endl;
	string s;	//s is used to store strings that are being "worked with"
	vector<string> events;	//stores the events that lead to this state from the initial state
	vector<string> events_short;	//stores the events of the state with fewest events
	vector<string> states;	//stores the states that correspond to the path
	vector<string> states_short;	//stores the states of the current shortest path
	//get rid of first 3 lines
	for(int i = 0; i < 3; i++)
	{
		getline(infile, s);
	}
	while(getline(infile,s))
	{
		if(s == "")
		{
			//gets rid of "FIND PATH FOR DEADLOCK STATE... ", newline, and "Path is:"
			for(int i = 0; i < 3; i++)
			{
				getline(infile, s);
			}

			if(events.size() < events_short.size() || (events_short.size() == 0 && events.size() >= MINIMUM_STRING_LENGTH))
			{
				events_short = events;
				states_short = states;
			}

			events.clear();
			states.clear();
		}else{
			istringstream iss(s);
			string currentEvent, currentState;
			iss>>currentEvent>>currentState;
			events.push_back(currentEvent);
			states.push_back(currentState);
		}

		
	}


	//check if the string found has the required minimum length
	if(events_short.size() < MINIMUM_STRING_LENGTH)
	{
		cout<<"Error: Unable to find a string with minimum length "<<MINIMUM_STRING_LENGTH<<"."<<endl;
		cout<<"Exiting..."<<endl;
		return 5;
	}

	cout<<"Success!"<<endl<<endl;

	//report state with shortest string

	cout<<"List of events (length "<<events_short.size()<<") :"<<endl<<endl;
	for(unsigned int i = 0; i < events_short.size(); i++)
	{
		cout<<events_short.at(i)<<endl;
	}
	cout<<endl;

	//export result
	cout<<"Exporting result to short_path.txt"<<endl;
	ofstream outfile("short_path.txt");
	outfile<<"-------------------FINDING PATH FOR DEADLOCK STATE ";
	outfile<<states_short.at(states_short.size() - 1);
	outfile<<"--------------------"<<endl<<endl;
	outfile<<"Path is:"<<endl;
	for(unsigned int i = 0; i < events_short.size(); i++)
	{
		outfile<<events_short.at(i)<<"\t"<<states_short.at(i)<<endl;
	}
	cout<<endl;

	cout<<"Exiting..."<<endl;
	return 0;
}
