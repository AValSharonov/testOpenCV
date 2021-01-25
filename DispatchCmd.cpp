#include "windows.h"
#include "DispatchCmd.h"
#include <opencv2\imgcodecs\legacy\constants_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>



//using namespace std;

DispatchCmd::DispatchCmd(void)
{
	//countArg = 0;
/*
	CmdList.push_back("load");  //0
	CmdList.push_back("ld");    //1
	CmdList.push_back("store"); //2
	CmdList.push_back("s");		//3
	CmdList.push_back("blur");	//4
	CmdList.push_back("resize");//5
	CmdList.push_back("help");	//6
	CmdList.push_back("h");		//7
	CmdList.push_back("exit");	//8
	CmdList.push_back("quit");	//9
	CmdList.push_back("q");		//10
	*/
	cmmd = NULL;
	///Mat i;
	//imagesOrig[" "] = NULL;
}

DispatchCmd::~DispatchCmd()
{
	//CmdList.clear();

}

#define LOAD 1
#define HELP 2
#define EXIT 3
#define UNKNOW_COMMAND 4
#define SAVE 5
#define BLUR 6
#define RESIZE 7

int DispatchCmd::Manager(char* cmd)
{
	int ret;
	 len = strlen(cmd);
	if (cmmd != NULL)
		free(cmmd);

		cmmd = (char*)malloc((strlen(cmd) + 1));
		memset(cmmd,0,len+1);
		strncpy_s(cmmd,len+1, cmd,80);

	command = 0;

	
	if (len == 1)
	{
		if(cmd[0]=='h' || cmd[0] == 'H')
			command = HELP;
		if (cmd[0] == 'q' || cmd[0] == 'Q' || cmd[0] == 'e' || cmd[0] == 'E')
			command = EXIT;
		if (cmd[0] == 's' || cmd[0] == 'S')
			command = SAVE;
		if (command==0)
		{
			command = UNKNOW_COMMAND;
		}
			ret=HandlerCmd();
			return ret;
	}
	else
	{
		//читаем первый символ - букву
		unsigned char c;
		int i = 0;
		while (1)
		{
			c = isalpha(cmd[i]);
			if (c == 0)
			{
				i++;
				if (i >= len)
				{
					command = UNKNOW_COMMAND;
					int ret=HandlerCmd();		return ret;
				}
				continue;
			}
			c = cmd[i];
			break;
		}
		index = i;
		if (c == 'e' || c == 'E')
			command = EXIT;
		if (c == 'q' || c == 'Q')
			command = EXIT;
		if (c == 'l' || c == 'L')
			command = LOAD;
		if(c == 's' || c == 'S')
			command = SAVE;
		if (c == 'b' || c == 'B')
			command = BLUR;
		if (c == 'r' || c == 'R')
			command = RESIZE;
	}
	
	 ret=HandlerCmd();
	return ret;
}

//---------------------------------------------------------------------------------------------
int  DispatchCmd::CheckFormatCmd()
{
	unsigned char c,point;
	int cntNoAlpha = 0;
	int offset = 0;
	int i = 0;
	index = 0; indImgSrc = 0; indImgDest = 0; indW = 0; indH = 0;
	lenImgSrc = 0, lenImgDest = 0, lenW = 0, lenH = 0;
	curIndex = 0;
	
	while (1)
	{							
		c = isspace(cmmd[i]);
			if (c)  //это пробел
			{
				cntNoAlpha = 1;
				i++;
				if (i >= len)
				{
					//command = UNKNOW_COMMAND;
					return 0;
				}
				continue;
			}		
		switch (offset)
		{
		case 0:
			index = i;			offset++;			break;
		case 1:
			lenCmd = curIndex+1;			break;
		case 2:
			indImgSrc = i-1;			offset++;					break;
		case 3:
			lenImgSrc = curIndex+1;			break;
		case 4:
			indImgDest = i-1;			offset++;			break;
		case 5:
			lenImgDest = curIndex+1;			break;
		case 6:
			indW = i-1;			offset++;				lenW = GetLen(0);
			break;
		case 7:
			 curIndex++;			break;
		case 8:
			indH = i - 1;			lenH = GetLen(1);
			offset++;				break;
		case 9:

			break;
		}
		if (cntNoAlpha)
		{
			offset++;   //начало отсчета букв
			cntNoAlpha = 0;
			curIndex = 0;
		}
		curIndex++;  //считает буквы
				
		i++;
		if (i >= len)
		{
			return 0;
		}
	}
	
}
//---------------------------------------------------------------------------------------------
int  DispatchCmd::GetLen(int flag)
{
	//читать из comm пока нет пробелов или не наступит конец буфера
	int length=0;
	int i = 0,j=0;
	char c;
	char buf[123] = { 0 };
	if (flag)  //height
		flag = indH;
	else
		flag = indW;
	if (flag == 0)		
		return 0;	//j = flag;
	while (1)
	{
		 c = isspace(cmmd[flag]);
		if (c)  //это пробел
		{
			length = atoi(buf);
			return length;
		}
		else
		{
					//			j++;
			buf[i] = cmmd[flag];
			flag++;
			if (flag >= len)
			{
				length = atoi(buf);
				return length;
			}
			i++;
		}
	}
}
//---------------------------------------------------------------------------------------------
int  DispatchCmd::HandlerCmd()
{
	int ret=0;

	switch (command)
	{
	case UNKNOW_COMMAND:
		cout << "Command not found\n";
		ret = 0;		break;
	case LOAD:
		CheckFormatCmd();
		ret=LoadFile();
		break;
	case SAVE:
		CheckFormatCmd();
		ret = WriteFile();
		break;
	case BLUR:
		CheckFormatCmd();
		ret = Blur();
		break;
	case RESIZE:
		CheckFormatCmd();
		ret = ReSize();
		break;
	case HELP:
		CheckFormatCmd();
		ret =Help();
		break;
	case EXIT:
		return -1;
	default:
		cout << "Command not found\n";	
		break;

	}
	return ret;
}

