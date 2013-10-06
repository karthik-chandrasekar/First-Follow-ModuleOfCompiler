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
void printRawInput(int);
int  splitMultipleLines(int);
void printGrammarArray(int);
void parseInput(int);
void parseSingleLine(string, int);
void printInput();
void addRule(int);
void formTerminalList();
void formNonTerminalList();
void formRuleMap();
void printGrammarRulesMap();
int isListEmpty();
void addDollarForFollow(string);

void validateInput(int);
void checkForError(int);
void checkForErrorCode0(int);
void findForErrorCode0(string);
void checkForErrorCode1();
void checkForErrorCode2();
void checkForErrorCode3();
void checkForErrorCode4();
void printError();


//FIRST_SET
void collectFirstSet();
void findFirstSet(string);
void remove_z(string);
void printFirstSet();
int stringcmp(string, string);

//FOLLOW SET
void collectFollowSet();
void findFollowSet(string);
void printFollowSet();
void getLastStrRule(string);

//GLOBAL DATA STRUCTURES

string grammar_array[100];
char input_array[100][100];
string raw_input[100];


//SET 
set<string> rules_string_set;
set<string>:: iterator rules_string_set_it;
set<string> temp_set;
set<string> first_set;
set<string> non_terminal_called_for_first_set;
set<string> terminal_set;
set<string> non_terminal_set;
set<string> :: iterator symb_it;
set<int> error_code_set;
set<int>::iterator error_code_set_it;
set<string> non_terminal_called_set;
set<string> follow_set;
set<string> first_str_rule_set;
set<string> grammar_rule_key_set;
set<string> all_symb_grammar_set;
set<string> non_term_track_set;
set<char> allowed_spl_char_set;

//MAP
map<string, list<list<string> > > grammar_rules_map;
map<string, list<list<string> > >:: iterator grammar_rules_map_it;
map<string, set<string> > first_set_map;
map<string, set<string> >:: iterator first_set_map_it;
map<string, set<string> > follow_set_map;
map<string, set<string> >:: iterator follow_set_map_it;
map<string, int> non_term_epsilon_map;

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
	validateInput(count);
	collectFirstSet();
	//collectFollowSet();

	return 0;
} 

int getInput()
{
//Get the input per line. 

	string line;
	int count =0;
	//cout << "Enter grammar specification"<<endl;
	while(true)
	{
		getline(cin, line);
		if (line.empty())
			break;
		raw_input[count] = line;
		count++;	
	}
	//printRawInput(count);
	count = splitMultipleLines(count);
	//printGrammarArray(count);
	parseInput(count);
	printGrammarRulesMap();
	return count;
}

void validateInput(int count)
{
	checkForError(count);
	printError();
}

void checkForError(int count)
{
	checkForErrorCode0(count);
	checkForErrorCode1();
	checkForErrorCode2();
	checkForErrorCode3();
	checkForErrorCode4();
}

void checkForErrorCode0(int count)
{
	int i=0;
	allowed_spl_char_set.insert('-');
	allowed_spl_char_set.insert('>');
	allowed_spl_char_set.insert('#');
	while(i<count)
	{
		findForErrorCode0(grammar_array[i]);	
		i++;
	}
}

void findForErrorCode0(string single_line)
{
	int i=0;
	int string_length = single_line.length();

	while(i< string_length)
	{
		i++;	
		if(single_line[i] == '\0')
			break;
		if(isalpha(single_line[i]) or isdigit(single_line[i]))
			continue;
		else if	(isspace(single_line[i]))
			continue;
		else if(allowed_spl_char_set.count(single_line[i])>0)
			continue;
		else
		{
			error_code_set.insert(0);
			break;
		}
	}
}

void checkForErrorCode1()
{
	for(symb_it = non_terminal_set.begin(); symb_it != non_terminal_set.end(); symb_it++)
	{
		if(grammar_rule_key_set.count(*symb_it) ==0)
		{
			error_code_set.insert(1);
			break;
		}
	}
}

