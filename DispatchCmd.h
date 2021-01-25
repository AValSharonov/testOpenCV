#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "opencv.hpp"

using namespace std;
using namespace  cv;

class DispatchCmd
{
public:
	//DispatchCmd(int argc,char**argv);
	DispatchCmd(void);
	~DispatchCmd();

	int Manager(char* cmd);
	int  HandlerCmd();
	int CheckFormatCmd();
	int LoadFile();
	//int CheckName(char* name);
	int  WriteFile();
	int  Blur();
	int   ReSize();
	int   Help();
	int  GetLen(int flag);

	//vector<string> CmdList;
	//vector<string> ArgList;
	int countArg=0;

	std::string cmd;
	char* cmmd = NULL;
	char* bufName=NULL;
	char* bufNameSrc = NULL;
	std::string nameFile;
	std::map <string, Mat> imagesOrig;

	char index = 0,indImgSrc=0,indImgDest=0,indW=0,indH=0;
	int lenCmd=0, lenImgSrc = 0, lenImgDest = 0, lenW = 0, lenH = 0;
	int command = 0;
	Mat  imgClone;
	int len = 0, curIndex=0;
};



