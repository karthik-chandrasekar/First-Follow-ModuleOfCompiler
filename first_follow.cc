#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <string.h>
#include <stdio.h>

using namespace std; 

//FUNCTIONS;

int getInput();
void printGrammarArray(int);
void parseInput(int);
void parseSingleLine(string, int);
void printInput();
void addRule(int);
void formTerminalList();
void formNonTerminalList();
void formRuleMap();
void printGrammarRulesMap();


//FIRST_SET
void collectFirstSet();
void findFirstSet(string);
void remove_z(string);
void printFirstSet();
int stringcmp(string, string);


//FINDING FOLLOW SETS:
void collectFollowSet();
void findFollowSet(char);
void printFollowSet();


char getFirstCharRuleString(char);

//GLOBAL DATA STRUCTURES

string grammar_array[100];
char input_array[100][100];

//SET 
set<string> rules_string_set;
set<string>:: iterator rules_string_set_it;
set<int> error_set;
set<int>::iterator error_set_it;
set<string> temp_set;
set<string> first_set;
set<string> non_terminal_called_for_first_set;
set<string> terminal_set;
set<string> non_terminal_set;


//MAP
map<string, list<list<string> > > grammar_rules_map;
map<string, list<list<string> > >:: iterator grammar_rules_map_it;
map<string, set<string> > first_set_map;
map<string, set<string> >:: iterator first_set_map_it;

//LIST
list<string> terminal_list;
list<string> non_terminal_list;
list<string>::iterator list_it;
list<string> single_rule_list;
list<string> :: iterator single_rule_list_it;
list<list<string> > multiple_rule_list;
list<list<string> >:: iterator multiple_rule_list_it;


int main () 
{
	int count;
	int isValid;

	count = getInput();
	collectFirstSet();

	return 0;
} 

int getInput()
{
//Get the input per line. 

	string line;
	int count =0;
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
	parseInput(count);
	//printInput();
	printGrammarRulesMap();
	return count;
}

void printGrammarArray(int count)
{
	int i;
	for(i=0;i < count; i++)
	{	
		cout << "Grammar specification" << "  " <<grammar_array[i]<<endl;
	}
}

void parseInput(int count)
{

	int i=0;
	while(i<count)
	{
		parseSingleLine(grammar_array[i], i);
		i++;
	}
}

void parseSingleLine(string single_line, int row_count)
{
	int i=0, to_break =0;
	int start_word =0 , end_word = 0;
	int string_length = single_line.length();
	string temp;
	
	single_rule_list.clear();
	multiple_rule_list.clear();	
	

	while(i < string_length)
	{
		if(to_break ==1)
			break;
		start_word = i;
		while(isalpha(single_line[i]) or (isdigit(single_line[i])))
		{
			i++;
			if(isspace(single_line[i]))
				break;
		}
		end_word = i;
		temp = single_line.substr(start_word, (end_word-start_word));
		single_rule_list.push_back(temp);

		while(!(isalpha(single_line[i])))
		{
			i++;
			if (single_line[i] == '#')
			{
				to_break = 1;
				break;
			}
		}
	}
	addRule(row_count);
}


void addRule(int row_count)
{

	if(row_count ==0)
	{
		formNonTerminalList();
	}
	else if(row_count ==1)
	{
		formTerminalList();
	}
	else
	{
		formRuleMap();
	}
}

void formNonTerminalList()
{

	for(single_rule_list_it = single_rule_list.begin(); single_rule_list_it != single_rule_list.end(); single_rule_list_it++)
	{
		non_terminal_list.push_back(*single_rule_list_it);
		non_terminal_set.insert(*single_rule_list_it);	
	}
}

void formTerminalList()
{

	for(single_rule_list_it = single_rule_list.begin(); single_rule_list_it != single_rule_list.end(); single_rule_list_it++)
	{
		terminal_list.push_back(*single_rule_list_it);
		terminal_set.insert(*single_rule_list_it);	
	}
}

void formRuleMap()
{

	string key;
	key = single_rule_list.front();
	single_rule_list.pop_front();
	if (grammar_rules_map.count(key) == 0)
	{
		multiple_rule_list.push_back(single_rule_list);
		grammar_rules_map[key] = multiple_rule_list;
	}
	else
	{
		multiple_rule_list = grammar_rules_map[key];
		multiple_rule_list.push_back(single_rule_list);
		grammar_rules_map[key] = multiple_rule_list;
	}	
} 
void printInput()
{
	for(single_rule_list_it = single_rule_list.begin(); single_rule_list_it != single_rule_list.end(); single_rule_list_it++)
	{
		cout<< *single_rule_list_it<<endl;
	}

}

void printGrammarRulesMap()
{
	for(grammar_rules_map_it = grammar_rules_map.begin(); grammar_rules_map_it != grammar_rules_map.end(); grammar_rules_map_it++)
	{
		cout<< (*grammar_rules_map_it).first<<"   " ;
		multiple_rule_list = (*grammar_rules_map_it).second;
		for(multiple_rule_list_it = multiple_rule_list.begin(); multiple_rule_list_it != multiple_rule_list.end(); multiple_rule_list_it++)
		{
			single_rule_list = *multiple_rule_list_it;
			for(single_rule_list_it=single_rule_list.begin(); single_rule_list_it != single_rule_list.end(); single_rule_list_it++)
			{
				cout<<*single_rule_list_it<<"  ";
			}
			cout<<endl;
		}
	}

}

