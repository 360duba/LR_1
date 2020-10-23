#include<iostream>
#include<map>
#include<set>
#include"CheckLR1.h"


using namespace std;

CheckLR1class CheckLR1classcore;

int main(int argc, char* argv[]) {
	CheckLR1classcore.makeAnalysedSheet("E->E+T\nE->T\nT->T*F\nT->F\nF->(E)\nF->i");
	//CheckLR1classcore.makeAnalysedSheet("S->BB\nB->aB\nB->b");
	cout << argc << " " << endl;
	//for (int i = 1; i < argc; i++) {
	//	
	//}
	
	if (strcmp(argv[1], "AnalysedSheet")) {	//argv2 为初始文法 返回值为状态转换表
		
		//CheckLR1classcore.makeAnalysedSheet(string{ argv[2] });
	}
	else if (strcmp(argv[1], "DoCheck")) {	//argv2 为初始文法 argv3 为判断句型

	}

}