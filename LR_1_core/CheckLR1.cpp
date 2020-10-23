
#include"CheckLR1.h"




////寻找给定字符串的FIRST集
//SymbolSet CheckLR1class::searchFIRSTsets(string input) {
//	string tmpinput = input;
//	SymbolSet output;
//	output.clear();
//	int searchpointer = 0;
//	while (tmpinput.at(searchpointer) == NULLCHARACTER && searchpointer< tmpinput.length())
//		searchpointer++;
//
//	if (searchpointer == tmpinput.length()) {
//		output.insert('#');
//	}
//
//	if (Vnset.find(tmpinput.at(searchpointer)) != Vnset.end()) {//是非终结符
//		//auto tmp = FIRSTset[input.at(searchpointer)];
//		//output.insert(tmp.begin(),tmp.end());
//		deque<char> waitingcheckstring;
//		deque<char>::iterator pointer;
//		waitingcheckstring.push_back(tmpinput.at(searchpointer));
//		pointer = waitingcheckstring.begin();
//		while (pointer != waitingcheckstring.end()) {
//
//			for (auto i = GrammarFormula.begin(); i != GrammarFormula.end(); i++) {			//i->first.at(0)为待检测字符串
//				if (i->second.at(0) == *pointer) {
//					if (Vnset.find(i->first.at(0)) == Vnset.end()) {			//终结符
//						output.insert(i->first.at(0));
//					}
//					else {
//						bool isexist = false;
//						for (auto tmpiter = waitingcheckstring.begin(); tmpiter != waitingcheckstring.end(); tmpiter++) {		//防止重复递归
//							if (*tmpiter == i->first.at(0)) {
//								isexist = true;
//							}
//						}
//						if (isexist == false) {
//							waitingcheckstring.push_back(i->first.at(0));
//						}
//					}
//				}
//			}
//
//			pointer++;
//		}
//		
//	}
//	else {
//		output.insert(tmpinput.at(searchpointer));
//	}
//	return output;
//}


//利用检测FIRSTset变动情况来求FIRST集的函数，试图忽视左递归与回溯，并不产生新的非终结符
void CheckLR1class::getFIRSTsets() {
	multimap<string, string> CopyedGrammarFormula = GrammarFormula;		//备份的文法表达式列表

	while (true) {
		bool isFIRSTsetincreased = false;

		for (auto i = FIRSTset.begin(); i != FIRSTset.end() && isFIRSTsetincreased == false; i++) {		//删去first集中存在空字符且在产生式右部首位的字符
			if (i->second.find(NULLCHARACTER) != i->second.end()) {
				for (auto ii = CopyedGrammarFormula.begin(); ii != CopyedGrammarFormula.end(); ii++) {
					if (ii->first.at(0) == i->first) {

						auto oldii = ii;
						CopyedGrammarFormula.insert(make_pair(ii->first.substr(1),ii->second));
						ii++;
						CopyedGrammarFormula.erase(oldii);
						
						isFIRSTsetincreased = true;
						
					}
				}
			}
		}

		for (auto i = CopyedGrammarFormula.begin(); i != CopyedGrammarFormula.end(); i++) {
			//ReversedGrammarFormula.insert(i->second, i->first);
			if (Vnset.find(i->first.at(0)) == Vnset.end() && (FIRSTset.find(i->second.at(0))== FIRSTset.end() || FIRSTset[i->second.at(0)].find(i->first.at(0)) == FIRSTset[i->second.at(0)].end())) {	//终结符且未收录直接添加
				FIRSTset[i->second.at(0)].insert(i->first.at(0));
				isFIRSTsetincreased = true;
			}
			if (Vnset.find(i->first.at(0)) != Vnset.end() && FIRSTset[i->first.at(0)].size() != 0) {						//非终结符且该符号非终结符非空
				bool isallhasincluded = true;																					//若存在不包括则添加
				
				
					for (auto tmpj = FIRSTset[i->first.at(0)].begin(); tmpj != FIRSTset[i->first.at(0)].end(); tmpj++) {
						bool isthishasincluded = false;
						for (auto tmpi = FIRSTset[i->second.at(0)].begin(); tmpi != FIRSTset[i->second.at(0)].end(); tmpi++) {						
							if (*tmpi == *tmpj && *tmpj!=NULLCHARACTER) {
								isthishasincluded = true;
							}					
						}
						if (isthishasincluded == false) {
							isallhasincluded = false;
						}
					}
				
				if (isallhasincluded == false) {
					for (auto tmpj = FIRSTset[i->first.at(0)].begin(); tmpj != FIRSTset[i->first.at(0)].end(); tmpj++) {
						if (*tmpj != NULLCHARACTER) {
							FIRSTset[i->second.at(0)].insert(*tmpj);
							isFIRSTsetincreased = true;
						}					
					}
					
				}
			}

		}

		if (isFIRSTsetincreased == false) {
			break;
		}

	}
	
	

}

