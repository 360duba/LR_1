#pragma once
#include<iostream>
#include<map>
#include<set>
#include<vector>
#include<deque>
#include<stack>
#include <tuple>
#include "DAcommon.h"
using namespace std;

#ifndef CHECKLR1_H_
#define CHECKLR1_H_

#define NULLCHARACTER '$'

#define GrammarVn 0
#define GrammarSentence 1
#define SearchSymbol 2
#define Index 3

static int GLOBALProjectSentenceorder = 0;
static int GLOBALstatusblockorder = 0;

typedef set<char> SymbolSet;

//			文法句左  文法句右 向前搜索符 遍历位置
typedef tuple<string, string, SymbolSet, int> ProjectSentence;

//struct ProjectSentence {
//	
//	string GrammarVn = "";			//文法句左
//	string GrammarSentence = "";	//文法句右
//	SymbolSet SearchSymbol;			//向前搜索符
//	//string totalstring = "";		//便于求其FIRST集，每遇求分散族则展开
//	int index = 0;					//遍历位置
//
//	int ProjectSentenceorder = GLOBALProjectSentenceorder;
//	
//	ProjectSentence() {
//		GLOBALProjectSentenceorder++;
//	}
//
//	~ProjectSentence() {
//		GLOBALProjectSentenceorder--;
//	}
//
//	bool operator < (const ProjectSentence& p2) const
//	{
//		/*if (p1.ProjectSentenceorder<p2.ProjectSentenceorder) {
//			return true;
//		}
//		return false;*/
//		if (GrammarVn < p2.GrammarVn || GrammarSentence < p2.GrammarSentence || SearchSymbol < p2.SearchSymbol || index < p2.index) {
//			return true;
//		}
//		return false;
//	}
//
//	bool operator > (const ProjectSentence& p2) const
//	{
//		/*if (p1.ProjectSentenceorder > p2.ProjectSentenceorder) {
//			return true;
//		}
//		return false;*/
//		if (GrammarVn > p2.GrammarVn || GrammarSentence > p2.GrammarSentence || SearchSymbol > p2.SearchSymbol || index > p2.index) {
//			return false;
//		}
//		return true;
//	}
//
//	bool operator == (ProjectSentence b) const {
//		if (GrammarVn == b.GrammarVn && GrammarSentence == b.GrammarSentence && SearchSymbol == b.SearchSymbol && index == b.index) {
//			return true;
//		}
//		return false;
//	}
//	bool operator != (ProjectSentence b) const {
//		if (!(GrammarVn == b.GrammarVn && GrammarSentence == b.GrammarSentence && SearchSymbol == b.SearchSymbol && index == b.index)) {
//			return true;
//		}
//		return false;
//	}
//
//	void operator = (ProjectSentence b) {
//		GrammarVn = b.GrammarVn;
//		GrammarSentence = b.GrammarSentence;
//		SearchSymbol = b.SearchSymbol;
//		index = b.index;
//	}
//};





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
		//GLOBALstatusblockorder--;
	}

	bool operator < (const statusblock &rhs) const{
		//if (ProjectItem.empty() == true || rhs.ProjectItem.empty()==true) {
		//	return true;
		//}
		/*for (auto i = ProjectItem.begin(); i != ProjectItem.end(); i++) {
			bool iscontain = false;
			for (auto j = rhs.ProjectItem.begin(); j != rhs.ProjectItem.end(); j++) {
				if (i->GrammarSentence == j->GrammarSentence && i->GrammarVn == j->GrammarVn && i->index == j->index && i->SearchSymbol == j->SearchSymbol) {
					iscontain = true;
				}
			}
			if (iscontain == false) {
				return true;
			}
		}
		return false;*/
		if (ProjectItem < rhs.ProjectItem) {
			return true;
		}
		return false;
		/*if (statusblockorder < rhs.statusblockorder) {
			return true;
		}
		return false;*/
	}

	bool operator <= (const statusblock& rhs) const {
		if (ProjectItem <= rhs.ProjectItem) {
			return true;
		}
		return false;
		/*for (auto i = ProjectItem.begin(); i != ProjectItem.end(); i++) {
			bool iscontain = false;
			for (auto j = rhs.ProjectItem.begin(); j != rhs.ProjectItem.end(); j++) {
				if (i->GrammarSentence == j->GrammarSentence && i->GrammarVn == j->GrammarVn && i->index == j->index && i->SearchSymbol == j->SearchSymbol) {
					iscontain = true;
				}
			}
			if (iscontain == false) {
				return true;
			}
		}
		return false;*/
	}

	bool operator > (const statusblock& rhs) const {
		if (ProjectItem > rhs.ProjectItem) {
			return true;
		}
		return false;
		/*for (auto i = ProjectItem.begin(); i != ProjectItem.end(); i++) {
			bool iscontain = false;
			for (auto j = rhs.ProjectItem.begin(); j != rhs.ProjectItem.end(); j++) {
				if (i->GrammarSentence == j->GrammarSentence && i->GrammarVn == j->GrammarVn && i->index == j->index && i->SearchSymbol == j->SearchSymbol) {
					iscontain = true;
				}
			}
			if (iscontain == false) {
				return false;
			}
		}
		return true;*/
	}

	bool operator >= (const statusblock& rhs) const {
		if (ProjectItem >= rhs.ProjectItem) {
			return true;
		}
		return false;
		/*for (auto i = ProjectItem.begin(); i != ProjectItem.end(); i++) {
			bool iscontain = false;
			for (auto j = rhs.ProjectItem.begin(); j != rhs.ProjectItem.end(); j++) {
				if (i->GrammarSentence == j->GrammarSentence && i->GrammarVn == j->GrammarVn && i->index == j->index && i->SearchSymbol == j->SearchSymbol) {
					iscontain = true;
				}
			}
			if (iscontain == false) {
				return true;
			}
		}
		return false;*/
	}

	bool operator==(const statusblock &rhs) const {
		if (ProjectItem == rhs.ProjectItem) {
			return true;
		}
		return false;
		/*for (auto i = ProjectItem.begin(); i != ProjectItem.end(); i++) {
			bool iscontain = false;
			for (auto j = rhs.ProjectItem.begin(); j != rhs.ProjectItem.end(); j++) {
				if (i->GrammarSentence == j->GrammarSentence && i->GrammarVn == j ->GrammarVn && i->index == j ->index && i->SearchSymbol == j->SearchSymbol) {
					iscontain = true;
				}
			}
			if (iscontain == false) {
				return false;
			}
		}
		return true;*/
	}


	bool operator != (const statusblock& rhs) const {
		if (ProjectItem != rhs.ProjectItem) {
			return true;
		}
		return false;
		/*for (auto i = ProjectItem.begin(); i != ProjectItem.end(); i++) {
			bool iscontain = false;
			for (auto j = rhs.ProjectItem.begin(); j != rhs.ProjectItem.end(); j++) {
				if (i->GrammarSentence == j->GrammarSentence && i->GrammarVn == j->GrammarVn && i->index == j->index && i->SearchSymbol == j->SearchSymbol) {
					iscontain = true;
				}
			}
			if (iscontain == false) {
				return true;
			}
		}
		return false;*/
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

	SymbolSet searchFIRSTsets(char input, SymbolSet lastsearchsymbol);

	SymbolSet getSymbols(statusblock input);
	void getFIRSTsets();
	void makeAnalysedSheet(string input);
	void outputstatussheet();
	void outputaline(deque<int>& statusstack, deque<char>& symbolstack, string& reststring, string instruction);
	void makeAnalysedSheet();

	void DoCheck();
	void outputGrammarFormula();
	void BindInputHWND(HWND Input_RichTextDialogclass);
	void BindoutputFirstSetHWND(HWND Output_RichTextDialogclass);
	void BindoutputFollowSetHWND(HWND Output_RichTextDialogclass);
	void BindoutputStatusSheetHWND(HWND Output_RichTextDialogclass);
	void BindOutputHWND(HWND Output_RichTextDialogclass);
	void collapsesameFormula(statusblock& input);
	void outputGroupset();

	TCHAR* toTCHAR(std::string input);
	TCHAR* toTCHAR(int input);

	statusblock getCLOSURE(statusblock input);
	statusblock GO(statusblock input, char symbol);

	int isfind(map<int,statusblock>& inputvector, statusblock& input);
private:
	HWND				Output_Bind_RichTextDialogclass;						//输出列表框句柄
	HWND				FirstSet_Bind_RichTextDialogclass;						//First集列表框句柄
	HWND				FollowSet_Bind_RichTextDialogclass;						//Follow集列表框句柄
	HWND				StatusSheet_Bind_RichTextDialogclass;					//状态转换图列表框句柄
	RichTextDialogclass Input_Bind_RichTextDialogclass;							//输入文本框类的引用

	SymbolSet Vnset;															//非终结符集合
	std::map<char, set<char>> FIRSTset;											//FIRST集集合
	std::multimap<std::string, std::string> GrammarFormula;
	char mostcharacter;															//最高非终结符
	
	std::map<statusblock, std::map<char, statusblock>> GOset;

	std::map<int,statusblock> AnalysedDFA;										//状态分析DFA其中所在位置为其状态序号

	map<int, map<char, string>> ACTION;							//ACTION表
	map<int, map<char, string>> GOTO;								//GOTO表

	set<char> SearchSymbolsets;													//ACTION搜索符集合
	set<char> VnSymbolSets;														//GOTO搜索符集合

	map<statusblock, int> StatusblocksOrder;							//状态族列表
	map<int, statusblock> ReservedStatusblocksOrder;							//反序存放的状态族列表
	
	int totalitem = 1;

	TCHAR				output[MAXLEN] = { 0 };
};

#endif