void collectFirstSet()
{
	for(list_it = non_terminal_list.begin(); list_it != non_terminal_list.end(); list_it++)
	{
		findFirstSet(*list_it);
	}
	printFirstSet();

}

void findFirstSet(string non_terminal)
{

	int to_break = 0;
	int to_remove_z = 0;
	int i;
	string cur_str;
	set<string> temp_set;
	int count = 0;	

	if(non_terminal_called_for_first_set.count(non_terminal)>0)
		return;

	non_terminal_called_for_first_set.insert(non_terminal);

	
	multiple_rule_list = grammar_rules_map[non_terminal];		

	cout<<endl<<endl;
	cout<<"BEGINS for "<<non_terminal<<endl;
		
		for(multiple_rule_list_it = multiple_rule_list.begin(); multiple_rule_list_it != multiple_rule_list.end(); multiple_rule_list_it++)
		{
			single_rule_list = *multiple_rule_list_it;
			for(single_rule_list_it=single_rule_list.begin(); single_rule_list_it != single_rule_list.end(); single_rule_list_it++)
			{
				cur_str = *single_rule_list_it;
					
				if (terminal_set.count(cur_str) > 0)
				{
					first_set.clear();
					temp_set.clear();
					
					if(first_set_map.count(non_terminal) == 0)
					{
						first_set.insert(cur_str);
						first_set_map[non_terminal] = first_set;
					}			
					else
					{
						first_set = first_set_map[non_terminal];
						first_set.insert(cur_str);
						first_set_map[non_terminal] = first_set;
					}	
					to_break = 1;
				}
				
				else
				{
					cout<< "CHAR IS NON TERMINAL   "<< cur_str<<endl;
					if (first_set_map.count(cur_str)>0)
					{
						temp_set = first_set_map[cur_str];
						if (first_set_map.count(non_terminal)>0)
							first_set = first_set_map[non_terminal];
						first_set.insert(temp_set.begin(), temp_set.end());
						first_set_map[non_terminal] = first_set;
					}
					
					else 
					{
						if (stringcmp(cur_str,"Z"))
						{
							cout << "EPSILON IDENTIFIED"<<endl;
							temp_set.insert("Z");
							if (first_set_map.count(non_terminal)>0)
								first_set = first_set_map[non_terminal];
							first_set.insert(temp_set.begin(), temp_set.end());
							first_set_map[non_terminal] = first_set;
						}
						else
						{
							cout<<" "<<"RECURSION call "<< cur_str<< "   for   "<<non_terminal<<"count is "<<count<<endl<<endl;
							count++;
							findFirstSet(cur_str);
							cout<< "OUT OF RECURSION   "<<cur_str<<"  for "<<non_terminal<<endl;
							temp_set = first_set_map[cur_str];
							if(first_set_map.count(non_terminal) ==0)
							{
								first_set.insert(temp_set.begin(), temp_set.end());
								first_set_map[non_terminal] = first_set;
			
							}
							else
							{
								first_set = first_set_map[non_terminal];
								first_set.insert(temp_set.begin(), temp_set.end());
								first_set_map[non_terminal] = first_set;	
				
							}

						}
						if (temp_set.count("Z") != 0)
						{
							cout<<"Z is present"<<endl;
							to_remove_z = 0;
						}
						else
						{
							to_break = 1;
							to_remove_z = 1;
						}
					}
				}
				}
				if(to_remove_z)
				{
					remove_z(non_terminal);
				}
		}
}

void remove_z(string non_terminal)
{
	set<string> first_set;
	set<string> new_first_set;
	set<string> :: iterator first_set_it;
	
	first_set = first_set_map[non_terminal];
	for(first_set_it = first_set.begin(); first_set_it != first_set.end(); first_set_it++)
	{
		if(stringcmp((*first_set_it), "Z") == 0)
			continue;
		new_first_set.insert(*first_set_it);
	}
	first_set_map[non_terminal] = new_first_set;
}


int stringcmp(string a, string b)
{
	int i=0;
	for(i=0;a[i]!='\0' && b[i]!='\0';i++)
	{
		
		if(a[i] == b[i])
			continue;
		else
			return 0;
	}
	return 1;
}

void printFirstSet()
{
	cout<< "Print first set - starts"<<endl<<endl;
	for(first_set_map_it = first_set_map.begin();first_set_map_it != first_set_map.end(); first_set_map_it++)
	{
		
		cout<<"First set of "<<(*first_set_map_it).first<<endl;
		cout<<"Values are"<<endl;
		first_set = (*first_set_map_it).second;
		for(rules_string_set_it = first_set.begin(); rules_string_set_it != first_set.end(); rules_string_set_it++)
		{
			cout<<*rules_string_set_it<< "  ";
		}
		cout<<endl;
	}
	cout << "Print first set - ends"<<endl<<endl;
}