SymbolSet CheckLR1class::searchFIRSTsets(char input, SymbolSet lastsearchsymbol) {
	SymbolSet FIRSTsetofinput;
	deque<char> Vnneedtofetch;

	if (input != NULLCHARACTER) {	//获取input的first集
		if (Vnset.find(input) != Vnset.end()) {										//非终结符
			FIRSTsetofinput.insert(FIRSTset[input].begin(), FIRSTset[input].end());
			if (FIRSTset[input].find(NULLCHARACTER) == FIRSTset[input].end()) {	//如果没有找到终结符 直接返回其first集
				return FIRSTsetofinput;
			}
		}
		else {																		//终结符
			FIRSTsetofinput.insert(input);
			return FIRSTsetofinput;
		}
		
	}

	for (auto i = lastsearchsymbol.begin(); i != lastsearchsymbol.end(); i++) {		//需要获取后面的符号
		if (Vnset.find(*i) != Vnset.end())
			FIRSTsetofinput.insert(FIRSTset[*i].begin(), FIRSTset[*i].end());		//非终结符取FIRST集
		else
			FIRSTsetofinput.insert(*i);												//终结符取自身
	}
	if (FIRSTsetofinput.size() == 0)
		FIRSTsetofinput.insert('#');
	return FIRSTsetofinput;
}

////如果源状态族或目的状态族存在iscorrect为false 则不进行复制 返回false
//bool CheckLR1class::statusblockcpy(statusblock dis, statusblock src) {
//	if (dis.isCorrect == true && src.isCorrect == true) {
//		dis.ProjectItem.insert(src.ProjectItem.begin(), src.ProjectItem.end());
//		return true;
//	}
//	return false;
//}



//bool CheckLR1class::statusblockcmp(statusblock dis, statusblock src) {
//	if (dis.isCorrect != src.isCorrect) {
//		return false;
//	}
//	auto i = dis.ProjectItem.begin(), ii = src.ProjectItem.begin();
//	for (; i != dis.ProjectItem.end() && ii != src.ProjectItem.end(); i++, ii++) {
//		//if(!(i->GrammarSentence == ii->GrammarSentence && i->GrammarVn == ii->GrammarVn && i->index == ii->index && i->SearchSymbol == ii->SearchSymbol && i->totalstring == ii->totalstring)){
//		
//		return false;
//		}
//	}
//	if (i != dis.ProjectItem.end() || ii != src.ProjectItem.end()) {
//		return false;
//	}
//	return true;
//}

////获取状态转换表
//void CheckLR1class::getStatusSheet() {
//	deque<statusblock> potentialdeque;	//缓存需要展开进行输出的队列
//	statusblock initalstatusblock;
//	for (auto i = GrammarFormula.begin(); i != GrammarFormula.end(); i++) {
//		if (i->second.at(0) == mostcharacter) {
//			ProjectSentence tmp;
//			tmp.GrammarSentence = i->first;
//			tmp.GrammarVn = i->second;
//			tmp.totalstring = "#" + i->second + "#";
//			tmp.SearchSymbol = searchFIRSTsets(tmp.totalstring.substr(tmp.index + 2));
//			if (tmp.SearchSymbol.size()==0) {
//				tmp.SearchSymbol.insert('#');
//			}
//			initalstatusblock.ProjectItem[tmp.ProjectSentenceorder] = tmp;
//		}
//	}
//	AnalysedDFA[initalstatusblock.statusblockorder]=initalstatusblock;
//	potentialdeque.push_back(initalstatusblock);
//
//	while (potentialdeque.empty() == false) {
//		auto tmp = potentialdeque.front();
//		potentialdeque.pop_front();
//		auto tmpsymbols = getSymbols(tmp);
//		for (auto i = tmpsymbols.begin(); i != tmpsymbols.end(); i++) {
//			auto tmper = GO(tmp, *i);
//			int tmpthenum = isfind(AnalysedDFA, tmper);
//			if (tmpthenum == -1) {
//				AnalysedDFA[tmper.statusblockorder]=tmper;
//				potentialdeque.push_back(tmper);
//				tmp.childstatusblockorder.insert(potentialdeque.size()-1);
//			}
//			else {
//				tmp.childstatusblockorder.insert(tmpthenum);
//			}
//		}
//		
//	}
//
//}


