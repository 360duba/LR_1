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
	
	string GrammarVn = "";			//文法句左
	string GrammarSentence = "";	//文法句右
	SymbolSet SearchSymbol;			//向前搜索符
	//string totalstring = "";		//便于求其FIRST集，每遇求分散族则展开
	int index = 0;					//遍历位置

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
	SymbolSet Vnset;															//非终结符集合
	std::map<char, set<char>> FIRSTset;											//FIRST集集合
	std::multimap<std::string, std::string> GrammarFormula;
	char mostcharacter;															//最高非终结符
	

	std::map<int,statusblock> AnalysedDFA;										//状态分析DFA其中所在位置为其状态序号

	map<int, map<char, string>> ACTION;							//ACTION表
	map<int, map<char, string>> GOTO;								//GOTO表

	set<char> SearchSymbolsets;													//ACTION搜索符集合
	set<char> VnSymbolSets;														//GOTO搜索符集合

	map<statusblock, int> StatusblocksOrder;							//状态族列表
	map<int, statusblock> ReservedStatusblocksOrder;							//反序存放的状态族列表
	
};

#endif