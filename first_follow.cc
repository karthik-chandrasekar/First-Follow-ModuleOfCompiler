#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <list>

using namespace std; 


//FUNCTIONS;

int getInput();
void printGrammarArray(int);
void cleanInput(int);
void get_input_array(int, int, int, int);
void fill_rules_map(int, int, int, int);

void print_input_array(int);
void print_rules_string_map();

void validateInput(int);

void fill_terminal_list();
void fill_non_terminal_list();
void print_terminal_list();
void print_non_terminal_list();
void print_terminal_set();
void print_non_terminal_set();

void validateErrorCode0(int);
void validateErrorCode1(int);
void validateErrorCode2();
void validateErrorCode3();
void validateErrorCode4(int);
void checkForError();

//FINDING FIRST SETS
void collectFirstSet();
void findFirstSet(char);
void printFirstSet();


//FINDING FOLLOW SETS:
void collectFollowSet();
void findFollowSet();
void printFollowSet();

//GLOBAL DATA STRUCTURES

string grammar_array[100];
char input_array[100][100];

//SET 
set<string> rules_string_set;
set<string>:: iterator rules_string_set_it;
set<int> error_set;
set<int>::iterator error_set_it;
set<char> rules_key_set;
set<char> terminal_set;
set<char> non_terminal_set;
set<char>:: iterator set_it;
set<char> first_set;

//MAP
map<char, string> rules_map;
map<char, string>::iterator rules_map_it;
map<char, set<string> > rules_string_map;
map<char, set<string> >:: iterator rules_string_map_it;
map<char, set<char> > first_set_map;
map<char, set<char> > ::iterator first_set_map_it;

//LIST
list<char> terminal_list;
list<char> non_terminal_list;
list<char>::iterator list_it;


int main () 
{
	int count;
	int isValid;

	count = getInput();
	cleanInput(count);
	print_input_array(count);
	fill_terminal_list();
	fill_non_terminal_list();
	
	//Validation Part 
	validateInput(count);
	checkForError();
	print_rules_string_map();
	collectFirstSet();

	return 0;
} 

int getInput()
{
//Get the input per line. 

	string line;
	int i, count =0;
	cout << "Enter grammar specification"<<endl;
	while(true)
	{
		getline(cin, line);
		if (line.empty())
			break;
		grammar_array[count] = line;
		count++;	
	}
	printGrammarArray(count);
	return count;
}

void printGrammarArray(int count)
{
	int i=0;
	for(i=0;i < count; i++)
	{	
		cout << "Grammar specification" << "  " <<grammar_array[i]<<endl;
	}
}

void print_input_array(int count)
{
//Just print the array
	cout << "printing input array"<<endl;
	int i,j;
	for (i=0;i< count; i++)
	{
		for (j=0;;j++)
		{
			if (input_array[i][j] == '\0')
				break;
			//cout<<"i "<<i<<"  "<<"j "<<j<<" "<<"value "<<input_array[i][j]<<" "<<endl;
			cout<<input_array[i][j]<<" ";
		}
		cout<<endl;
	}
}


void print_rules_string_map()
{
//Just print  rules string map
	cout<<"Printing rules map"<<endl;
	for(rules_string_map_it=rules_string_map.begin();rules_string_map_it!=rules_string_map.end();rules_string_map_it++)
	{
	 	cout<<(*rules_string_map_it).first<<"   ";
		rules_string_set = (*rules_string_map_it).second;
		for(rules_string_set_it=rules_string_set.begin(); rules_string_set_it!=rules_string_set.end(); rules_string_set_it++)
		{
			cout<<*rules_string_set_it<<"  ";
		}
		cout<<endl;
	}
}

void cleanInput(int count)
{
//Inputs can have multiple statements in a single statement separated by # and ## and the end of grammar. So properly put them in an array such that every single rule is separately stored. 

	int rows_count = count;
	int start_col;	
	int end_col = 0;
	int rule_count = 0;
	int i,j;

	for(i=0;i<rows_count;i++)
	{
		start_col =0;
		for(j=0;;j++)
		{
			if(grammar_array[i][j] == '#')   //If we encounter # it is end of a rule
			{
				end_col = j;
				get_input_array(i,start_col, end_col, rule_count);
				rule_count += 1;
				start_col = end_col;
				break;
			}
		}
	}
}