int CheckLR1class::isfind(map<int,statusblock>& inputvector, statusblock&input) {
	int index = 0;
	for (auto i = inputvector.begin(); i != inputvector.end(); i++,index++) {
		if (i->second == input) {
			return index;
		}
	}
	return -1;
}

//获取当前状态所有可能的下一步转换符号
SymbolSet CheckLR1class::getSymbols(statusblock input) {
	SymbolSet output;
	output.clear();
	for (auto i = input.ProjectItem.begin(); i != input.ProjectItem.end(); i++) {
		auto tmp = *i;
		if (tmp.index < tmp.GrammarSentence.length()) {
			output.insert(tmp.GrammarSentence.at(tmp.index));

		}
		
	}
	return output;
}

std::map<statusblock, std::map<char, statusblock>> GOset;

statusblock CheckLR1class::GO(statusblock I, char X) {
	statusblock output;
	statusblock needtogetCLOSURE;
	for (auto i = I.ProjectItem.begin(); i != I.ProjectItem.end(); i++) {
		if (i->index == i->GrammarSentence.length()) {	//已到结尾，不须扩展
			/*auto tmpcopyed = *i;
			tmpcopyed.index;
			output += tmpcopyed;*/
			//auto oldi = i;
			//I.ProjectItem.insert(tmpcopyed);			
			//i++;
			//I.ProjectItem.erase(oldi);
		}
		else if (i->index<i->GrammarSentence.length() && i->GrammarSentence.at(i->index) == X) {	//需要拓展
			auto tmpcopyed = *i;
			tmpcopyed.index++;
			needtogetCLOSURE += tmpcopyed;
			output += tmpcopyed;
			
			//auto oldi = i;
			//I.ProjectItem.insert(tmpcopyed);		
			//i++;
			//I.ProjectItem.erase(oldi);
		}
	}

	output += getCLOSURE(needtogetCLOSURE);
	
	return output;
}

void createAnalysedSheet() {

}

