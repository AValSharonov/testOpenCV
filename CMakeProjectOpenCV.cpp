// CMakeProjectOpenCV.cpp: определяет точку входа для приложения.
//
#include <iostream>
#include "DispatchCmd.h"

using namespace std;
int    main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
	std::shared_ptr < DispatchCmd >  DspCmd(new DispatchCmd());
	
		DspCmd->countArg = argc;
	
		int ret;
		while (1)
		{
			char s[80];
			cin.getline(s, 80);
			cout << s << endl;
			system("pause");

			ret=DspCmd->Manager(s);
			if(ret<0)
			break;
		}
	//}
	return 0;
}


