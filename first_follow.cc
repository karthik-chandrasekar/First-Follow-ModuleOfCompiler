#include <iostream>
#include <string>

using namespace std; 

int getInput();
void cleanInput(int);
void copyToken(int, int, int, int);

string grammar_array[100];
string structured_input[100];

int main () 
{
	int count;
	count = getInput();
	cleanInput(count); 
	return 0;
} 

int getInput()
{
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


void cleanInput(int count)
{
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
				if(grammar_array[i][j+1]=='#') //If there are two successive #'s it is end of grammar
				{
					return;
				}
				start_col = end_col;
			}
		
		}
	}
}


void copyToken(int row, int start_col, int end_col, int rule_count)
{
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