void get_input_array(int row, int start_col, int end_col, int rule_count)
{
	int i;
	int j=0;
	for(i=start_col; i<end_col; i++)
	{
		if (isspace(grammar_array[row][i]))		
			continue;
		input_array[rule_count][j] = grammar_array[row][i];	
		j++;
	}
	input_array[rule_count][j] = '\0';
	if (row > 1)
	{
		fill_rules_map(row, start_col, end_col, rule_count);
	}
}

void fill_rules_map(int row, int start_col, int end_col, int rule_count)
{
	string rule_value;
	char rule_value_array[10];
	int i, j;

	for(i=3,j=0;input_array[row][i] != '\0';i++,j++)
	{
		rule_value_array[j] = input_array[row][i];		
	}

	if(input_array[row][3] == '\0')
	{
		rule_value_array[j] = 'Z';
		j++;
	}

	rule_value_array[j] = '\0';

	rules_map[input_array[row][0]] = rule_value_array;

	if (rules_string_map.count(input_array[row][0]) == 0)
	{
		rules_string_set.insert(rule_value_array);
		rules_string_map[input_array[row][0]] = rules_string_set;

	}
	else
	{
		rules_string_set = rules_string_map[input_array[row][0]];
		rules_string_set.insert(rule_value_array);
		rules_string_map[input_array[row][0]] = rules_string_set;
	}
	rules_string_set.clear();
}


void  validateInput(int count)
{
// Validate whether the entered input is proper

   	validateErrorCode4(count); 
	validateErrorCode1(count);
	validateErrorCode3();
	validateErrorCode2();
}

void checkForError()
{
//Analyse error_set and display errors and terminate the program execution
	
	int to_stop_execution = 0;
	for(error_set_it = error_set.begin();error_set_it != error_set.end(); error_set_it++)
	{
		cout<<"ERROR CODE : "<< *(error_set_it)<<endl;
		to_stop_execution = 1;
	}
	if (to_stop_execution == 1)
		//exit(0);
		cout << "The program shud have been terminated";
}


void collectFirstSet()
{
// Collect first sets 		
	for(list_it = non_terminal_list.begin(); list_it!=non_terminal_list.end(); list_it++)
	{
		cout<< "findFirstSet called for "<<*list_it<<endl;
		findFirstSet(*list_it);
	}
	printFirstSet();	
}

