#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

using namespace std; 

int getInput();
void printGrammarArray(int);
void cleanInput(int);
void get_input_array(int, int, int, int);
void fill_rules_map(int, int, int, int);
void print_input_array(int);
void print_rules_map();
void print_rules_string_map();
int validateInput(int);
int validateTerminals(int);
int validateNonTerminals(int);
void fillTerminalSet();
void fillNonTerminalSet();
void printTerminalSet();
void printNonTerminalSet();


string grammar_array[100];
char input_array[100][100];
set <char> terminal_set;
set <char> non_terminal_set;
set<char>::iterator it;
set<string> rules_string_set;
set<string>:: iterator rules_string_set_it;
map<char, string> rules_map;
map<char, string>::iterator rules_map_it;
map<char, set<string> > rules_string_map;
map<char, set<string> >:: iterator rules_string_map_it;

int main () 
{
	int count;
	int isValid;

	count = getInput();
	cleanInput(count);
	print_input_array(count); 
	isValid = validateInput(count);
	if (isValid == 0)
		cout << "Input is not valid"<<endl;
	else
		cout << "Input is valid"<<endl;
	print_rules_string_map();
	fillTerminalSet();
	fillNonTerminalSet();
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

void print_rules_map()
{
//Just print  rules map
	cout<<"Printing rules map"<<endl;
	for(rules_map_it=rules_map.begin();rules_map_it!=rules_map.end();rules_map_it++)
	{
	 	cout<<(*rules_map_it).first<<"   "<<(*rules_map_it).second<<endl;
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
	cout<< "Rule count"<<" "<<rule_count<<endl;
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


int validateInput(int count)
{
// Validate whether the entered input is proper
	int isValidTerm;
	int isValidNonTerm;

   	isValidTerm = validateTerminals(count); 
	isValidNonTerm = validateNonTerminals(count);
	if (isValidTerm && isValidNonTerm)
		{
		return 1;
		}
	else
		{
		return 0;
		}
}

int validateTerminals(int count)
{
// Check if All terminals have some rule associated with it

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
			return 0;
	}
	return 1;
}

int validateNonTerminals(int count)
{
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
			return 0;
	}
	return 1;
}

void fillTerminalSet()
{
	int i=0;
	for(i=0;;i++)
	{
		if(input_array[0][i] == '\0')
			break;
		terminal_set.insert(input_array[0][i]);
	}
	printTerminalSet();
}

void printTerminalSet()
{
	cout<<"printing terminal set"<<endl;
	for(it=terminal_set.begin();it!=terminal_set.end();++it)
	{
		cout<<*it<<endl;
	}	
}

void fillNonTerminalSet()
{
	int i=0;
	for(i=0;;i++)
	{
		if(input_array[0][i] == '\0')
			break;
		non_terminal_set.insert(input_array[1][i]);
	}
	printNonTerminalSet();	
}

void printNonTerminalSet()
{
	cout<<"printing Non terminal set"<<endl;
	for(it=non_terminal_set.begin();it!=non_terminal_set.end();++it)
	{
		cout<<*it<<endl;
	}	
}

//TODO
//Input getting part need to be revisited for sure. 
