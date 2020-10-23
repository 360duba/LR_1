#pragma once
#include<iostream>
#include<map>
#include<set>
#include<vector>
#include<deque>
#include<stack>

using namespace std;

#ifndef CHECKLR1_H_
#define CHECKLR1_H_

#define NULLCHARACTER '$'

static int GLOBALProjectSentenceorder = 0;
static int GLOBALstatusblockorder = 0;

typedef set<char> SymbolSet;

struct ProjectSentence {
	
	string GrammarVn = "";			//�ķ�����
	string GrammarSentence = "";	//�ķ�����
	SymbolSet SearchSymbol;			//��ǰ������
	//string totalstring = "";		//��������FIRST����ÿ�����ɢ����չ��
	int index = 0;					//����λ��

	int ProjectSentenceorder = GLOBALProjectSentenceorder;
	
	ProjectSentence() {
		GLOBALProjectSentenceorder++;
	}

	~ProjectSentence() {
		GLOBALProjectSentenceorder--;
	}

	friend bool operator < (const ProjectSentence& p1, const ProjectSentence& p2)
	{
		if (p1.ProjectSentenceorder<p2.ProjectSentenceorder) {
			return true;
		}
		return false;
	}

	friend bool operator > (const ProjectSentence& p1, const ProjectSentence& p2)
	{
		if (p1.ProjectSentenceorder > p2.ProjectSentenceorder) {
			return true;
		}
		return false;
	}

	bool operator == (ProjectSentence b) const {
		if (GrammarVn == b.GrammarVn && GrammarSentence == b.GrammarSentence && SearchSymbol == b.SearchSymbol && index == b.index) {
			return true;
		}
		return false;
	}
	bool operator != (ProjectSentence b) const {
		if (!(GrammarVn == b.GrammarVn && GrammarSentence == b.GrammarSentence && SearchSymbol == b.SearchSymbol && index == b.index)) {
			return true;
		}
		return false;
	}

	void operator = (ProjectSentence b) {
		GrammarVn = b.GrammarVn;
		GrammarSentence = b.GrammarSentence;
		SearchSymbol = b.SearchSymbol;
		index = b.index;
	}
};





struct statusblock {
	
	set<ProjectSentence> ProjectItem;
	bool isCorrect = true;
	//set<int> childstatusblockorder;
	int statusorder;
	int statusblockorder = GLOBALstatusblockorder;
	statusblock() {
		GLOBALstatusblockorder++;
	}
	~statusblock() {
		GLOBALstatusblockorder--;
	}

	bool operator < (statusblock b) const{
		if (statusblockorder<b.statusblockorder) {
			return true;
		}
		return false;
	}

	bool operator <= (statusblock b) const {
		if (statusblockorder <= b.statusblockorder) {
			return true;
		}
		return false;
	}

	bool operator > (statusblock b) const {
		if (statusblockorder > b.statusblockorder) {
			return true;
		}
		return false;
	}

	bool operator >= (statusblock b) const {
		if (statusblockorder >= b.statusblockorder) {
			return true;
		}
		return false;
	}

	bool operator == (statusblock b) const {

		if (ProjectItem == b.ProjectItem && isCorrect == b.isCorrect) {
			return true;
		}
		return false;
	}

	bool operator != (statusblock b) const {
		if (!(ProjectItem == b.ProjectItem && isCorrect == b.isCorrect)) {
			return true;
		}
		return false;
	}

	void operator +=(ProjectSentence b)  {
		ProjectItem.insert(b);
	}

	void operator += (statusblock b) {
		if (b.isCorrect == true && isCorrect == true) {
			ProjectItem.insert(b.ProjectItem.begin(), b.ProjectItem.end());
		}
	}

	void operator = (statusblock b) {
		if (b.isCorrect == true && isCorrect == true) {
			ProjectItem.clear();
			ProjectItem.insert(b.ProjectItem.begin(), b.ProjectItem.end());
		}
	}
};



class CheckLR1class {
public:
	CheckLR1class() {};
	/*void getFIRSTsets();*/
	//SymbolSet searchFIRSTsets(string input);
	SymbolSet searchFIRSTsets(char input, SymbolSet lastsearchsymbol);

	SymbolSet getSymbols(statusblock input);
	void getFIRSTsets();
	void makeAnalysedSheet(string input);
	void createAnalysedSheet();
	//void getStatusSheet();
	void makeAnalysedSheet();
	//statusblock getCLOSURE(ProjectSentence input);
	statusblock getCLOSURE(statusblock input);
	statusblock GO(statusblock input, char symbol);
	/*bool statusblockcpy(statusblock dis, statusblock src);
	bool statusblockcmp(statusblock dis, statusblock src);*/
	int isfind(map<int,statusblock>& inputvector, statusblock& input);
private:
	SymbolSet Vnset;															//���ս������
	std::map<char, set<char>> FIRSTset;											//FIRST������
	std::multimap<std::string, std::string> GrammarFormula;
	char mostcharacter;															//��߷��ս��
	

	std::map<int,statusblock> AnalysedDFA;										//״̬����DFA��������λ��Ϊ��״̬���

	map<int, map<char, string>> ACTION;							//ACTION��
	map<int, map<char, string>> GOTO;								//GOTO��

	set<char> SearchSymbolsets;													//ACTION����������
	set<char> VnSymbolSets;														//GOTO����������

	map<statusblock, int> StatusblocksOrder;							//״̬���б�
	map<int, statusblock> ReservedStatusblocksOrder;							//�����ŵ�״̬���б�
	
};

#endif