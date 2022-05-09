#include<iostream>
#include<fstream>
#include<cctype>
#include<vector>
#include<string.h>
#include<algorithm>
using namespace std;

class rootnode                                                                      //parent class
{
public:
    char wordata;
    bool wordend;
    virtual void setWordTerminate()=0;                                               //pure virtual function

    char Data()
    {
        return wordata;
    }
    char Data(char c)                                                                   //function overloading
    {
        wordata = c;
        return wordata;
    }

    bool WordTerminate()
    {
        return wordend;
    }

    friend class Trie;                                                                  // declaring class trie as friend class

};

class node: public rootnode                                                            //subclass
{
public:
    node()
    {
        wordata= ' ';
        wordend = false;
    }
    ~node() {}
    node* findingChild(char c);
    void appendChild(node* child)
    {
        wChildren.push_back(child);
    }
    void setWordTerminate()
    {
        wordend = true;
    }
    vector<node*> children()
    {
        return wChildren;
    }
private:
    vector<node*> wChildren;
};

node* node::findingChild(char c)
{
    for ( int ii = 0; ii < wChildren.size(); ii++ )
    {
        node* temp1 = wChildren.at(ii);                                             //returns the character at the specified position
        if ( temp1->Data() == c )
        {
            return temp1;
        }
    }
    return NULL;
}

class Trie
{
public:
    Trie();
    ~Trie() {}

    void addWord(string s);
    bool Complete(string s,vector<string>&res);
    bool searchWord(string);
    void parseTree(node *current,char * s,vector<string>&,bool &loop);

    bool deleteWord(string s);

private:
    node* root;



};
Trie::Trie()
{
    root = new node();
}

void Trie::addWord(string s)
{
    node* current = root;
    if ( s.length() == 0 )
    {
        current->setWordTerminate();                                                            // an empty word
        return;
    }
    for ( int ii = 0; ii < s.length(); ii++ )
    {
        node* child = current->findingChild(s[ii]);
        if ( child != NULL )
        {
            current = child;
        }
        else
        {
            node* tmp = new node();
            tmp->Data(s[ii]);
            current->appendChild(tmp);
            current = tmp;
        }
        if ( ii == s.length() - 1 )
            current->setWordTerminate();
    }
}

bool Trie::searchWord(string s)
{
    node* current = root;
    while ( current != NULL )
    {
        for ( int i = 0; i < s.length(); i++ )
        {
            node* temp1 = current->findingChild(s[i]);
            if ( temp1 == NULL )
                return false;
            current = temp1;
        }
        if ( current->WordTerminate() )
            return true;
        else
            return false;
    }
    return false;
}

bool Trie::Complete(string s, vector<string> &res)
{
    node* current=root;
    for ( int ii = 0; ii < s.length(); ii++ )
    {
        node* tmp = current->findingChild(s[ii]);
        if ( tmp == NULL )
            return false;
        current = tmp;
    }
    char c[100];
    strcpy(c,s.c_str());
    bool loop=true;
    parseTree(current,c,res,loop);
    return true;
}

void Trie::parseTree(node *current, char *s,vector<string> &res,bool& loop)
{
    char k[100]= {0};
    char aa[2]= {0};
    if(loop)
    {
        if(current!=NULL)
        {
            if(current->WordTerminate()==true)
            {
                res.push_back(s);
                if(res.size()>200)
                    loop=false;
            }
            vector<node *> child=current->children();
            for(int jj=0; jj<child.size() && loop; jj++)
            {
                strcpy(k,s);
                aa[0]=child[jj]->Data();
                aa[1]='\0';
                strcat(k,aa);
                if(loop)
                    parseTree(child[jj],k,res,loop);
            }
        }
    }
}

bool loadDictionary(Trie* trie,string fn)
{
    ifstream words;
    ifstream input;
    words.open(fn.c_str());
    if(!words.is_open())
    {
        cout<<"Dictionary File can't be open"<<endl;
        return false;
    }
    while(!words.eof())
    {
        char s[100];
        words >> s;
        trie->addWord(s);
    }
    words.close();
    input.close();
    return true;
}

void WriteNewWord(Trie *trie)
{
	cout<<"Enter a Word: ";
	string NewWord;
	cin>>NewWord;
	bool OnlyAlpha=true;
	for(int i=0;i<NewWord.length();i++)
	{
		if(!isalpha(NewWord[i]))
			{
				OnlyAlpha=false;
				break;
			}
	}
	if(OnlyAlpha)
	{
		transform(NewWord.begin(), NewWord.end(), NewWord.begin(), ::tolower);
		vector<string> ListOfWords;
    	trie->Complete(NewWord,ListOfWords);
    	if(ListOfWords.size()!=0)
    	{
    		cout<<"The word '"<<NewWord<<"' already exists in the dictionary.\n";
    		return;
		}
		else
		{
			ofstream out;
			out.open("wordlist.txt",ios::app);
			if(!out.is_open())
			{
				cout<<"Sorry!\nCould not open the dictionary!\n";
				out.close();
				return;
			}
			else
			{
				out<<NewWord<<"\n";
				cout<<"Successfully loaded in the dictionary!\n";
				out.close();
				trie->addWord(NewWord);
				return;
			}
		}
	}
	else
	{
		cout<<"\Word entered by you is not valid!!!\n Enter a valid word.\n";
		return;
	}
}