void CheckLR1class::makeAnalysedSheet() {
	statusblock initalstatusblock;
	ProjectSentence tmp;
	tmp.GrammarSentence = string{ mostcharacter };
	tmp.GrammarVn = "0";
	tmp.index = 0;
	SymbolSet tmpsymbolset;
	tmpsymbolset.insert('#');
	tmp.SearchSymbol = tmpsymbolset;
	initalstatusblock.ProjectItem.insert(tmp);
	initalstatusblock += getCLOSURE(initalstatusblock);

	//set<set<ProjectSentence>> alreadyexpanedstatusblock;		//已经展开过的状态族集合的集合 由于statusblock结构中存在自增变量无法利用
	stack<statusblock> searchingstack;
	searchingstack.push(initalstatusblock);

	while (searchingstack.empty() == false) {
		statusblock currentstatusblock = searchingstack.top();
		//alreadyexpanedstatusblock.insert(currentstatusblock.ProjectItem);
		searchingstack.pop();
		SymbolSet symbol_iter = getSymbols(currentstatusblock);

		for (auto i = symbol_iter.begin(); i != symbol_iter.end(); i++) {
			statusblock tmpfetchedstatusblock = GO(currentstatusblock, *i);
			
			GOset[currentstatusblock][*i] = tmpfetchedstatusblock;
			//auto thei_iter = alreadyexpanedstatusblock.begin();
			//std::set<ProjectSentence>::iterator theii_iter = thei_iter->end() , theiii_iter= tmpfetchedstatusblock.ProjectItem.end();
			bool isfinded = false;

			//for (; thei_iter != alreadyexpanedstatusblock.end() && isfinded==true; thei_iter++) {
			//	theii_iter = thei_iter->begin(), theiii_iter = tmpfetchedstatusblock.ProjectItem.begin();
			//	for(;theii_iter!=thei_iter->end() && theiii_iter!=tmpfetchedstatusblock.ProjectItem.end();theii_iter++, theiii_iter++)
			//		if (!(theii_iter->GrammarSentence == theiii_iter->GrammarSentence && theii_iter->GrammarVn == theiii_iter->GrammarVn && theii_iter->index== theiii_iter->index && theii_iter->SearchSymbol== theiii_iter->SearchSymbol)){	//如果展开过 则文法中 没有不同的 且 都到了结尾 {
			//			isfinded = false;
			//			break;
			//		}
			//}//问题：：：
			for (auto GOset_iter = GOset.begin(); GOset_iter != GOset.end(); GOset_iter++) {
				if (GOset_iter->first == tmpfetchedstatusblock) {
					isfinded = true;
				}
			}
			

			if(isfinded==false /*|| theii_iter!= thei_iter->end() || theiii_iter != tmpfetchedstatusblock.ProjectItem.end()*/)		//尚未进行展开
				searchingstack.push(tmpfetchedstatusblock);
		}
	}
	



	//makeStatusblockssets();

	StatusblocksOrder.insert(make_pair(initalstatusblock, 0));			//零状态

	for (auto i = GOset.begin(); i != GOset.end(); i++) {
		bool isfind = false;
		for (auto tmpi = StatusblocksOrder.begin(); tmpi != StatusblocksOrder.end(); tmpi++) {
			if (tmpi->first == i->first) {
				isfind = true;
			}
		}
		if(isfind==false)
			StatusblocksOrder.insert(make_pair(i->first, StatusblocksOrder.size()));
		//if (StatusblocksOrder.find(i->first) == StatusblocksOrder.end()) {
		//	StatusblocksOrder.insert(make_pair(i->first,StatusblocksOrder.size()));
		//}

		for (auto ii = i->second.begin(); ii != i->second.end(); ii++) {
			bool isfind = false;
			for (auto tmpi = StatusblocksOrder.begin(); tmpi != StatusblocksOrder.end(); tmpi++) {
				if (tmpi->first == ii->second) {
					isfind = true;
				}
			}
			if (isfind == false)
				StatusblocksOrder.insert(make_pair(ii->second, StatusblocksOrder.size()));
			/*if (StatusblocksOrder.find(ii->second) == StatusblocksOrder.end()) {
				StatusblocksOrder.insert(make_pair(ii->second, StatusblocksOrder.size()));
			}*/
		}
		
	}

	//makeVtsets();

	for (auto i = GOset.begin(); i != GOset.end(); i++) {
		for (auto tmpsearchsymbol = i->second.begin(); tmpsearchsymbol != i->second.end(); tmpsearchsymbol++) {
			if(Vnset.find(tmpsearchsymbol->first)==Vnset.end())
				SearchSymbolsets.insert(tmpsearchsymbol->first);
			else
				VnSymbolSets.insert(tmpsearchsymbol->first);
		}
	}

	//存入表中
	//createAnalysedSheet();

	
	//建立空表
	for (auto row_iter = StatusblocksOrder.begin(); row_iter != StatusblocksOrder.end(); row_iter++) {
		for (auto col_iter = SearchSymbolsets.begin(); col_iter != SearchSymbolsets.end(); col_iter++) {
			ACTION[row_iter->second][*col_iter] = "";
		}

		for (auto col_iter = VnSymbolSets.begin(); col_iter != VnSymbolSets.end(); col_iter++) {
			GOTO[row_iter->second][*col_iter] = "";
		}
	}

	//写入空表
	for (auto tmpi = GOset.begin(); tmpi != GOset.end(); tmpi++) {
		for (auto tmpchar = tmpi->second.begin(); tmpchar != tmpi->second.end(); tmpchar++) {
			if (SearchSymbolsets.find(tmpchar->first) != SearchSymbolsets.end()) {
				char numbertmpbuffer[10];
				_itoa_s(StatusblocksOrder[tmpchar->second], numbertmpbuffer,9);
				ACTION[StatusblocksOrder[tmpi->first]][tmpchar->first] = "s" + std::string{ numbertmpbuffer };
			}
			else {
				char numbertmpbuffer[10];
				_itoa_s(StatusblocksOrder[tmpchar->second], numbertmpbuffer, 9);
				GOTO[StatusblocksOrder[tmpi->first]][tmpchar->first] = std::string{ numbertmpbuffer };
			}
				
		}
		
	}

	for (auto tmpi = StatusblocksOrder.begin(); tmpi != StatusblocksOrder.end(); tmpi++) {
		ReservedStatusblocksOrder.insert(make_pair(tmpi->second, tmpi->first));
	}

	for (auto tmp = StatusblocksOrder.begin(); tmp != StatusblocksOrder.end(); tmp++) {
		if (tmp->first.ProjectItem.size() == 1 && tmp->first.ProjectItem.begin()->index == tmp->first.ProjectItem.begin()->GrammarSentence.length()) {	//规约
			if (tmp->first.ProjectItem.begin()->GrammarVn == "0" && tmp->first.ProjectItem.begin()->SearchSymbol.size() == 1 && *tmp->first.ProjectItem.begin()->SearchSymbol.begin() == '#' && tmp->first.ProjectItem.begin()->GrammarSentence == std::string{ mostcharacter }) {	//ACC接受
				ACTION[StatusblocksOrder[tmp->first]]['#'] = "ACC";		//ACC
			}
			else {
				for (auto tmpchar = tmp->first.ProjectItem.begin()->SearchSymbol.begin(); tmpchar != tmp->first.ProjectItem.begin()->SearchSymbol.end(); tmpchar++) {
					
					for (auto tmper = ReservedStatusblocksOrder.begin(); tmper != ReservedStatusblocksOrder.end(); tmper++) {
						if (tmper->second.ProjectItem.size()==1 && tmper->second.ProjectItem.begin()->GrammarVn == tmp->first.ProjectItem.begin()->GrammarVn && tmper->second.ProjectItem.begin()->GrammarSentence == tmp->first.ProjectItem.begin()->GrammarSentence) {
							char numbertmpbuffer[10];
							_itoa_s(StatusblocksOrder[tmp->first], numbertmpbuffer, 9);
							ACTION[StatusblocksOrder[tmp->first]][*tmpchar] = "r" + string{ numbertmpbuffer };
						}
					}
					
				}

			}
		}
	}

	
	
}