void checkForErrorCode2()
{
	for(symb_it = grammar_rule_key_set.begin(); symb_it != grammar_rule_key_set.end(); symb_it++)
	{
		if(non_terminal_set.count(*symb_it) ==0)
		{
			error_code_set.insert(2);
			break;
		}
	}

	for(symb_it = all_symb_grammar_set.begin(); symb_it != all_symb_grammar_set.end(); symb_it++)
	{
		if(non_terminal_set.count(*symb_it) ==0)
		{
			if(terminal_set.count(*symb_it)==0)
			{
				error_code_set.insert(2);
				break;
			}
		}
	}
}


void checkForErrorCode3()
{
	for(symb_it = terminal_set.begin(); symb_it != terminal_set.end(); symb_it++)
	{
		if(grammar_rule_key_set.count(*symb_it)>0)
		{
			error_code_set.insert(3);
			break;
		}
	}
}


void checkForErrorCode4()
{
	for(symb_it=terminal_set.begin(); symb_it != terminal_set.end(); symb_it++)
	{
		if(all_symb_grammar_set.count(*symb_it) == 0)
		{
			error_code_set.insert(4);
			break;
		}
	}
}


void printError()
{
	int to_exit=0;
	if (error_code_set.count(0)>0)
	{
		cout<<"ERROR CODE " <<'0'<<endl;
		exit(1);
	}
	for(error_code_set_it = error_code_set.begin(); error_code_set_it != error_code_set.end(); error_code_set_it++)
	{
		cout<<"ERROR CODE "<<*error_code_set_it<<endl;
		to_exit = 1;
	}
	if (to_exit == 1)
		exit(1);
}

void printRawInput(int count)
{
	int i;
	for(i=0;i < count; i++)
	{	
		cout << "Raw Input" << "  " <<raw_input[i]<<endl;
	}
}

