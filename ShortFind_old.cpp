/*	ShortFind
*	-Finds the shortest string of events that lead to a blocking state
*	-Use with 'blocking-events-full.txt' from Stephen Lanham's CountBlockingStates
*	-Minimum number of events to find can be specified as a command line argument
*	-Outputs the discovered state along with its event string
*	-Finds the first state satisfying requirements, e.g. if 2 states have the shortest strings then the first one found will be output
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
		cout<<"ShortFind: finds shortest string given a list of blocking states and their shortest path."<<endl;
		cout<<"Intended for use with 'blocking-events-full.txt' from Stephen Lanham's CountBlockingStates."<<endl;
		cout<<"By default, minimum string length is set to 0. Enter a non-negative integer after filename to specify a value."<<endl;
		cout<<"Usage: <executable> [-h] <blocking-events-full.txt> <minimum string length>"<<endl;
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
	cout<<"Finding state with shortest string..."<<endl;
	string name = "";	//stores name of state
	string name_short = "";	//stores name of state with shortest string
	string s;	//s is used to store strings that are being "worked with"
	vector<string> events;	//stores the events that lead to this state from the initial state
	vector<string> events_short;	//stores the events of the state with fewest events

	while(getline(infile,s))
	{
		if(s != "State found which transitions out of non-blocking space" && s != "Blocking state explored")
		{
			if(s.empty()) continue;	//gets rid of empty newlines
			istringstream instring(s);
			instring >> s;	//gets transition name
			events.push_back(s);
			instring >> s;	//gets rid of list of states after transition name

		}else{
			
			//cout<<"Current smallest: "<<events_short.size()<<endl; 	//<--DEBUG MESSAGE
			//cout<<"Current: "<<events.size()<<endl;			//<--DEBUG MESSAGE

			//check to see if current state has a shorter string than the one stored
		
			//if short name is empty, that means that no states have been read yet - make first state the short state
			if(name_short == "")
			{
				name_short = name;
				events_short = events;

				//the first state does not satisfy the minimum string length - erase it and continue
				if(events_short.size() < MINIMUM_STRING_LENGTH)
				{
					name_short = "";
					events_short.clear();
				}
				
			}else if(events.size() < events_short.size() && events.size() >= MINIMUM_STRING_LENGTH)	//exclude strings less than minimum string length
			{
				name_short = name;
				events_short = events;
				//cout<<"CHANGED"<<endl;				//<--DEBUG MESSAGE
			}

			getline(infile, name);	//reads name of state
			getline(infile, s);	//gets rid of 'Transition History:' or 'Marked? YES/NO'
			events.clear();

		}

		
	}

	//compare last state to shortest

	//cout<<"Current smallest: "<<events_short.size()<<endl;			//<--DEBUG MESSAGE
	//cout<<"Current (last):"<<events.size()<<endl;					//<--DEBUG MESSAGE
	if(events.size() < events_short.size() && events.size() >= MINIMUM_STRING_LENGTH)	//exclude strings less than minimum string length
		{
			name_short = name;
			events_short = events;
			//cout<<"CHANGED (last)"<<endl;					//<--DEBUG MESSAGE
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
	cout<<"State with shortest string: "<<endl<<endl<<name_short<<endl<<endl;
	cout<<"List of events (length "<<events_short.size()<<") :"<<endl<<endl;
	for(unsigned int i = 0; i < events_short.size(); i++)
	{
		cout<<events_short.at(i)<<"  ";
	}
	cout<<endl<<endl;

	cout<<"Exiting..."<<endl;
	return 0;
}
