#include <bits/stdc++.h>
#define ASCII_RANGE 256
using namespace std;
class Node
{
	public:
	unsigned char data;
	int weight;
	Node* LeftChild;
	Node* RightChild;
	Node* Parent;
	Node()
	{
		data=(unsigned char)0;
		LeftChild=NULL;
		RightChild=NULL;
		Parent=NULL;
		weight=-1;
	}
};
class Extractor
{
	public:
	string code;
	Extractor()
	{
		buffer=(unsigned char)0;
		len=0;
		code="";
	}
	void Push(int ch)
	{
		buffer=(buffer<<1)|ch;
		len++;
		if (len==8)
		Write();
	}
	void EndOfInput()
	{
		code+=buffer;
		buffer=(unsigned char)len;
		code+=buffer;
	}
	private:
	unsigned char buffer;
	int len;
	void Write()
	{
		code+=buffer;
		buffer=(unsigned char)0;
		len=0;
	}
};
class Tree
{
	private:
	Node* Twig;
	Node* head;
	void Select(int counter)
	{
		
		int tag=-1;
		for (int i=0;i<counter;i++)
		{
			if (Twig[i].Parent==NULL)
			{
				tag=Twig[i].weight;
				break;
			}
		}
		for (int i=0;i<counter;i++)
		{
			if (Twig[i].Parent==NULL&&tag>Twig[i].weight)
			tag=Twig[i].weight;
		}
		for (int i=0;i<counter;i++)
		{
			if (tag==Twig[i].weight&&Twig[i].Parent==NULL)
			{
				Twig[i].Parent=&Twig[counter];
				Twig[counter].LeftChild=&Twig[i];
				Twig[counter].weight=Twig[i].weight;
				break;
			}
		}
		tag=-1;
		for (int i=0;i<counter;i++)
		{
			if (Twig[i].Parent==NULL)
			{
				tag=Twig[i].weight;
				break;
			}
		}
		for (int i=0;i<counter;i++)
		{
			if (Twig[i].Parent==NULL&&tag>Twig[i].weight)
			tag=Twig[i].weight;
		}
		for (int i=0;i<counter;i++)
		{
			if (tag==Twig[i].weight&&Twig[i].Parent==NULL)
			{
				Twig[i].Parent=&Twig[counter];
				Twig[counter].RightChild=&Twig[i];
				Twig[counter].weight+=Twig[i].weight;
				break;
			}
		}
	}
	void init()
	{
		if (Twig!=NULL)
		{
			delete []Twig;
			Twig=NULL;
			Twig=new Node[2*ASCII_RANGE-1];
		}
	}
	public:
	Tree()
	{
		Twig=new Node[2*ASCII_RANGE-1];
		head=NULL;
	}
	~Tree()
	{
		if (Twig!=NULL)
		{
			delete []Twig;
			Twig=NULL;
		}
	}
	void Make(int weight[])
	{
		init();
		for (int i=0;i<ASCII_RANGE;i++)
		{
			Twig[i].data=(unsigned char)i;
			Twig[i].weight=weight[i];
		}
		for (int i=ASCII_RANGE;i<2*ASCII_RANGE-1;i++)
		{
			Select(i);
		}
		for (int i=ASCII_RANGE;i<2*ASCII_RANGE-1;i++)
		{
			if (Twig[i].Parent==NULL)
			{
				head=&Twig[i];
				break;
			}
		}
	}
	void Make(string word)
	{
		int weight[ASCII_RANGE];
		for (int i=0;i<ASCII_RANGE;i++)
		weight[i]=0;
		for (int i=0;i<word.length();i++)
		weight[(unsigned char)word[i]]++;
		Make(weight);
	}
	string EnCode(string word)
	{
		Extractor ext;
		for (int i=0;i<word.length();i++)
		{
			Node* tmp=&Twig[(unsigned char)word[i]];
			Node* cur=tmp->Parent;
			while (true)
			{
				if (cur==NULL)
				break;
				if (cur->LeftChild==tmp)
				{
					ext.Push(1);
				}
				else
				{
					ext.Push(0);
				}
				tmp=tmp->Parent;
				cur=tmp->Parent;
			}
		}
		ext.EndOfInput();
		return ext.code;
	}
	string DeCode(string code)
	{
		string word;
		unsigned char tmp;
		Node* cur=head;
		int len=code[code.length()-1];
		if (len!=0)
		{
			tmp=code[code.length()-2];
			for (int j=0;j<len;j++,tmp=tmp>>1)
			{ 
				if (tmp%2==1)
				cur=cur->LeftChild;
				else
				cur=cur->RightChild;
				if (cur->LeftChild==NULL&&cur->RightChild==NULL)
				{
					word+=cur->data;
					cur=head;
				}
			}
		}
		for (int i=code.length()-3;i>=0;i--)
		{
			tmp=code[i];
			for (int j=0;j<8;j++,tmp=tmp>>1)
			{ 
				if (tmp%2==1)
				cur=cur->LeftChild;
				else
				cur=cur->RightChild;
				if (cur->LeftChild==NULL&&cur->RightChild==NULL)
				{
					word+=cur->data;
					cur=head;
				}
			}
		}
		string p="";
		for (int i=word.length()-1;i>=0;i--)
		p+=word[i];
		return p;
	}
};
class FileEditor
{
	private:
	Tree Ext;
	ifstream InFile;
	ofstream OutFile;
	string word;
	string code;
	string password;
	int* Weight;
	bool flag;
	bool pass;
	bool EndWithTXT(string name)
	{
		return (name.find(".txt")==name.length()-4&&name.length()>4);
	}
	bool EndWithHAF(string name)
	{
		return (name.find(".haf")==name.length()-4&&name.length()>4);
	}
	public:
	FileEditor()
	{
		Weight=new int[ASCII_RANGE];
	}
	bool IsGoodFile()
	{
		return InFile.is_open()&&OutFile.is_open();
	}
	void GetFileName(string InFileName,string OutFileName)
	{
		if (EndWithTXT(InFileName)&&EndWithHAF(OutFileName))
		{
			flag=true;
			InFile.open(InFileName.data(),ios::in|ios::binary);
			OutFile.open(OutFileName.data(),ios::out|ios::binary);
		}
		else if (EndWithHAF(InFileName)&&EndWithTXT(OutFileName))
		{
			flag=false;
			InFile.open(InFileName.data(),ios::in|ios::binary);
			OutFile.open(OutFileName.data(),ios::out|ios::trunc|ios::binary);
		}
		else
		{
			InFile.close();
			OutFile.close();
		}
		if (InFile.peek()==EOF)
		{
			InFile.close();
			OutFile.close();
		}
	}
	~FileEditor()
	{
		delete []Weight;
		if (InFile.is_open())
		InFile.close();
		if (OutFile.is_open())
		OutFile.close();
	}
	void ReadFromInFile()
	{
		InFile.seekg(0,ios::beg);
		if (flag)
		{
			string tmp="";
			while (InFile.peek()!=EOF)
			{
				getline(InFile,tmp);
				if (InFile.peek()!=EOF)
				tmp+='\n';
				word+=tmp;
			}
		}
		else
		{
			for (int i=0;i<ASCII_RANGE;i++)
			InFile>>Weight[i];
			InFile>>pass;
			string tmp="";
			getline(InFile,tmp);
			if (pass)
			getline(InFile,password);
			else
			getline(InFile,tmp);
			while (InFile.peek()!=EOF)
			{
				getline(InFile,tmp);
				if (InFile.peek()!=EOF)
				tmp+='\n';
				code+=tmp;
			}
		}
	}
	bool NeedPassword()
	{
		return !flag&&pass;
	}
	bool GetFlag()
	{
		return flag;
	}
	void SetPass(bool r)
	{
		if (!flag)
		return;
		pass=r;
	}
	void MakePassword(string word)
	{
		if (flag&&pass)
		password=Ext.EnCode(word);
	}
	bool IsRightPassword(string word)
	{
		Ext.DeCode(password);
		return NeedPassword()&&Ext.EnCode(word)==Ext.DeCode(password);
	}
	void GenerateCode()
	{
		//if (!IsRightPassword())
		if (flag)
		{
			for (int i=0;i<ASCII_RANGE;i++)
			Weight[i]=0;
			for (int i=0;i<word.length();i++)
			Weight[(unsigned char)word[i]]++;
			Ext.Make(word);
			code=Ext.EnCode(word);
		}
		else
		{
			Ext.Make(Weight);
			word=Ext.DeCode(code);
		}
	}
	void WriteToOutputFile()
	{
		if (flag)
		{
			for (int i=0;i<ASCII_RANGE;i++)
			OutFile<<Weight[i]<<" ";
			OutFile<<pass<<endl;
			if (pass)
			OutFile<<Ext.EnCode(password);
			OutFile<<endl;
			OutFile<<code;
		}
		else
		OutFile<<word;
	}
};
