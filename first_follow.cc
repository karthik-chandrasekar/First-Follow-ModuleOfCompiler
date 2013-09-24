#include <iostream>
#include <string>

using namespace std; 

int getInput();
void cleanInput(int);
void copyToken(int, int, int, int);
void printStructuredInput(int);
int validateInput(int);
int validateTerminals(int);
int validateNonTerminals(int);

string grammar_array[100];
string structured_input[100];

int main () 
{
	int count;
	int isValid;

	count = getInput();
	cleanInput(count);
	printStructuredInput(count); 
	isValid = validateInput(count);
	if (isValid == 0)
		cout << "Input is not valid"<<endl;
	return 0;
} 

int getInput()
{
//Get the input.

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

	for(i=0;i < count; i++)
	{	
		cout << "Grammar specification" << "  " <<grammar_array[i]<<endl;
	}
	return count;
}

void printStructuredInput(int count)
{
//Just print the array
	int i,j;
	for (i=0;i< count; i++)
	{
		for (j=0;;j++)
		{
			if (structured_input[i][j] == '\0')
				break;
			cout<<"i "<<i<<"  "<<"j "<<j<<" "<<"value "<<structured_input[i][j]<<" "<<endl;
		}
	}

}

void cleanInput(int count)
{
//Inputs can have multiple statements in a single statement separated by # and ## and the end of grammar. So properly put them in an array such that every single rule is separately stored. 

	int rows_count = count;
	int start_col = 0;	
	int end_col = 0;
	int rule_count = 0;
	int i,j;

	for(i=0;i<rows_count;i++)
	{
		for(j=0;;j++)
		{
			if(grammar_array[i][j] == '#')   //If we encounter # it is end of a rule
			{
				end_col = j;
				copyToken(i,start_col, end_col, rule_count);
				rule_count += 1;
				start_col = end_col;
				break;
			}
		
		}
	}
}


void copyToken(int row, int start_col, int end_col, int rule_count)
{
//Utility function to copy string from the given start and end index values
	int count =0;
	int i;
	
	for(i=start_col;i<end_col;i++)
	{
		if (isspace(grammar_array[row][i]))
		{
			continue;
		}
		structured_input[rule_count][count] = grammar_array[row][i];
		count ++;
	}
	
}

int validateInput(int count)
{
// Validate whether the entered input is proper
	int isValidTerm;
	int isValidNonTerm;

   	isValidTerm = validateTerminals(count); 
	isValidNonTerm = validateNonTerminals(count);
	if (isValidTerm & isValidNonTerm)
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
	
	terminals = structured_input[0];

	for(i=0;i<count;i++)
	{
		for(j=0,k=0;j<count;j++)
		{
			//cout<<"terminal "<<terminals[i]<<" "<<"Input "<<structured_input[j][k]<<" "<<"index "<<"i "<<i <<"j "<<j<<"k "<<k<<endl;
			if (terminals[i] == structured_input[j][k])
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

	non_terminal = structured_input[1];
	
	for(i=0;i<count;i++)
	{
		for(j=0;j<count;j++)
		{
			for(k=0;;k++)
			{
				//cout<<"terminal "<<terminals[i]<<" "<<"Input "<<structured_input[j][k]<<" "<<"index "<<"i "<<i <<"j "<<j<<"k "<<k<<endl;
				if (structured_input[j][k] == '\0')
					break;
				if (non_terminal[i] == structured_input[j][k])
				{
					non_term = 1;
					break;
				}
			}
		}
		if (non_term ==0)
			return 0;
	}
	return 1;
}

//Input getting part need to be revisited for sure. 