//---------------------------------------------------------------------------------------------
int   DispatchCmd::WriteFile()
{
	if (bufName != NULL)
		free(bufName);
	bufName = (char*)malloc(lenImgDest + 1);
	memset(bufName, 0, lenImgDest + 1);
	std::copy(cmmd + indImgDest, cmmd + indImgDest + lenImgDest, bufName);

	if (bufNameSrc != NULL)
		free(bufNameSrc);
	bufNameSrc = (char*)malloc(lenImgSrc + 1);
	memset(bufNameSrc, 0, lenImgSrc + 1);
	std::copy(cmmd + indImgSrc, cmmd + indImgSrc + lenImgSrc, bufNameSrc);
		//int ret = CheckName(bufNameSrc);
		//if (ret < 0)		ArgList.push_back(bufNameSrc);
	Mat image;
	if (!imagesOrig.count(String(bufNameSrc)))
	{
		cout << "Error: No image in memory" << std::endl;
		return -1;
	}
	else
		image = imagesOrig[String(bufNameSrc)];
	cout << "Write images ..." << endl;
	bool result = false;
	vector<int> compression_params;
	compression_params.push_back(IMWRITE_JPEG_QUALITY);
	compression_params.push_back(30);  //0..100

	try
	{
		result = imwrite(bufName,image, compression_params);
	}
	catch (exception& error_)
	{
		cout << "Caught exception: " << error_.what() << endl;
		cout << "Error: the image has been incorrectly writed." << std::endl;
		return -1;
	}
}
//---------------------------------------------------------------------------------------------
int   DispatchCmd::LoadFile()
{
	if (bufName != NULL)
			free(bufName);
		bufName = (char*)malloc(lenImgDest + 1);
		memset(bufName, 0, lenImgDest + 1);
		std::copy(cmmd + indImgDest, cmmd + indImgDest + lenImgDest, bufName);

		std::vector<char> myvector(lenImgSrc);
		if (bufNameSrc != NULL)
				free(bufNameSrc);
		bufNameSrc = (char*)malloc(lenImgSrc + 1);
		memset(bufNameSrc, 0, lenImgSrc + 1);
		std::copy(cmmd + indImgSrc, cmmd + indImgSrc + lenImgSrc, bufNameSrc);
	

		cout << "Read images ..." << endl;
		try
		{
	
			Mat image = imread(bufName, CV_LOAD_IMAGE_COLOR);
				// We check that our image has been correctly loaded
			if (image.empty()) {
				std::cout << "Error: the image has been incorrectly loaded." << std::endl;
				return -1;
			}
			if (!imagesOrig.count(String(bufNameSrc)))
				imagesOrig[String(bufNameSrc)] = image.clone();
	
			
			namedWindow("Original image", CV_WINDOW_AUTOSIZE);
			imshow("Original image", image);
				// Finally, we display our image and ask the program to wait for a key to be pressed
				//imshow("My first OpenCV window", image);
			waitKey(0);
			
			destroyAllWindows();

		}
		catch (exception& error_)
		{
			cout << "Caught exception: " << error_.what() << endl;
			return -1;
		}

	return 0;
}

