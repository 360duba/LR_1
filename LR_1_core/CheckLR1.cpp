
#include"CheckLR1.h"




////Ѱ�Ҹ����ַ�����FIRST��
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
//	if (Vnset.find(tmpinput.at(searchpointer)) != Vnset.end()) {//�Ƿ��ս��
//		//auto tmp = FIRSTset[input.at(searchpointer)];
//		//output.insert(tmp.begin(),tmp.end());
//		deque<char> waitingcheckstring;
//		deque<char>::iterator pointer;
//		waitingcheckstring.push_back(tmpinput.at(searchpointer));
//		pointer = waitingcheckstring.begin();
//		while (pointer != waitingcheckstring.end()) {
//
//			for (auto i = GrammarFormula.begin(); i != GrammarFormula.end(); i++) {			//i->first.at(0)Ϊ������ַ���
//				if (i->second.at(0) == *pointer) {
//					if (Vnset.find(i->first.at(0)) == Vnset.end()) {			//�ս��
//						output.insert(i->first.at(0));
//					}
//					else {
//						bool isexist = false;
//						for (auto tmpiter = waitingcheckstring.begin(); tmpiter != waitingcheckstring.end(); tmpiter++) {		//��ֹ�ظ��ݹ�
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


//���ü��FIRSTset�䶯�������FIRST���ĺ�������ͼ������ݹ�����ݣ����������µķ��ս��
void CheckLR1class::getFIRSTsets() {
	multimap<string, string> CopyedGrammarFormula = GrammarFormula;		//���ݵ��ķ����ʽ�б�

	while (true) {
		bool isFIRSTsetincreased = false;

		for (auto i = FIRSTset.begin(); i != FIRSTset.end() && isFIRSTsetincreased == false; i++) {		//ɾȥfirst���д��ڿ��ַ����ڲ���ʽ�Ҳ���λ���ַ�
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
			if (Vnset.find(i->first.at(0)) == Vnset.end() && (FIRSTset.find(i->second.at(0))== FIRSTset.end() || FIRSTset[i->second.at(0)].find(i->first.at(0)) == FIRSTset[i->second.at(0)].end())) {	//�ս����δ��¼ֱ�����
				FIRSTset[i->second.at(0)].insert(i->first.at(0));
				isFIRSTsetincreased = true;
			}
			if (Vnset.find(i->first.at(0)) != Vnset.end() && FIRSTset[i->first.at(0)].size() != 0) {						//���ս���Ҹ÷��ŷ��ս���ǿ�
				bool isallhasincluded = true;																					//�����ڲ����������
				
				
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

	if (input != NULLCHARACTER) {	//��ȡinput��first��
		if (Vnset.find(input) != Vnset.end()) {										//���ս��
			FIRSTsetofinput.insert(FIRSTset[input].begin(), FIRSTset[input].end());
			if (FIRSTset[input].find(NULLCHARACTER) == FIRSTset[input].end()) {	//���û���ҵ��ս�� ֱ�ӷ�����first��
				return FIRSTsetofinput;
			}
		}
		else {																		//�ս��
			FIRSTsetofinput.insert(input);
			return FIRSTsetofinput;
		}
		
	}

	for (auto i = lastsearchsymbol.begin(); i != lastsearchsymbol.end(); i++) {		//��Ҫ��ȡ����ķ���
		if (Vnset.find(*i) != Vnset.end())
			FIRSTsetofinput.insert(FIRSTset[*i].begin(), FIRSTset[*i].end());		//���ս��ȡFIRST��
		else
			FIRSTsetofinput.insert(*i);												//�ս��ȡ����
	}
	if (FIRSTsetofinput.size() == 0)
		FIRSTsetofinput.insert('#');
	return FIRSTsetofinput;
}

////���Դ״̬���Ŀ��״̬�����iscorrectΪfalse �򲻽��и��� ����false
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

////��ȡ״̬ת����
//void CheckLR1class::getStatusSheet() {
//	deque<statusblock> potentialdeque;	//������Ҫչ����������Ķ���
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

//��ȡ��ǰ״̬���п��ܵ���һ��ת������
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
		if (i->index == i->GrammarSentence.length()) {	//�ѵ���β��������չ
			/*auto tmpcopyed = *i;
			tmpcopyed.index;
			output += tmpcopyed;*/
			//auto oldi = i;
			//I.ProjectItem.insert(tmpcopyed);			
			//i++;
			//I.ProjectItem.erase(oldi);
		}
		else if (i->index<i->GrammarSentence.length() && i->GrammarSentence.at(i->index) == X) {	//��Ҫ��չ
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

	//set<set<ProjectSentence>> alreadyexpanedstatusblock;		//�Ѿ�չ������״̬�弯�ϵļ��� ����statusblock�ṹ�д������������޷�����
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
			//		if (!(theii_iter->GrammarSentence == theiii_iter->GrammarSentence && theii_iter->GrammarVn == theiii_iter->GrammarVn && theii_iter->index== theiii_iter->index && theii_iter->SearchSymbol== theiii_iter->SearchSymbol)){	//���չ���� ���ķ��� û�в�ͬ�� �� �����˽�β {
			//			isfinded = false;
			//			break;
			//		}
			//}//���⣺����
			for (auto GOset_iter = GOset.begin(); GOset_iter != GOset.end(); GOset_iter++) {
				if (GOset_iter->first == tmpfetchedstatusblock) {
					isfinded = true;
				}
			}
			

			if(isfinded==false /*|| theii_iter!= thei_iter->end() || theiii_iter != tmpfetchedstatusblock.ProjectItem.end()*/)		//��δ����չ��
				searchingstack.push(tmpfetchedstatusblock);
		}
	}
	



	//makeStatusblockssets();

	StatusblocksOrder.insert(make_pair(initalstatusblock, 0));			//��״̬

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

	//�������
	//createAnalysedSheet();

	
	//�����ձ�
	for (auto row_iter = StatusblocksOrder.begin(); row_iter != StatusblocksOrder.end(); row_iter++) {
		for (auto col_iter = SearchSymbolsets.begin(); col_iter != SearchSymbolsets.end(); col_iter++) {
			ACTION[row_iter->second][*col_iter] = "";
		}

		for (auto col_iter = VnSymbolSets.begin(); col_iter != VnSymbolSets.end(); col_iter++) {
			GOTO[row_iter->second][*col_iter] = "";
		}
	}

	//д��ձ�
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
		if (tmp->first.ProjectItem.size() == 1 && tmp->first.ProjectItem.begin()->index == tmp->first.ProjectItem.begin()->GrammarSentence.length()) {	//��Լ
			if (tmp->first.ProjectItem.begin()->GrammarVn == "0" && tmp->first.ProjectItem.begin()->SearchSymbol.size() == 1 && *tmp->first.ProjectItem.begin()->SearchSymbol.begin() == '#' && tmp->first.ProjectItem.begin()->GrammarSentence == std::string{ mostcharacter }) {	//ACC����
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
//				//TODO �ս��
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

//��ȡ��ǰ��Ŀ��հ�
statusblock CheckLR1class::getCLOSURE(statusblock input) {
	statusblock output;
	output = input;

	while (true) {
		bool isstatblockincreased = false;
		for (auto i = output.ProjectItem.begin(); i != output.ProjectItem.end(); i++) {
			//if (Vnset.find(i->second.GrammarSentence.at(i->second.index)) != Vnset.end()) {
			for (auto ii = GrammarFormula.begin(); ii != GrammarFormula.end(); ii++) {

				if (i->index < i->GrammarSentence.length() && i->GrammarSentence.at(i->index) == ii->second.at(0)) {//����Ƿ��ս����չ��
					ProjectSentence tmpsentence;
					tmpsentence.index = 0;
					tmpsentence.GrammarVn = ii->second;
					tmpsentence.GrammarSentence = ii->first;

					if (i->index + 1 == i->GrammarSentence.size()) {//����ѵ���β
						tmpsentence.SearchSymbol = searchFIRSTsets(NULLCHARACTER, i->SearchSymbol);
					}
					else
						tmpsentence.SearchSymbol = searchFIRSTsets(i->GrammarSentence.at(i->index + 1), i->SearchSymbol);

					auto thei = output.ProjectItem.begin();			//������Ӳ���������Ӳ�isstatblockincreased=true

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

	while (true) {															//�ϲ���ͬ���Ų�ͬ������
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


////����������Ŀ�����ʱ״̬��
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


	GrammarFormula.insert(std::make_pair(string{ mostcharacter }, string{ "0" }));		//�ع��ķ� ���ս��������Ϊ�� �� ��0�� ��С ����¼��

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