int splitMultipleLines(int count)
{
	int i=0;
	int j, line_length;
	string line;
	int start_word, end_word;
	int to_break =0;
	int local_break;
	int new_row_count =0;
	
	while(i<count)
	{
		line = raw_input[i];
		line_length = line.length();
		i++;
		start_word = 0;
		end_word = 0;
		local_break =0;
		j =0;
		
		if(to_break == 1)
			break;
	
	
		while(j < line_length)
		{
			if(to_break == 1 || local_break ==1)
				break;

			start_word = j;
			while(line[j]!= '#' && j<line_length)
			{
				j++;
			}	
			end_word = j;
				
			while(isspace(line[start_word]))
			{
				start_word++;
				if(start_word == end_word)
				{
					local_break =1;
					break;
				}
			}
			while(isspace(line[end_word]))
				end_word--;
			if(line[j+1]=='#')
			{
				to_break=1;
			}
			else if(start_word != end_word)
			{
				grammar_array[new_row_count] = line.substr(start_word, (end_word-start_word));
				grammar_array[new_row_count][end_word] = '\0';
				new_row_count ++;
			}
			j++;
		}
	}
	return new_row_count;
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

		if(single_line[i] == '\0')
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
				to_break = 0;
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
	int is_list_empty = 0;

	key = single_rule_list.front();
	single_rule_list.pop_front();
	is_list_empty = isListEmpty();
	if (is_list_empty == 0)
		single_rule_list.push_back("#");
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
int isListEmpty()
{
	int i=0;
	int not_empty =0;
	string temp;

	for(single_rule_list_it = single_rule_list.begin(); single_rule_list_it != single_rule_list.end(); single_rule_list_it++)
	{
		i=0;
		temp = *single_rule_list_it;
		while(temp[i]!='\0')
		{	
		 	if(isalpha(temp[i]))
			{
				not_empty = 1;	
			}
			i++;
		}
		if (not_empty ==1)
		 return 1;
	}
return 0;

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
		multiple_rule_list = (*grammar_rules_map_it).second;
		for(multiple_rule_list_it = multiple_rule_list.begin(); multiple_rule_list_it != multiple_rule_list.end(); multiple_rule_list_it++)
		{
			//cout<< (*grammar_rules_map_it).first<<"   " ;
			grammar_rule_key_set.insert((*grammar_rules_map_it).first);
			single_rule_list = *multiple_rule_list_it;
			if(stringcmp(single_rule_list.back(), "#")==0)
				non_term_epsilon_map[(*grammar_rules_map_it).first] = 1;	
			for(single_rule_list_it=single_rule_list.begin(); single_rule_list_it != single_rule_list.end(); single_rule_list_it++)
			{
				//cout<<*single_rule_list_it<<"  ";
				if (stringcmp((*single_rule_list_it), "#") ==1)
					continue;
				else
					all_symb_grammar_set.insert((*single_rule_list_it));
			}
			//cout<<endl;
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
	//Local DS
	int to_break = 0;
	int to_remove_z = 0;
	int i;
	string cur_str;
	set<string> temp_set;
	set<string> first_set;
	int count = 0;
	int rule_count =0;
	list<string> single_rule_list;
	list<string> :: iterator single_rule_list_it;
	list<list<string> > multiple_rule_list;
	list<list<string> >:: iterator multiple_rule_list_it;
	

	if(non_terminal_called_for_first_set.count(non_terminal)>0)
		return;

	non_terminal_called_for_first_set.insert(non_terminal);

	
	multiple_rule_list = grammar_rules_map[non_terminal];		

	cout<<endl<<endl;
	cout<<"BEGINS for "<<non_terminal<<endl<<endl;
		
		for(multiple_rule_list_it = multiple_rule_list.begin(); multiple_rule_list_it != multiple_rule_list.end(); multiple_rule_list_it++)
		{
			to_break =0;
			to_remove_z = 0;
			single_rule_list = *multiple_rule_list_it;
			cout<<"rule count for non terminal  "<< non_terminal<< "  is  "<<rule_count<<endl<<endl;

			for(single_rule_list_it=single_rule_list.begin(); single_rule_list_it != single_rule_list.end(); single_rule_list_it++)
			{
				if(to_break ==1)
				{
					cout<<"breaking for "<<*single_rule_list_it<<" Non term is "<<non_terminal<<endl;	
					break;
				}
	
				to_break = 0;	
				first_set.clear();
				temp_set.clear();

				cur_str = *single_rule_list_it;

				cout<<"dealing with "<<cur_str<<endl;

				if (terminal_set.count(cur_str) > 0)
				{
					
					if(first_set_map.count(non_terminal) == 0)
					{
						cout << "Terminal "<<cur_str<<endl;
						first_set.insert(cur_str);
						first_set_map[non_terminal] = first_set;
					}			
					else
					{
						cout << "Terminal "<<cur_str<<endl;
						first_set = first_set_map[non_terminal];
						first_set.insert(cur_str);
						first_set_map[non_terminal] = first_set;
					}	
					to_break = 1;
				}
				
				else
				{
					if (first_set_map.count(cur_str)>0)
					{
						cout<< "Cur str first set already present "<< cur_str<<endl;
						temp_set = first_set_map[cur_str];
						if (first_set_map.count(non_terminal)>0)
							first_set = first_set_map[non_terminal];
						first_set.insert(temp_set.begin(), temp_set.end());
						first_set_map[non_terminal] = first_set;
					}
					
					else 
					{
						if (non_terminal_set.count(cur_str) ==0)
						{
							cout << "EPSILON IDENTIFIED for "<<non_terminal<<endl;
							temp_set.insert("#");
							if (first_set_map.count(non_terminal)>0)
								first_set = first_set_map[non_terminal];
							first_set.insert(temp_set.begin(), temp_set.end());
							first_set_map[non_terminal] = first_set;
						}
						else
						{
							cout<<" "<<"RECURSION call "<< cur_str<< "   for   "<<non_terminal<<"  count is "<<count<<endl<<endl;
							count++;
							findFirstSet(cur_str);
							cout<< "OUT OF RECURSION   "<<cur_str<<"  for "<<non_terminal<<"and to break option is "<< to_break<<endl<<endl;
							temp_set = first_set_map[cur_str];
							if(first_set_map.count(non_terminal) ==0)
							{

								cout<<"Adding first set for non terminal "<<non_terminal<<endl;
								first_set.insert(temp_set.begin(), temp_set.end());
								first_set_map[non_terminal] = first_set;
			
							}
							else
							{

								cout<< "Adding first set for non terminal "<<non_terminal<<endl;
								first_set = first_set_map[non_terminal];
								first_set.insert(temp_set.begin(), temp_set.end());
								first_set_map[non_terminal] = first_set;	
				
							}

						}
					}
					if (temp_set.count("#") != 0)
					{
						cout<<"# is present for  "<<non_terminal<<endl;
						to_remove_z = 0;
					}
					else if(to_break ==1)
					{
						to_remove_z = 1;
					}
					else
					{
						to_break = 1;
						to_remove_z = 1;
						cout<<"call to remove z function"<<endl;
					}
				}
				}
				if(to_remove_z == 1)
				{
					remove_z(non_terminal);
				}
		rule_count ++;
		}
}

void remove_z(string non_terminal)
{
	cout<<"REMOVEEEE Z called for non terminal "<<non_terminal<<endl;
	set<string> first_set;
	set<string> new_first_set;
	set<string> :: iterator first_set_it;
	
	first_set = first_set_map[non_terminal];
	for(first_set_it = first_set.begin(); first_set_it != first_set.end(); first_set_it++)
	{
		if(stringcmp((*first_set_it), "#") == 1)
		{
			cout << "# removed for non terminal"<<non_terminal<<endl;
			continue;
		}
		new_first_set.insert(*first_set_it);
	}
	first_set_map[non_terminal] = new_first_set;
}


int stringcmp(string a, string b)
{
	//cout << "Comparing strings "<<a<<" "<<b<<endl;

	int i=0;
	if(a.length() != b.length())
		return 0;
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

	int comma_count = 0;
	int first_set_size =0;

	for(list_it = non_terminal_list.begin();list_it != non_terminal_list.end(); list_it++)
	{
		comma_count = 0;
		first_set_size = 0;
		
		cout<<"FIRST("<<(*list_it)<<")  =  { ";
		first_set = first_set_map[(*list_it)];
		
		first_set_size = (first_set.size() - 1);			

		for(rules_string_set_it = first_set.begin(); rules_string_set_it != first_set.end(); rules_string_set_it++)
		{
			cout<<*rules_string_set_it;
			if (comma_count< first_set_size)
			{
				cout<<", ";
				comma_count ++;
			}
		}
		cout<<" }"<<endl;
	}
}


//FOLLOW SET

void collectFollowSet()
{
	for(list_it = non_terminal_list.begin(); list_it != non_terminal_list.end(); list_it++)
	{
		findFollowSet(*list_it);
	}
	addDollarForFollow(non_terminal_list.front());
	printFollowSet();
}

void printFollowSet()
{
	int comma_count = 0;
	int follow_set_size = 0;

	for(list_it = non_terminal_list.begin(); list_it != non_terminal_list.end(); list_it++)
	{
		comma_count = 0;
		follow_set_size =0;

		cout<<"FOLLOW("<<(*list_it)<<") = { ";
		follow_set = follow_set_map[(*list_it)];
		follow_set_size = (follow_set.size() - 1);

		for(rules_string_set_it = follow_set.begin(); rules_string_set_it != follow_set.end(); rules_string_set_it++)
		{
			cout<<*rules_string_set_it;
			if (comma_count < follow_set_size)
			{
				cout<<", ";
				comma_count ++;
			}
		}
		cout<<" }"<<endl;
	}
}

void findFollowSet(string non_terminal)
{

	//Local DS
	list<string> track_list;
	int to_break =0, i=0;
	string cur_str;
	string prev_str, first_rule_str;
	list<string> single_rule_list;
	list<string> :: iterator single_rule_list_it;
	list<list<string> > multiple_rule_list;
	list<list<string> >:: iterator multiple_rule_list_it;
	set<string> temp_set;
	
	multiple_rule_list = grammar_rules_map[non_terminal];

	//cout<<endl<<endl;
	//cout<< "BEGINS for "<<non_terminal<<endl;
	if(non_terminal_called_set.count(non_terminal))
		return;
	non_terminal_called_set.insert(non_terminal);
	
	for(multiple_rule_list_it = multiple_rule_list.begin(); multiple_rule_list_it != multiple_rule_list.end(); multiple_rule_list_it++)
	{
		single_rule_list.clear();

		single_rule_list = *multiple_rule_list_it;

		track_list.clear();

		
		for(single_rule_list_it=single_rule_list.begin(); single_rule_list_it != single_rule_list.end(); single_rule_list_it++)
		{

			cur_str = *single_rule_list_it;
			
			cout<<"cur str is  "<< cur_str<<endl;
				
			if(terminal_set.count(cur_str)>0)
			{
				cout<<"Terminal  "<<cur_str<<endl;
		
				if(track_list.size()>0)
				{
					prev_str = track_list.back();
					non_term_track_set.clear();
					getLastStrRule(prev_str);
					first_str_rule_set.insert(prev_str);
					for(symb_it= first_str_rule_set.begin(); symb_it != first_str_rule_set.end(); symb_it++ )
					{
						prev_str = *symb_it;	
						cout<< "Prev str is "<<prev_str<<endl;
						if(non_terminal_set.count(prev_str)>0)
						{
							follow_set.clear();
							follow_set.insert(cur_str);
							if(follow_set_map.count(prev_str)>0)
							{
								temp_set = follow_set_map[prev_str];
								follow_set.insert(temp_set.begin(), temp_set.end());
							}
							follow_set_map[prev_str] = follow_set;
							cout<<"Added in follow of "<<prev_str<<endl;

						}
					}
					first_str_rule_set.clear();
				  }
			}
			else
			{
				if(non_terminal_set.count(cur_str) !=0)
				{
					cout<<" "<<"Recursion calling     "<<cur_str<<"  for "<<non_terminal<<endl;
					findFollowSet(cur_str);
					cout<< "Out of Recursion call    "<<cur_str<<"   for "<<non_terminal<<endl;

					if(track_list.size()>0)
					{
						prev_str = track_list.back();
						if(non_terminal_set.count(prev_str))
						{ 
							non_term_track_set.clear();
							first_str_rule_set.clear();
							temp_set.clear();

							temp_set = first_set_map[prev_str];
							if(follow_set_map.count(prev_str)>0)
							{
								follow_set = follow_set_map[prev_str];
								temp_set.insert(follow_set.begin(), follow_set.end());
							} 
							follow_set_map[prev_str] = temp_set;
						}
					}
				}
				if(follow_set.count("#") !=0)
				{
					cout<<"# is present"<<endl;
					continue;
				}	
			}
			track_list.push_back(cur_str);
		}
	}
}

void getLastStrRule(string cur_str)
{
	cout<<"Start of mystery function"<<endl;
	list<list<string> > all_rule_list;
	list<list<string> > ::iterator all_rule_list_it;	
	list<string> a_rule_list;
	string current_str;

	if (non_term_track_set.count(cur_str) >0)
		return;
	non_term_track_set.insert(cur_str);

	all_rule_list = grammar_rules_map[cur_str];
	
	for(all_rule_list_it = all_rule_list.begin(); all_rule_list_it != all_rule_list.end(); all_rule_list_it++)
	{
		a_rule_list = *all_rule_list_it;
		current_str = a_rule_list.back();
		if (non_terminal_set.count(current_str)>0)
		{	
			getLastStrRule(current_str);
		}
		first_str_rule_set.insert(current_str);
	}
}


void addDollarForFollow(string cur_str)
{
	set<string> :: iterator add_dollar_set_it;

	first_str_rule_set.clear();
	non_term_track_set.clear();

	getLastStrRule(cur_str);
	first_str_rule_set.insert(cur_str);	

	for(add_dollar_set_it = first_str_rule_set.begin(); add_dollar_set_it != first_str_rule_set.end(); add_dollar_set_it++)
	{

		if(non_terminal_set.count(*add_dollar_set_it)==0)
			continue;
		//cout<< "Adding $ for "<<*add_dollar_set_it<<endl;
		follow_set.clear();
		follow_set = follow_set_map[*add_dollar_set_it];
		follow_set.insert("$");
		follow_set_map[*add_dollar_set_it] = follow_set;

	}
}