void findFirstSet(char terminal_char)
{
//Compute first set
	
	int to_break = 0;
	int i;
	string temp;
	set<char> temp_set;
	int count = 0;

	if (!(isalpha(terminal_char)))
		return;

	if (!(rules_key_set.count(terminal_char)))
		return;

	rules_string_map_it = rules_string_map.find(terminal_char);
	rules_string_set = (*rules_string_map_it).second;

	cout << "find first set - BEGINSSSSS for   "<< terminal_char<<" "<<endl<<endl;

	for(rules_string_set_it=rules_string_set.begin();rules_string_set_it != rules_string_set.end(); rules_string_set_it++)
	{
		cout<< "Inside first for loop of first set"<<endl;
		
		temp = *rules_string_set_it;
		cout << "Rule string is "<<temp<<endl;
		for(i=0;temp[i]!='\0';i++)
		{
			if (to_break == 1)
				break;
			cout<< "Inside second for loop of find first set for "<<temp[i]<<"  "<<endl;
			
			first_set.clear();
			temp_set.clear();
			if(terminal_set.count(temp[i]) > 0)
			{
				
				cout << "Inside if" <<endl;	
				if(first_set_map.count(terminal_char) == 0)
				{	
					cout<< "First time seeing term - adding it"<<endl;
					first_set.insert(temp[i]);
					first_set_map[terminal_char] = first_set;	
				}
				else
				{
					cout << "ALready first set is present for "<< terminal_char<<endl;
					first_set = first_set_map[terminal_char];
					first_set.insert(temp[i]);
					first_set_map[terminal_char] = first_set;	
				}
				to_break = 1;
			}

			else
			{
				//It is a  non terminal. Collect the first set of the non terminal (It is assumed that it has been calculated already) and append it with the first set of this non terminal. 
			
				cout << "Inside else" <<endl;	
				if (first_set_map.count(temp[i]) > 0)
				{		
					cout << "Inside if - Right flow"<<endl;
					//First set is already calculated for this non terminal
					temp_set = first_set_map[temp[i]];
					first_set.insert(temp_set.begin(), temp_set.end());
				}
				
				else
				{
					if(isalpha(temp[i]))
					{
						cout<<"  "<<temp[i]<<" "<<"Recursive call - SUSPICIOUS- call "<<count <<endl<<endl;
						count ++;
						if (temp[i] == 'Z')
						{
							cout<< "Presence of Z is identified"<<endl;
							temp_set.insert('Z');
						}
						else
						{
							findFirstSet(temp[i]);
							temp_set = first_set_map[temp[i]];
						}
						if(first_set_map.count(terminal_char) ==0 )
						{
							first_set.insert(temp_set.begin(), temp_set.end());
							first_set_map[terminal_char] = first_set;
						}
						else
						{
							first_set = first_set_map[terminal_char];
							first_set.insert(temp_set.begin(), temp_set.end());
							first_set_map[terminal_char] = first_set;
						}
					}	
				}			

				
				//check for the presence of epsilon in tempset and it if present, continue looping. Else break
				if (temp_set.count('Z') != 0)
				{
					cout<<"Z is present "<<endl;
					continue;
				}
				else
					to_break =1;
			}
		}
	}
	cout<< "finding first set - Ends"<<endl<<endl;			
}

void printFirstSet()
{
//Printing First set

	cout<< "Print first set - starts"<<endl<<endl;
	for(first_set_map_it = first_set_map.begin();first_set_map_it != first_set_map.end(); first_set_map_it++)
	{
		
		cout<<"First set of "<<(*first_set_map_it).first<<endl;
		cout<<"Values are"<<endl;
		first_set = (*first_set_map_it).second;
		for(set_it = first_set.begin(); set_it != first_set.end(); set_it++)
		{
			cout<<*set_it<< "  ";
		}
		cout<<endl;
	}
	cout << "Print first set - ends"<<endl<<endl;
}


void collectFolloweSet()
{
//Collect follow sets
	for(list_it = non_terminal_list.begin(); list_it != non_terminal_list.end(); list_it++)
	{
		cout<<"findFollowSet called for "<<*list_it<<endl;
		findFollowSet(*list_it);
	}
	printFollowSet();
}


void findFollowSet()
{

}

void printFollowSet()
{


}

void validateErrorCode4(int count)
{
// Check for ERROR  4

	string terminals;
	int term = 0;
	int i, j, k;
	
	terminals = input_array[0];

	for(i=0;i<count;i++)
	{
		term = 0;
		if (!(isalpha(terminals[i])))
			continue;
		for(j=0,k=0;j<count;j++)
		{
			//cout<<"terminal "<<terminals[i]<<" "<<"Input "<<input_array[j][k]<<" "<<"index "<<"i "<<i <<"j "<<j<<"k "<<k<<endl;
			if (terminals[i] == input_array[j][k])
			   {		
				term = 1;
				break;
			   }
		}
		if(term ==0 )
			error_set.insert(4);
	}
}


void validateErrorCode0()
{
//check for syntax error
	cout << "validate error code 1 function is empty";		
}

