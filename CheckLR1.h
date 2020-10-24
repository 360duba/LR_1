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

//			�ķ�����  �ķ����� ��ǰ������ ����λ��
typedef tuple<string, string, SymbolSet, int> ProjectSentence;

//struct ProjectSentence {
//	
//	string GrammarVn = "";			//�ķ�����
//	string GrammarSentence = "";	//�ķ�����
//	SymbolSet SearchSymbol;			//��ǰ������
//	//string totalstring = "";		//��������FIRST����ÿ�����ɢ����չ��
//	int index = 0;					//����λ��
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
	HWND				Output_Bind_RichTextDialogclass;						//����б����
	HWND				FirstSet_Bind_RichTextDialogclass;						//First���б����
	HWND				FollowSet_Bind_RichTextDialogclass;						//Follow���б����
	HWND				StatusSheet_Bind_RichTextDialogclass;					//״̬ת��ͼ�б����
	RichTextDialogclass Input_Bind_RichTextDialogclass;							//�����ı����������

	SymbolSet Vnset;															//���ս������
	std::map<char, set<char>> FIRSTset;											//FIRST������
	std::multimap<std::string, std::string> GrammarFormula;
	char mostcharacter;															//��߷��ս��
	
	std::map<statusblock, std::map<char, statusblock>> GOset;

	std::map<int,statusblock> AnalysedDFA;										//״̬����DFA��������λ��Ϊ��״̬���

	map<int, map<char, string>> ACTION;							//ACTION��
	map<int, map<char, string>> GOTO;								//GOTO��

	set<char> SearchSymbolsets;													//ACTION����������
	set<char> VnSymbolSets;														//GOTO����������

	map<statusblock, int> StatusblocksOrder;							//״̬���б�
	map<int, statusblock> ReservedStatusblocksOrder;							//�����ŵ�״̬���б�
	
	int totalitem = 1;

	TCHAR				output[MAXLEN] = { 0 };
};

#endif