//statusblock CheckLR1class::GO(statusblock input, char symbol) {
//	statusblock output;
//	for (auto i = input.ProjectItem.begin(); i != input.ProjectItem.end(); i++) {
//		auto tmp = *i;
//		if (tmp.GrammarSentence.at(tmp.index) == symbol) {
//			tmp.index++;
//			if(tmp.index<=tmp.GrammarSentence.length())
//				statusblockcpy(output,getCLOSURE(tmp));
//			else if(tmp.index == tmp.GrammarSentence.length() + 1) {
//				//TODO 终结符
//				output.ProjectItem[tmp.ProjectSentenceorder]=tmp;
//			}
//			else {
//				
//			}
//
//		}
//	}
//
//	return output;
//}

//获取当前项目族闭包
statusblock CheckLR1class::getCLOSURE(statusblock input) {
	statusblock output;
	output = input;

	while (true) {
		bool isstatblockincreased = false;
		for (auto i = output.ProjectItem.begin(); i != output.ProjectItem.end(); i++) {
			//if (Vnset.find(i->second.GrammarSentence.at(i->second.index)) != Vnset.end()) {
			for (auto ii = GrammarFormula.begin(); ii != GrammarFormula.end(); ii++) {

				if (i->index < i->GrammarSentence.length() && i->GrammarSentence.at(i->index) == ii->second.at(0)) {//如果是非终结符则展开
					ProjectSentence tmpsentence;
					tmpsentence.index = 0;
					tmpsentence.GrammarVn = ii->second;
					tmpsentence.GrammarSentence = ii->first;

					if (i->index + 1 == i->GrammarSentence.size()) {//如果已到结尾
						tmpsentence.SearchSymbol = searchFIRSTsets(NULLCHARACTER, i->SearchSymbol);
					}
					else
						tmpsentence.SearchSymbol = searchFIRSTsets(i->GrammarSentence.at(i->index + 1), i->SearchSymbol);

					auto thei = output.ProjectItem.begin();			//这个句子不存在则添加并isstatblockincreased=true

					for (; thei != output.ProjectItem.end(); thei++) {
						if (*thei == tmpsentence) {
							break;
						}
					}
					if (thei == output.ProjectItem.end()) {
						//output += tmpsentence;
						output.ProjectItem.insert(tmpsentence);
						isstatblockincreased = true;
					}
				}
			}
			//}
		}
		if (isstatblockincreased == false) {
			break;
		}
	}
	
	bool ischanged = false;

	while (true) {															//合并相同符号不同搜索符
		ischanged = false;
		auto theend = output.ProjectItem.end();
		theend--;
		for (auto i = output.ProjectItem.begin(); ischanged == false && i != output.ProjectItem.end() && i != theend; i++) {
			auto j = i++;
			for (auto ii = j; ii != output.ProjectItem.end(); ii++) {
				if (i == ii)
					continue;
				if (i->GrammarVn == ii->GrammarVn && i->GrammarSentence == ii->GrammarSentence && i->index == ii->index) {
					ProjectSentence newtmp;
					newtmp.GrammarSentence = i->GrammarSentence;
					newtmp.GrammarVn = i->GrammarVn;
					newtmp.index = i->index;
					newtmp.SearchSymbol.insert(i->SearchSymbol.begin(), i->SearchSymbol.end());
					newtmp.SearchSymbol.insert(ii->SearchSymbol.begin(), ii->SearchSymbol.end());
					output.ProjectItem.insert(newtmp);

					auto oldi = i, oldii = ii;
					i++;
					ii++;
					output.ProjectItem.erase(oldi);
					output.ProjectItem.erase(oldii);
					ischanged = true;
					break;
				}
			}

			if (ischanged == true) {
				break;
			}
		}
		
		if (ischanged == false) {
			break;
		}
	}

	return output;
}