class spellchecker
{
public:
char lower_alpha[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

int MissingCharacter (string wInput)                    //when a single character is missing from the word
{
    string Xinput, line, Tinput, Tline, Xline;
    int found = 0;
    ifstream words ("words.txt");
    if (words)
    {
        while (getline(words,line))
        {
            for (int i = 0; i < 26; i++)
            {
                int len = wInput.size(), flen = line.size();
                Xinput = wInput;
                Xline = line;
                Xinput.resize(len+1,'a');
                Xinput[len] = lower_alpha[i];
                len = Xinput.size();
                string Ninput, Nline;
                if (len == flen)
                {
                    for (int j = 1; j <=len; j++)
                        Ninput.push_back(Xinput[j]);
                    for (int j = 1; j <= flen; j++)
                        Nline.push_back(Xline[j]);
                    Xinput.resize(1);
                    Xline.resize(1);
                    sort(Nline.begin(),Nline.end());
                    sort(Ninput.begin(),Ninput.end());
                    Tinput = Xinput + Ninput;
                    Tline = Xline + Nline;
                    if (Tinput == Tline)
                    {
                        found = 1;
                        cout<<line<<endl;
                        break;
                    }
                }
                if (found == 1) break;
            }
            if (found == 1) break;
        }
        words.close();
    }
    else
    {
        cout<<"\nAn error occurred......\n";
    }
    return found;
}

int ExtraCharacter (string wInput)                                              //when some extra letter is added by th user by mistale
{
    string Xinput, line, Ninput, Tinput;
    int found = 0;
    ifstream words ("words.txt");
    if (words)
    {
        while (getline(words,line))
        {
             int len = wInput.size(), flen = line.size();
             if ((len-1) == flen)
             {
                 for (int i = 1; i < len; i++)
                 {
                     Xinput = wInput;
                     Xinput.erase(Xinput.begin()+i);
                     if (Xinput == line)
                     {
                         found = 1;
                         cout<<line<<endl;
                         break;
                     }

                 }
             }
        }
        words.close();
    }
    else
    {
        cout<<"\n An error occurred........\n";
    }
    return found;
}

int mixtureExtraMissing (string wInput)                                 //mixture of above two i.e. when one letter is missing and extra letter is also added
{
    string Xinput, line, Xline;
    int found = 0;
    ifstream words ("words.txt");
    if (words)
    {
        while (getline(words,line))
        {
            int len = wInput.size(), flen = line.size();
            if (len == flen)
            {
                for (int i = 1; i < len; i++)
                {
                    for (int j = 0; j < 26; j++)
                    {
                        Xinput = wInput; Xline = line;
                        Xinput.erase(Xinput.begin()+i);
                        Xinput.resize(len, 'a');
                        Xinput[len-1] = lower_alpha[j];
                        sort(Xinput.begin()+1,Xinput.end());
                        sort(Xline.begin()+1,Xline.end());
                        if (Xinput == Xline)
                        {
                            found = 1;
                            cout<<line<<endl;
                            break;
                        }
                    }
                    if (found == 1) break;
                }
                if (found == 1) break;
            }
        }
        words.close();
    }
    return found;
}

int ExchangeCharacter (string wInput)                               //when in a word some letter is replaced by any other letter
{
    string line, Xinput;
    int found = 0;
    ifstream words ("words.txt");
    if (words)
    {
        while (getline(words,line))
        {
            int len = wInput.size(), flen = line.size();
            if (len == flen)
            {
                for (int i = len-1; i >= 0 ; i--)
                {
                    Xinput = wInput;
                    Xinput[i] = lower_alpha[0];
                    for (int j=0; j<26; j++)
                    {
                        if (Xinput == line)
                        {
                            found = 1;
                            cout<<line<<endl;
                            break;
                        }
                        Xinput[i] = lower_alpha[j];
                    }
                    if (found == 1 ) break;
                    else continue;
                }
            }
        }
        words.close();
    }
    else cout<<"\n An error occurred......"<<endl;
    return found;
}


int IncorrectArrangement(string wInput)                                                 //when letters are incorrectly arranged in a word means they are not their right place
{
    string line;
    int found = 0;
    ifstream words ("words.txt");
    if (words)
    {
        while(getline(words,line))
        {
            string Xinput = wInput, Ninput, permutations, Tinput, Tline, Nline, Xline = line;
            int len = Xinput.size(), flen = line.size();
            if (len == flen)
            {
                for (int i=1; i<Xinput.length(); i++)
                    Ninput.push_back(wInput[i]);
                for (int i = 1; i < flen; i++)
                    Nline.push_back(Xline[i]);
                Xinput.resize(1);
                Xline.resize(1);
                sort(Nline.begin(),Nline.end());
                sort(Ninput.begin(), Ninput.end());
                Tinput = Xinput + Ninput;
                Tline = Xline + Nline;
                if (Tinput == Tline)
                {
                    found = 1;
                    cout<<line<<endl;
                    break;
                }
            }
        }
        words.close();
    }
    else
    {
        cout<<endl<<"An error occurred...."<<endl;
    }
    return found;
}

};

class spelling :public spellchecker                                                                             //single level inheritance
{
    public:
    void spell()
    {

        spellchecker obj2;


        string wInput,line;
        int len,flen,correct=0;
        cout<<"Enter the word: ";
        cin>>wInput;
        len = wInput.length();
        for (int i=0; i < len; i++)
            wInput[i] = tolower(wInput[i]);
        ifstream words ("words.txt");
        if(words)
        {
            while (getline(words,line))
            {
                flen = line.length();
                if (len==flen)
                {
                    if (line==wInput)
                    {
                        correct=1;
                    }
                    else continue;
                }
                else continue;
            }
            words.close();
            if (correct==1)
            {
                cout<<endl<<"Spelling is correct"<<endl;
            }
            if (correct==0)
            {
                int missing = 0, extra = 0, mixed = 0, incorrect = 0, exchanged = 0;
                cout<<endl<<"Spelling is wrong. Possible right spellings are given below:- "<<endl<<endl;
                missing = obj2.MissingCharacter(wInput);
                extra = obj2.ExtraCharacter(wInput);
                mixed = obj2.mixtureExtraMissing(wInput);
                incorrect = obj2.IncorrectArrangement(wInput);
                exchanged = obj2.ExchangeCharacter(wInput);
                if (missing == 0 && extra == 0 && mixed == 0 && incorrect == 0 && exchanged == 0)
                {
                    cout<<endl<<"No such word exist"<<endl;
                }
            }
        }
        else
        {
            cout<<"Not able to open words.txt"<<endl;
        }
        cout<<endl<<"Press any key to continue..."<<endl<<endl;

     }
};


int main()
{
    spelling obj;
    Trie obj3;

	Trie* trie = new Trie(); //Where new is used to allocate memory for a C++ class object, the object's constructor is called after the memory is allocated

    char mode;
    cout<<"Dictionary File is Loading...."<<endl;
    loadDictionary(trie,"words.txt");
    while(1)
    {
        cout<<endl<<endl;
        cout<<"Interactive mode,press "<<endl;
        cout<<"1: Auto Complete Feature"<<endl;
        cout<<"2: Search a Word"<<endl;
        cout<<"3: Enter new words into your personal file\n";
        cout<<"4: Spell Checker"<<endl;
        cout<<"5: Delete your file"<<endl;
        cout<<"6: Quit the program"<<endl;
        cin>>mode;
        if(isalpha(mode))
        {
        	cout<<"Invalid Input!\n";
        	cout<<"Enter either 1,2,3,4,5 or 6...";
            continue;
        }
        switch(mode)
        {
        case '1':
        {
            string s;
            cout<<"Enter the partial word : ";
            cin>>s;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            vector<string> ListOfWords;
            trie->Complete(s,ListOfWords);
            if(ListOfWords.size()==0)
            {
                cout<<"Sorry!\nNo suggestions"<<endl;
                cout<<"Do you want to enter this word into the memory?(y/n) : ";
                char pp;
                cin>>pp;
                if(pp=='y' || pp=='Y')
                {
                	WriteNewWord(trie);
				}
            }
            else
            {
                cout<<"Auto complete reply :"<<endl;
                for(int i=0; i<ListOfWords.size(); i++)
                {
                    cout<<" \t   "<<ListOfWords[i]<<endl;
                }
            }
        }
        continue;

        case '2':
        	{string s;
            cout<<"Enter the word : ";
            cin>>s;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            vector<string> ListOfWords;
            trie->Complete(s,ListOfWords);
            if(ListOfWords.size()==0)
            {
                cout<<"Sorry!\nWord not found"<<endl;
                cout<<"Do you want to enter this word into the dictionary?(y/n) : ";
                char pp;
                cin>>pp;
                if(pp=='y' || pp=='Y')
                {
                	WriteNewWord(trie);
				}
            }
            else
                cout<<"\nWord is already present in the dictionary";
        	 continue;
        	}
        case '3':
            WriteNewWord(trie);
        	continue;
        case '4':
            obj.spell();
            continue;
        case '5':
            { if( remove( "wordlist.txt" ) != 0 )
                    perror( "not deleted" );
            else
                    puts( "File created by you successfully deleted" );
            continue;                                                                                              //here we are trying to delete the file...hence the word..
            }
        case '6':
            delete trie;
            return 0;
        default:
        	cout<<"Invalid input!";
        	cout<<"\nEnter either 1,2,3,4,5 or 6..";
            continue;
        }
    }
}