//---------------------------------------------------------------------------------------------
int   DispatchCmd::Blur()
{

	if (bufName != NULL)
		free(bufName);
	bufName = (char*)malloc(lenImgDest + 1);
	memset(bufName, 0, lenImgDest + 1);
	
	std::copy(cmmd + indImgDest, cmmd + indImgSrc + lenImgDest, bufName);

	std::vector<char> myvector(lenImgSrc);
	if (bufNameSrc != NULL)
		free(bufNameSrc);
	bufNameSrc = (char*)malloc(lenImgSrc + 1);
	memset(bufNameSrc, 0, lenImgSrc + 1);
	std::copy(cmmd + indImgSrc, cmmd + indImgSrc + lenImgSrc, bufNameSrc);

	
	Mat image;
	if (!imagesOrig.count(String(bufNameSrc)))
	{
		cout << "Error: No image in memory" << std::endl;
		return -1;
	}
	else
		image = imagesOrig[String(bufNameSrc)];
	cout << " Gauss blur image ..." << endl;
	try
	{
		if (image.empty()) {
			std::cout << "Error: the image has been incorrectly loaded." << std::endl;
	
			return -1;
		}
		Mat blurredImage;
		// Apply the Gaussian Blur filter.
		// The Size object determines the size of the filter (the "range" of the blur)
		GaussianBlur(image, blurredImage, Size(lenW, lenW), 1.0);
		imagesOrig[String(bufName)]= blurredImage;
		// Show the blurred image in a named window
		imshow("Blurred Image", blurredImage);

		waitKey(0);
		destroyAllWindows();
	}
	catch (exception& error_)
	{
		cout << "Caught exception: " << error_.what() << endl;
		return -1;
	}

	return 0;
}
//---------------------------------------------------------------------------------------------
int   DispatchCmd::ReSize()
{

	if (bufName != NULL)
		free(bufName);
	bufName = (char*)malloc(lenImgDest + 1);
	memset(bufName, 0, lenImgDest + 1);
	std::copy(cmmd + indImgDest, cmmd + indImgDest + lenImgDest, bufName);

	std::vector<char> myvector(lenImgSrc);
	if (bufNameSrc != NULL)
		free(bufNameSrc);
	bufNameSrc = (char*)malloc(lenImgSrc + 1);
	memset(bufNameSrc, 0, lenImgSrc + 1);
	std::copy(cmmd + indImgSrc, cmmd + indImgSrc + lenImgSrc, bufNameSrc);

	Mat image;
	if (!imagesOrig.count(String(bufNameSrc)))
	{
		cout << "Error: No image in memory" << std::endl;
		return -1;
	}
	else
		image = imagesOrig[String(bufNameSrc)];
	
	try
	{
		if (image.empty()) {
			std::cout << "Error: the image has been incorrectly loaded." << std::endl;
			return -1;
		}

		resize(image,image, Size(lenW, lenH));
		imagesOrig[String(bufName)] = image;
		imshow("Resize image", image);

		waitKey(0);

		destroyAllWindows();

	}
	catch (exception& error_)
	{
		cout << "Caught exception: " << error_.what() << endl;
		return -1;
	}

	return 0;
}
//---------------------------------------------------------------------------------------------
int   DispatchCmd::Help()
{
	setlocale(LC_CTYPE, "rus");
	cout << "**********************************  Помощь  ****************************************\n";
	cout << "Команды\n";
	cout << "---------------------------------------------------\n";
	cout << "load, ld\n";
	cout << "Параметры\n";
	cout << "<name> -имя изображения, по имени оно доступно в других командах <filename> -имя файла для загрузки\n";
	cout << L"Действие\n";
	cout << "Загрузка изображения <filename> в формате JPG. Файл должен быть в папке exe-файла\n";
	cout << "---------------------------------------------------\n";
	cout << "store, s\n";
	cout << "Параметры\n";
	cout << "< name > -имя изображения	<filename> -имя файла для сохранениея\n";
	cout << "Действие\n";
	cout << "Сохранение  изображения  в формате JPG\n";
	cout << "---------------------------------------------------\n";
	cout << "blur\n";
	cout << "Параметры\n";
	cout << "<from_name> - имя изображения	< to_name > -имя изображения <size> -размер сглаживаемой области\n";
	cout << "Действие\n";
	cout << "Сглаживание изображение\n";
	cout << "---------------------------------------------------\n";
	cout << "resize\n";
	cout << "Параметры\n";
	cout << "< from_name > -имя изображения	<to_name> -имя изображения	<new_width>	< new_height>\n";
	cout << "Действие\n";
	cout << "Изменение размера изображения\n";
	cout << "---------------------------------------------------\n";
	cout << "help,h\n";
	cout << "Параметры\n";
	cout << "нет\n";
	cout << "Действие\n";
	cout << "Отображение этой помощи\n";
	cout << "---------------------------------------------------\n";
	cout << "exit, quit, q\n";
	cout << "Параметры\n";
	cout << "нет\n";
	cout << "Действие\n";
	cout << "Выход из программы\n";
	cout << "---------------------------------------------------\n";
	cout << "Замечание\n";
	cout << "Все параметры должны разделяться пробелами\n";
	cout << "Пример сессии :\n";
	cout << "ld img1 image.jpg\n";
	cout << "blur img1 img1b 5\n";
	cout << "s img1b image2.jpg\n";
	cout << "q\n";



	return 0;
}