void validateErrorCode1(int count)
{
	//Check for error code 1
	string non_terminal;
	int non_term = 0;
	int i, j, k;

	non_terminal = input_array[1];
	
	for(i=0;i<count;i++)
	{
		non_term = 0;
		if (!isalpha(non_terminal[i]))
			continue;
		for(j=2;j<count;j++)
		{
			for(k=3;;k++)
			{
				if (input_array[j][k] == '\0')
					break;
				if (!(isalpha(input_array[j][k])))
					continue;
				//cout<<"terminal "<<non_terminal[i]<<" "<<"Input "<<input_array[j][k]<<" "<<"index "<<"i "<<i <<"j "<<j<<"k "<<k<<endl;
				if (non_terminal[i] == input_array[j][k])
				{
					non_term = 1;
					break;
				}
			}
			if (non_term == 1)
				break;
		}
		if (non_term ==0)
			error_set.insert(1);
	}
}

void getRulesKeySet()
{
//Generate rules key set
	for(rules_string_map_it = rules_string_map.begin(); rules_string_map_it != rules_string_map.end(); rules_string_map_it++)
		{
			rules_key_set.insert((*rules_string_map_it).first);
			cout << (*rules_string_map_it).first<< " ";
		}
		cout<<endl;
}

void validateErrorCode3()
{
//check for error code 3 (i.e) If any terminal is on the left side of the rule. 
	getRulesKeySet();
	//cout<< "Rule set : ";	
	for(list_it=terminal_list.begin(); list_it!=terminal_list.end(); list_it++)
	{
		if (rules_key_set.count(*list_it) >= 1)
			{
				error_set.insert(3);
			}
	//cout<<*list_it<<" ";
	}
	cout<<endl;
}

void validateErrorCode2()
{
	string temp;
	int i;
	for(rules_string_map_it = rules_string_map.begin(); rules_string_map_it != rules_string_map.end(); rules_string_map_it++)
	{
		//Rules key check
		if(non_terminal_set.count((*rules_string_map_it).first) == 0)
			cout<<"Key accused"<<endl;
			error_set.insert(2);
			cout<<(*rules_string_map_it).first<<endl;

		//Rules value check
		rules_string_set = (*rules_string_map_it).second;
		for(rules_string_set_it=rules_string_set.begin(); rules_string_set_it!=rules_string_set.end(); rules_string_set_it++)
		{
			temp = *rules_string_set_it;
			for(i=0;;i++)
			{
				if (temp[i]=='\0')
					break;
				if (terminal_set.count(temp[i]) == 0 && non_terminal_set.count(temp[i]==0))
					error_set.insert(2);
					cout<<"Rule accused of error  "<<endl;
					cout<<temp[i]<<endl;
			}
		}
	
	}
}

void fill_terminal_list()
{
	int i=0;
	for(i=0;;i++)
	{
		if(input_array[1][i] == '\0')
			break;
		terminal_list.push_back(input_array[1][i]);
		terminal_set.insert(input_array[1][i]);
	}
	print_terminal_list();
	print_terminal_set();
}

void print_terminal_list()
{
	cout<<"printing terminal list"<<endl;
	for(list_it=terminal_list.begin();list_it!=terminal_list.end();list_it++)
	{
		cout<<*list_it<<endl;		
	}
}


void print_terminal_set()
{
	cout<<"Printing terminal set"<<endl;
	for(set_it=terminal_set.begin();set_it!=terminal_set.end();set_it++)
	{
		cout<<*set_it<<endl;
	}
}

void fill_non_terminal_list()
{
	int i=0;
	for(i=0;;i++)
	{
		if(input_array[0][i] == '\0')
			break;
		non_terminal_list.push_back(input_array[0][i]);
		non_terminal_set.insert(input_array[0][i]);
	}
	print_non_terminal_list();
	print_non_terminal_set();
}

void print_non_terminal_list()
{
	cout<<"printing non terminal list"<<endl;
	for(list_it=non_terminal_list.begin();list_it!=non_terminal_list.end();++list_it)
	{
		cout<<*list_it<<endl;
	}
}

void print_non_terminal_set()
{
	cout<<"printing non terminal set"<<endl;
	for(set_it=non_terminal_set.begin();set_it!=non_terminal_set.end();set_it++)
	{
		cout<<*set_it<<endl;
	}

}

//TODO
//Input getting part need to be revisited for sure. 
// Errorcode 2 and 0 has to be revisited