////根据输入项目求其此时状态族
//statusblock CheckLR1class::getCLOSURE(ProjectSentence input) {
//	statusblock output;
//	output.ProjectItem.clear();
//	if (input.index <= input.GrammarSentence.length()) {
//		output.ProjectItem[input.ProjectSentenceorder]=input;
//		char nowchar = input.GrammarSentence.at(input.index);
//		bool finished = false;
//		
//		string lasttotalstring = input.totalstring;
//		int lastindex = input.index;
//		while (finished == false) {
//			if (Vnset.find(nowchar) != Vnset.end()) {
//				for (auto i = GrammarFormula.begin(); i != GrammarFormula.end(); i++) {
//					if (nowchar == i->second.at(0)) {
//						ProjectSentence tmp;
//						tmp.GrammarVn = i->second;
//						tmp.GrammarSentence = i->first;
//						tmp.index = 0;
//						tmp.SearchSymbol = searchFIRSTsets(input.totalstring.substr(1));
//						tmp.totalstring = /*lasttotalstring.substr(0, lastindex) + */i->first + lasttotalstring.substr(lastindex + 1);
//						output.ProjectItem[tmp.ProjectSentenceorder]=tmp;
//						nowchar = i->first.at(0);
//
//						lasttotalstring = tmp.totalstring;
//						lastindex = tmp.index;
//					}
//				}
//			}
//			else {
//				finished = true;
//			}
//		}
//	}
//
//	return output;
//}

void CheckLR1class::makeAnalysedSheet(string input) {
	std::string tmp = input;

	mostcharacter = tmp.at(0);

	//mostcharacter = input.at(0);

	std::string tmpheader, tmptail;
	int flag = 1;	//flag==1 for tmpheader flag==2 for tmptail
	for (auto i = tmp.begin(); i != tmp.end(); i++) {
		if (*i == '\n' && flag == 2) {
			if (tmptail != "" && tmpheader != "")
				GrammarFormula.insert(std::make_pair(tmptail, tmpheader));
			//GrammarFormula[tmptail] = tmpheader;

			tmpheader = "";
			tmptail = "";
			flag = 1;
			
		}
		else if (*i == '-' && *(i + 1) == '>' && flag == 1) {
			i++;
			flag = 2;
		}
		else if (*i == '|' && flag == 2) {
			//GrammarFormula[tmptail] = tmpheader;
			GrammarFormula.insert(std::make_pair(tmptail, tmpheader));

			tmptail = "";
		}
		else if (flag == 1) {
			tmpheader += *i;
		}
		else if (flag == 2) {
			tmptail += *i;
		}
	}
	if (tmptail != "" && tmpheader != "")
		GrammarFormula.insert(std::make_pair(tmptail, tmpheader));
	//GrammarFormula[tmptail] = tmpheader;


	GrammarFormula.insert(std::make_pair(string{ mostcharacter }, string{ "0" }));		//拓广文法 非终结符不可能为零 故 “0” 最小 最先录入

	Vnset.clear();

	for (auto i = GrammarFormula.begin(); i != GrammarFormula.end(); i++) {
		Vnset.insert(i->second.at(0));
	}

	getFIRSTsets();

	makeAnalysedSheet();

	
	for (int i = 1; i < 10; i++) {
		cout << "wasdas";
	}

	GrammarFormula.erase(std::string{ NULLCHARACTER });
}
