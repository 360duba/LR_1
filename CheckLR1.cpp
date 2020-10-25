
#include"CheckLR1.h"



TCHAR* CheckLR1class::toTCHAR(std::string input) {
	if (input == "") {
		output[0] = '\0';
		output[1] = '\0';
		output[2] = '\0';
		output[3] = '\0';
	}
	else {
		_tcscpy_s(output, CA2T(input.c_str()));
	}

	return output;
}

TCHAR* CheckLR1class::toTCHAR(int input) {
	_itot_s(input, output, 10);
	return output;
}



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
		//if (tmp.index < tmp.GrammarSentence.length()) {
		if(get<Index>(tmp)<get<GrammarSentence>(tmp).length()){
			//output.insert(tmp.GrammarSentence.at(tmp.index));
			output.insert(get <GrammarSentence>(tmp).at(get <Index>(tmp)));
		}
		
	}
	return output;
}



statusblock CheckLR1class::GO(statusblock I, char X) {
	statusblock output;
	statusblock needtogetCLOSURE;
	for (auto i = I.ProjectItem.begin(); i != I.ProjectItem.end(); i++) {
		//if (i->index == i->GrammarSentence.length()) {	//已到结尾，不须扩展
		if (get<3>(*i) == get<GrammarSentence>(*i).length()) {	//已到结尾，不须扩展
		}
		//else if (i->index<i->GrammarSentence.length() && i->GrammarSentence.at(i->index) == X) {	//需要拓展
		else if (get<3>(*i) < get<GrammarSentence>(*i).length() && get<GrammarSentence>(*i).at(get<3>(*i)) == X) {	//需要拓展
			//auto tmpcopyed = *i;
			//tmpcopyed.index++;
			auto tmpcopyed = make_tuple(get<GrammarVn>(*i), get<GrammarSentence>(*i), get< SearchSymbol>(*i),get<Index>(*i)+1);
			needtogetCLOSURE += tmpcopyed;
			output += tmpcopyed;
			
		}
	}

	output += getCLOSURE(needtogetCLOSURE);
	


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
				if (get< GrammarVn>(*i) == get< GrammarVn>(*ii) && get< GrammarSentence>(*i) == get< GrammarSentence>(*ii) && get<Index>(*i) == get<Index>(*ii)) {

					SymbolSet newSymbolSet;
					newSymbolSet.insert(get< SearchSymbol>(*i).begin(), get< SearchSymbol>(*i).end());
					newSymbolSet.insert(get< SearchSymbol>(*ii).begin(), get< SearchSymbol>(*ii).end());
					ProjectSentence newtmp = make_tuple(get<GrammarVn>(*i), get<GrammarSentence>(*i), newSymbolSet, get<Index>(*i));
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

void CheckLR1class::collapsesameFormula(statusblock& input) {
	bool ischanged = false;


	while (true) {															//合并相同符号不同搜索符
		ischanged = false;
		auto theend = input.ProjectItem.end();
		theend--;
		for (auto i = input.ProjectItem.begin(); ischanged == false && i != input.ProjectItem.end() && i != theend; i++) {
			auto j = i;
			j++;
			for (auto ii = j; ii != input.ProjectItem.end(); ii++) {
				if (i == ii)
					continue;
				if (get< GrammarVn>(*i) == get< GrammarVn>(*ii) && get< GrammarSentence>(*i) == get< GrammarSentence>(*ii) && get<Index>(*i) == get<Index>(*ii)) {
					/*ProjectSentence newtmp;
					newtmp.GrammarSentence = i->GrammarSentence;
					newtmp.GrammarVn = i->GrammarVn;
					newtmp.index = i->index;
					newtmp.SearchSymbol.insert(i->SearchSymbol.begin(), i->SearchSymbol.end());
					newtmp.SearchSymbol.insert(ii->SearchSymbol.begin(), ii->SearchSymbol.end());*/

					SymbolSet newSymbolSet;
					newSymbolSet.insert(get< SearchSymbol>(*i).begin(), get< SearchSymbol>(*i).end());
					newSymbolSet.insert(get< SearchSymbol>(*ii).begin(), get< SearchSymbol>(*ii).end());
					ProjectSentence newtmp = make_tuple(get<GrammarVn>(*i), get<GrammarSentence>(*i), newSymbolSet, get<Index>(*i));
					input.ProjectItem.insert(newtmp);

					auto oldi = i, oldii = ii;
					i++;
					ii++;
					input.ProjectItem.erase(oldi);
					input.ProjectItem.erase(oldii);
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
}


void CheckLR1class::makeAnalysedSheet() {
	statusblock initalstatusblock;
	
	/*tmp.GrammarSentence = string{ mostcharacter };
	tmp.GrammarVn = "0";
	tmp.index = 0;*/
	SymbolSet tmpsymbolset;
	tmpsymbolset.insert('#');
	//tmp.SearchSymbol = tmpsymbolset;
	ProjectSentence tmp = make_tuple("0", string{ mostcharacter }, tmpsymbolset, 0);
	initalstatusblock.ProjectItem.insert(tmp);
	initalstatusblock += getCLOSURE(initalstatusblock);

	//set<set<ProjectSentence>> alreadyexpanedstatusblock;		//已经展开过的状态族集合的集合 由于statusblock结构中存在自增变量无法利用
	stack<statusblock> searchingstack;
	searchingstack.push(initalstatusblock);

	while (searchingstack.empty() == false) {
		statusblock currentstatusblock = searchingstack.top();

		collapsesameFormula(currentstatusblock);

		searchingstack.pop();
		SymbolSet symbol_iter = getSymbols(currentstatusblock);

		for (auto i = symbol_iter.begin(); i != symbol_iter.end(); i++) {
			statusblock tmpfetchedstatusblock = GO(currentstatusblock, *i);


			collapsesameFormula(tmpfetchedstatusblock);


			GOset[currentstatusblock][*i] = tmpfetchedstatusblock;

			bool isfinded = false;

			for (auto GOset_iter = GOset.begin(); GOset_iter != GOset.end(); GOset_iter++) {
				if (GOset_iter->first == tmpfetchedstatusblock) {
					isfinded = true;
				}
			}

			if(isfinded==false )		//尚未进行展开
				searchingstack.push(tmpfetchedstatusblock);
		}
	}

	makeStatusblockssets(initalstatusblock);
	drawDFAGraph();
}

//保存为dot格式并利用graphviz输出DFA图像
void CheckLR1class::drawDFAGraph() {
	FILE* fp;
	char PhotoPath[] = "tmp.jpg";
	char DotFilePath[] = "tmp.dot";
	fopen_s(&fp, DotFilePath, "w+");

	if (fp != NULL) {
		string outputbuffer = "digraph G{\n";

		for (auto i = StatusblocksOrder.begin(); i != StatusblocksOrder.end(); i++) {
			string tmpbuffer = "";
			char numbertmpbuffer[20] = { 0 };

			_itoa_s(StatusblocksOrder[i->first], numbertmpbuffer, 10);
			tmpbuffer += "node";
			tmpbuffer += string{ numbertmpbuffer };
			string numbertmpbufferstring1 = "node";
			numbertmpbufferstring1 += string{ numbertmpbuffer };
			tmpbuffer += "[label =\"";

			string totalgroupsets = "";

			for (auto ii = i->first.ProjectItem.begin(); ii != i->first.ProjectItem.end(); ii++) {
				string groupsets = "";
				groupsets += get<GrammarVn>(*ii);
				groupsets += "->";
				groupsets += get<GrammarSentence>(*ii);
				groupsets += " , ";
				auto tmp = get<SymbolSet>(*ii);
				auto tmpend = tmp.end();
				tmpend--;
				for (auto iii = tmp.begin(); iii != tmpend; iii++) {
					groupsets += *iii;
					groupsets += "|";
				}
				groupsets += *tmpend;

				auto tmp2 = groupsets.begin();
				int counter = 0;
				while (counter < get<Index>(*ii) + 3) {
					counter++;
					tmp2++;
				}
				groupsets.insert(tmp2, '.');
				groupsets += "\\n";
				totalgroupsets += groupsets;
			}

			tmpbuffer += totalgroupsets;
			tmpbuffer += "\", shape = \"box\"];\n";

			outputbuffer += tmpbuffer;
		}


		for (auto i = GOset.begin(); i != GOset.end(); i++) {
			string tmpbuffer = "";
			char numbertmpbuffer[20] = { 0 };

			_itoa_s(StatusblocksOrder[i->first], numbertmpbuffer, 10);
			string numbertmpbufferstring1 = "node";
			numbertmpbufferstring1 += string{ numbertmpbuffer };

			for (auto ii = i->second.begin(); ii != i->second.end(); ii++) {
				char numbertmpbuffer2[20] = { 0 };

				_itoa_s(StatusblocksOrder[ii->second], numbertmpbuffer2, 10);
				string numbertmpbufferstring2 = "node";
				numbertmpbufferstring2 += string{ numbertmpbuffer2 };

				tmpbuffer += numbertmpbufferstring1;
				tmpbuffer += "->";
				tmpbuffer += numbertmpbufferstring2;
				tmpbuffer += " [label=\"";
				tmpbuffer += string{ ii->first };
				tmpbuffer += "\"]; \n";


			}

			outputbuffer += tmpbuffer;
		}

		outputbuffer += "}";
		char* outputbufferchar = (char*)malloc(sizeof(char) * (outputbuffer.length() + 1));
		int i = 0;
		for (; i < outputbuffer.length(); i++) {
			*(outputbufferchar + i) = outputbuffer.at(i);
		}
		*(outputbufferchar + i) = '\0';
		fwrite(outputbufferchar, sizeof(char), outputbuffer.length() + 1, fp);
		fclose(fp);

		WinExec("Graphviz\\bin\\dot.exe -Tjpg tmp.dot -o tmp.jpg", SW_HIDE);
		//system("Graphviz\\bin\\dot.exe -Tjpg tmp.dot -o tmp.jpg");	//xdm 出不来jpg图像记得在这里改路径啊

	}

	


}

void CheckLR1class::makeStatusblockssets(statusblock &initalstatusblock) {


	StatusblocksOrder.insert(make_pair(initalstatusblock, 0));			//零状态

	for (auto i = GOset.begin(); i != GOset.end(); i++) {
		bool isfind = false;
		for (auto tmpi = StatusblocksOrder.begin(); tmpi != StatusblocksOrder.end(); tmpi++) {
			if (tmpi->first == i->first) {
				isfind = true;
			}
		}
		if (isfind == false)
			StatusblocksOrder.insert(make_pair(i->first, StatusblocksOrder.size()));

		for (auto ii = i->second.begin(); ii != i->second.end(); ii++) {
			bool isfind = false;
			for (auto tmpi = StatusblocksOrder.begin(); tmpi != StatusblocksOrder.end(); tmpi++) {
				if (tmpi->first == ii->second) {
					isfind = true;
				}
			}
			if (isfind == false)
				StatusblocksOrder.insert(make_pair(ii->second, StatusblocksOrder.size()));

		}

	}


	for (auto i = GOset.begin(); i != GOset.end(); i++) {
		for (auto tmpsearchsymbol = i->second.begin(); tmpsearchsymbol != i->second.end(); tmpsearchsymbol++) {
			if (Vnset.find(tmpsearchsymbol->first) == Vnset.end())
				SearchSymbolsets.insert(tmpsearchsymbol->first);
			else
				VnSymbolSets.insert(tmpsearchsymbol->first);
		}
	}
	SearchSymbolsets.insert('#');
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
			if (StatusblocksOrder.find(tmpchar->second) != StatusblocksOrder.end() && StatusblocksOrder.find(tmpi->first) != StatusblocksOrder.end()) {
				if (SearchSymbolsets.find(tmpchar->first) != SearchSymbolsets.end()) {
					char numbertmpbuffer[10] = { 0 };
					_itoa_s(StatusblocksOrder[tmpchar->second], numbertmpbuffer, 10);
					ACTION[StatusblocksOrder[tmpi->first]][tmpchar->first] = "s" + std::string{ numbertmpbuffer };
				}
				else {
					char numbertmpbuffer[10] = { 0 };
					auto check = StatusblocksOrder.at(tmpchar->second);
					_itoa_s(StatusblocksOrder.at(tmpchar->second), numbertmpbuffer, 10);
					GOTO[StatusblocksOrder[tmpi->first]][tmpchar->first] = std::string{ numbertmpbuffer };
				}
			}
		}

	}

	for (auto tmpi = StatusblocksOrder.begin(); tmpi != StatusblocksOrder.end(); tmpi++) {
		ReservedStatusblocksOrder.insert(make_pair(tmpi->second, tmpi->first));
	}

	for (auto tmp = StatusblocksOrder.begin(); tmp != StatusblocksOrder.end(); tmp++) {
		//if (tmp->first.ProjectItem.size() == 1 && tmp->first.ProjectItem.begin()->index == tmp->first.ProjectItem.begin()->GrammarSentence.length()) {	//规约
		for (auto iter = tmp->first.ProjectItem.begin(); iter != tmp->first.ProjectItem.end(); iter++) {
			if (get< Index>(*iter) == get<GrammarSentence>(*iter).length()) {
				for (auto tmpchar = get< SearchSymbol>(*iter).begin(); tmpchar != get< SearchSymbol>(*iter).end(); tmpchar++) {

					int Grammarorder = 0;
					for (auto Grammartmper = GrammarFormula.begin(); Grammartmper != GrammarFormula.end(); Grammartmper++, Grammarorder++) {
						if (Grammartmper->second == get<GrammarVn>(*iter) && Grammartmper->first == get<GrammarSentence>(*iter)) {

							char numbertmpbuffer[10] = { 0 };
							_itoa_s(Grammarorder, numbertmpbuffer, 10);
							ACTION[StatusblocksOrder[tmp->first]][*tmpchar] = "r" + string{ numbertmpbuffer };
						}

					}

				}
			}
		}
	}

	for (auto tmp = StatusblocksOrder.begin(); tmp != StatusblocksOrder.end(); tmp++) {
		for (auto iter = tmp->first.ProjectItem.begin(); iter != tmp->first.ProjectItem.end(); iter++) {
			if (get< GrammarVn>(*iter) == "0" && get< Index>(*iter) == get<GrammarSentence>(*iter).length() && get<SearchSymbol>(*iter).size() == 1 && *get< SearchSymbol >(*iter).begin() == '#' && get< GrammarSentence>(*iter) == std::string{ mostcharacter }) {	//ACC接受
				if (StatusblocksOrder.find(tmp->first) != StatusblocksOrder.end())
					ACTION[StatusblocksOrder[tmp->first]]['#'] = "ACC";		//ACC
			}
		}
	}

	//if (tmp->first.ProjectItem.size() == 1 && get<Index>(*tmp->first.ProjectItem.begin()) == get<GrammarSentence>(*tmp->first.ProjectItem.begin()).length()) {	//规约
		//if (tmp->first.ProjectItem.begin()->GrammarVn == "0" && tmp->first.ProjectItem.begin()->SearchSymbol.size() == 1 && *tmp->first.ProjectItem.begin()->SearchSymbol.begin() == '#' && tmp->first.ProjectItem.begin()->GrammarSentence == std::string{ mostcharacter }) {	//ACC接受

			//for (auto tmpchar = tmp->first.ProjectItem.begin()->SearchSymbol.begin(); tmpchar != tmp->first.ProjectItem.begin()->SearchSymbol.end(); tmpchar++) {
			//for (auto tmpchar = get< SearchSymbol>(*tmp->first.ProjectItem.begin()).begin(); tmpchar != get< SearchSymbol>(*tmp->first.ProjectItem.begin()).end(); tmpchar++) {
				//for (auto tmper = ReservedStatusblocksOrder.begin(); tmper != ReservedStatusblocksOrder.end(); tmper++) {
					/*if (tmper->second.ProjectItem.size()==1 && get< GrammarVn>(*tmper->second.ProjectItem.begin()) == get< GrammarVn>(*tmp->first.ProjectItem.begin()) && get< GrammarSentence>(*tmper->second.ProjectItem.begin()) == get< GrammarSentence>(*tmp->first.ProjectItem.begin())) {
						int Grammarorder = 0;
						for(auto Grammartmper=GrammarFormula.begin(); Grammartmper!=GrammarFormula.end(); Grammartmper++, Grammarorder++){
							if (Grammartmper->second == get<GrammarVn>(*tmper->second.ProjectItem.begin()) && Grammartmper->first == get<GrammarSentence>(*tmper->second.ProjectItem.begin())) {

								char numbertmpbuffer[10] = { 0 };
								_itoa_s(Grammarorder, numbertmpbuffer, 10);
								ACTION[StatusblocksOrder[tmp->first]][*tmpchar] = "r" + string{ numbertmpbuffer };
							}

						}

					}
				}

			}

	}*/



}

//获取当前项目族闭包
statusblock CheckLR1class::getCLOSURE(statusblock input) {
	statusblock output;
	output = input;

	while (true) {
		bool isstatblockincreased = false;
		for (auto i = output.ProjectItem.begin(); i != output.ProjectItem.end(); i++) {
			//if (Vnset.find(i->second.GrammarSentence.at(i->second.index)) != Vnset.end()) {
			for (auto ii = GrammarFormula.begin(); ii != GrammarFormula.end(); ii++) {

				if (get< Index>(*i) < get< GrammarSentence>(*i).length() && get< GrammarSentence>(*i).at(get< Index>(*i)) == ii->second.at(0)) {//如果是非终结符则展开
					/*ProjectSentence tmpsentence;
					tmpsentence.index = 0;
					tmpsentence.GrammarVn = ii->second;
					tmpsentence.GrammarSentence = ii->first;*/
					ProjectSentence tmpsentence;
					if (get<Index>(*i) + 1 == get< GrammarSentence>(*i).size()) {//如果已到结尾
						tmpsentence = make_tuple(ii->second, ii->first, searchFIRSTsets(NULLCHARACTER, get< SearchSymbol>(*i)), 0);
						//tmpsentence.SearchSymbol = searchFIRSTsets(NULLCHARACTER, i->SearchSymbol);
					}
					else
						tmpsentence = make_tuple(ii->second, ii->first, searchFIRSTsets(get< GrammarSentence>(*i).at(get<Index>(*i) + 1), get< SearchSymbol>(*i)), 0);
						//tmpsentence.SearchSymbol = searchFIRSTsets(i->GrammarSentence.at(i->index), i->SearchSymbol);

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
	
	collapsesameFormula(output);

	return output;
}



void CheckLR1class::makeAnalysedSheet(string input) {
	ACTION.clear();
	GOTO.clear();
	SearchSymbolsets.clear();
	VnSymbolSets.clear();
	StatusblocksOrder.clear();
	ReservedStatusblocksOrder.clear();
	AnalysedDFA.clear();
	Vnset.clear();
	FIRSTset.clear();
	GrammarFormula.clear();
	GOset.clear();


	std::string tmp = input;

	mostcharacter = tmp.at(0);

	//mostcharacter = input.at(0);

	std::string tmpheader, tmptail;
	int flag = 1;	//flag==1 for tmpheader flag==2 for tmptail
	for (auto i = tmp.begin(); i != tmp.end(); i++) {
		if ((*i == '\r' || *i == '\n') && flag == 2) {
			if (tmptail != "" && tmpheader != "")
				GrammarFormula.insert(std::make_pair(tmptail, tmpheader));
			//GrammarFormula[tmptail] = tmpheader;

			tmpheader = "";
			tmptail = "";
			flag = 1;
			i++;
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

	

	GrammarFormula.erase(std::string{ NULLCHARACTER });
}



/******************************************************************************
*				函数名：		BindInputHWND
*				函数功能：	绑定输入文本框的窗口句柄
*				传入参数：	Input_RichTextDialogclass HWND 要绑定的窗口句柄
*				传出参数：	void
*******************************************************************************/
void CheckLR1class::BindInputHWND(HWND Input_RichTextDialogclass)
{
	// TODO: Add your implementation code here.
	HWND tmp = Input_RichTextDialogclass;
	Input_Bind_RichTextDialogclass.RichTextDialog_BindHWND(tmp);
}

/******************************************************************************
*				函数名：		BindOutputHWND
*				函数功能：	绑定输出列表框的窗口句柄
*				传入参数：	Output_RichTextDialogclass HWND 要绑定的窗口句柄
*				传出参数：	void
*******************************************************************************/
void CheckLR1class::BindOutputHWND(HWND Output_RichTextDialogclass)
{
	// TODO: Add your implementation code here.
	HWND tmp = Output_RichTextDialogclass;
	Output_Bind_RichTextDialogclass = tmp;
	//Output_Bind_RichTextDialogclass.RichTextDialog_BindHWND(tmp);
}

/******************************************************************************
*				函数名：		BindoutputFirstSetHWND
*				函数功能：	绑定FIRST列表框的窗口句柄
*				传入参数：	Output_RichTextDialogclass HWND 要绑定的窗口句柄
*				传出参数：	void
*******************************************************************************/
void CheckLR1class::BindoutputFirstSetHWND(HWND Output_RichTextDialogclass) {
	// TODO: Add your implementation code here.
	HWND tmp = Output_RichTextDialogclass;
	FirstSet_Bind_RichTextDialogclass = tmp;
}

/******************************************************************************
*				函数名：		BindoutputFollowSetHWND
*				函数功能：	绑定FIRST列表框的窗口句柄
*				传入参数：	Output_RichTextDialogclass HWND 要绑定的窗口句柄
*				传出参数：	void
*******************************************************************************/
void CheckLR1class::BindoutputFollowSetHWND(HWND Output_RichTextDialogclass) {
	// TODO: Add your implementation code here.
	HWND tmp = Output_RichTextDialogclass;
	FollowSet_Bind_RichTextDialogclass = tmp;
}

/******************************************************************************
*				函数名：		BindoutputStatusSheetHWND
*				函数功能：	绑定FIRST列表框的窗口句柄
*				传入参数：	Output_RichTextDialogclass HWND 要绑定的窗口句柄
*				传出参数：	void
*******************************************************************************/
void CheckLR1class::BindoutputStatusSheetHWND(HWND Output_RichTextDialogclass) {
	// TODO: Add your implementation code here.
	HWND tmp = Output_RichTextDialogclass;
	StatusSheet_Bind_RichTextDialogclass = tmp;

}

void CheckLR1class::outputGroupset() {
	SendMessage(FollowSet_Bind_RichTextDialogclass, LVM_DELETEALLITEMS, 0, 0);

	LV_ITEM lv;
	lv.mask = LVIF_TEXT;
	lv.iImage = 0;

	lv.iItem = 0;
	lv.iSubItem = 0;

	for (auto i = ReservedStatusblocksOrder.begin(); i != ReservedStatusblocksOrder.end(); i++) {
		lv.pszText = toTCHAR(i->first);
		ListView_InsertItem(FollowSet_Bind_RichTextDialogclass, &lv);
		lv.iSubItem++;
		
		string totalgroupsets = "";
		
		for (auto ii = i->second.ProjectItem.begin(); ii != i->second.ProjectItem.end(); ii++) {
			string groupsets = "";
			groupsets += get<GrammarVn>(*ii);
			groupsets += "->";
			groupsets += get<GrammarSentence>(*ii);
			groupsets += " , ";
			auto tmp = get<SymbolSet>(*ii);
			auto tmpend = tmp.end();
			tmpend--;
			for (auto iii = tmp.begin(); iii != tmpend; iii++) {
				groupsets += *iii;
				groupsets += "|";
			}
			groupsets += *tmpend;

			auto tmp2 = groupsets.begin();
			int counter = 0;
			while (counter < get<Index>(*ii)+3) {
				counter++;
				tmp2++;
			}
			groupsets.insert(tmp2, '.');
			for(int whatatmp=0; whatatmp < 50-groupsets.length(); whatatmp++)
				groupsets += " ";
			//groupsets += "\r\n";
			totalgroupsets += groupsets;
		}


		lv.pszText = toTCHAR(string{ totalgroupsets });
		ListView_SetItem(FollowSet_Bind_RichTextDialogclass, &lv);
		lv.iSubItem++;


		lv.iItem++;
		lv.iSubItem = 0;
	}
}

void CheckLR1class::outputstatussheet() {
	SendMessage(StatusSheet_Bind_RichTextDialogclass, LVM_DELETEALLITEMS, 0, 0);

	LV_ITEM lv;
	lv.mask = LVIF_TEXT;
	lv.iImage = 0;

	lv.iItem = 0;
	lv.iSubItem = 0;

	lv.pszText = toTCHAR("Step");
	ListView_InsertItem(StatusSheet_Bind_RichTextDialogclass, &lv);
	lv.iSubItem++;


	for (auto i = SearchSymbolsets.begin(); i != SearchSymbolsets.end(); i++) {
		lv.pszText = toTCHAR(string{ *i });
		ListView_SetItem(StatusSheet_Bind_RichTextDialogclass, &lv);
		lv.iSubItem++;
	}

	for (auto i = VnSymbolSets.begin(); i != VnSymbolSets.end(); i++) {
		lv.pszText = toTCHAR(string{ *i });
		ListView_SetItem(StatusSheet_Bind_RichTextDialogclass, &lv);
		lv.iSubItem++;
	}

	lv.iItem++;
	lv.iSubItem = 0;

	for (auto row = ACTION.begin(), row_goto = GOTO.begin(); row != ACTION.end(), row_goto != GOTO.end(); row++, row_goto++) {

		lv.pszText = toTCHAR( row->first );
		ListView_InsertItem(StatusSheet_Bind_RichTextDialogclass, &lv);
		lv.iSubItem++;

		for (auto col = row->second.begin(); col != row->second.end(); col++) {
			lv.pszText = toTCHAR(col->second);
			ListView_SetItem(StatusSheet_Bind_RichTextDialogclass, &lv);
			lv.iSubItem++;
		}
		
		for (auto col_goto = row_goto->second.begin(); col_goto != row_goto->second.end(); col_goto++) {
			lv.pszText = toTCHAR(col_goto->second);
			ListView_SetItem(StatusSheet_Bind_RichTextDialogclass, &lv);
			lv.iSubItem++;
		}

		lv.iItem++;
		lv.iSubItem = 0;
	}

}



void CheckLR1class::outputaline(deque<int> &statusstack, deque<char> &symbolstack, string & reststring,string instruction) {
	LV_ITEM lv;
	lv.mask = LVIF_TEXT;
	lv.iImage = 0;

	lv.iItem = totalitem;
	lv.iSubItem = 0;

	string statusstackstring,symbolstackstring;
	for (auto i = statusstack.begin(); i != statusstack.end(); i++) {
		char numbertmpbuffer[10];
		_itoa_s(*i, numbertmpbuffer, 10);
		statusstackstring += string{ numbertmpbuffer } + " ";
	}

	for (auto i = symbolstack.begin(); i != symbolstack.end(); i++) {
		
		symbolstackstring += string{ *i };
	}
	

	lv.pszText = toTCHAR(totalitem+1);
	ListView_InsertItem(Output_Bind_RichTextDialogclass, &lv);
	lv.iSubItem++;

	lv.pszText = toTCHAR(statusstackstring);
	ListView_SetItem(Output_Bind_RichTextDialogclass, &lv);
	lv.iSubItem++;

	lv.pszText = toTCHAR(symbolstackstring);
	ListView_SetItem(Output_Bind_RichTextDialogclass, &lv);
	lv.iSubItem++;

	lv.pszText = toTCHAR(reststring);
	ListView_SetItem(Output_Bind_RichTextDialogclass, &lv);
	lv.iSubItem++;

	lv.pszText = toTCHAR(instruction);
	ListView_SetItem(Output_Bind_RichTextDialogclass, &lv);
	lv.iSubItem++;

	totalitem++;
	//order++;
}


void CheckLR1class::outputGrammarFormula(){
	SendMessage(FirstSet_Bind_RichTextDialogclass, LVM_DELETEALLITEMS, 0, 0);
	LV_ITEM lv;
	lv.mask = LVIF_TEXT;
	lv.iImage = 0;

	lv.iItem = 0;
	lv.iSubItem = 0;

	int order = 0;

	for (auto i = GrammarFormula.begin(); i != GrammarFormula.end(); i++) {
		char numbuffer[10] = { 0 };
		_itoa_s(order, numbuffer, 10);

		lv.pszText = toTCHAR(string{ numbuffer });
		ListView_InsertItem(FirstSet_Bind_RichTextDialogclass, &lv);
		lv.iSubItem++;

		lv.pszText = toTCHAR(i->second);
		ListView_SetItem(FirstSet_Bind_RichTextDialogclass, &lv);
		lv.iSubItem++;

		lv.pszText = toTCHAR(i->first);
		ListView_SetItem(FirstSet_Bind_RichTextDialogclass, &lv);
		lv.iSubItem++;

		totalitem++;
		lv.iSubItem = 0;

		order++;
	}
	

	
}


void CheckLR1class::DoCheck() {

	//GOTO[8]['T'] = "7";
	//GOTO[8]['E'] = "2";					//TODO: Solve these Problem Thanks for the help from mxy.

	totalitem = 0;
	bool finished = 0;
	int position = 0;
	deque<int> statusstack;				//状态
	statusstack.push_back(0);
	deque<char> symbolstack;			//符号栈
	symbolstack.push_back('#');


	string reststring = "";
	while (position <= Input_Bind_RichTextDialogclass.RichTextDialog_GetLength()) {
		reststring += Input_Bind_RichTextDialogclass.RichTextDialog_GetChar(position);
		position++;
	}
	reststring += "#";

	//reststring = "i+i*i#";

	while(reststring.length()>0 /*&& symbolstack.size()>0 && statusstack.size()>0*/) {
		char inputchar = reststring.at(0);

		
		string tmp = ACTION[statusstack.back()][inputchar];
		if (tmp != "") {
			if (tmp.at(0) == 's') {
				string tmpbuffer = "ACTION[";

				char numbuffer[10] = { 0 };
				_itoa_s(statusstack.back(), numbuffer, 10);

				auto lastsymbolstack = symbolstack;
				auto laststatusstack = statusstack;

				

				symbolstack.push_back(inputchar);
				statusstack.push_back(atoi(tmp.substr(1).c_str()) );

				tmpbuffer += string{ numbuffer };
				tmpbuffer += ",";
				tmpbuffer += inputchar;
				tmpbuffer += "]=";
				tmpbuffer += tmp.substr(1);
				outputaline(laststatusstack, lastsymbolstack, reststring, tmpbuffer);

				reststring.erase(reststring.begin());

				
			}
			else if (tmp.at(0) == 'r') {
				int GrammarOrder = 0;
				for (auto tmper = GrammarFormula.begin(); tmper != GrammarFormula.end(); tmper++,GrammarOrder++) {
					if (GrammarOrder == atoi(tmp.substr(1).c_str())) {
						string tmpbuffer = tmper->second;
						tmpbuffer += "->";
						tmpbuffer += tmper->first;
						tmpbuffer += " reduced ,";
						tmpbuffer += "GOTO(";

						auto lastsymbolstack = symbolstack;
						auto laststatusstack = statusstack;

						for (int looptime = 0; looptime < tmper->first.length(); looptime++) {
							statusstack.pop_back();
							symbolstack.pop_back();
						}

						auto tmpstatus = statusstack.rbegin();
						//tmpstatus++;

						char numbuffer[10] = { 0 };
						_itoa_s(*tmpstatus, numbuffer, 10);

						tmpbuffer += string{ numbuffer };
						tmpbuffer += ",";
						tmpbuffer += tmper->second;
						tmpbuffer += ")=";


						tmpbuffer += GOTO[*tmpstatus][tmper->second.at(0)];
						outputaline(laststatusstack, lastsymbolstack, reststring, tmpbuffer);
						symbolstack.push_back(tmper->second.at(0));

					//	statusstack.pop_back();

						

						statusstack.push_back(atoi(GOTO[statusstack.back()][tmper->second.at(0)].substr(0).c_str()));

						
					}
				}
			}
			else if (tmp == "ACC") {
				finished = 1;

				outputaline(statusstack, symbolstack, reststring, "ACC");
				break;
			}
				
		}
		else {
			finished = 0;
			//error;
			outputaline(statusstack, symbolstack, reststring, "ERROR");
			break;
		}
		
		
	}
	

	finished;
}