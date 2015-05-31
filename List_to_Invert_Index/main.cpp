#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>

using namespace std;

int main()
{

	LARGE_INTEGER startTime, endTime, fre;
	double times;

	QueryPerformanceFrequency(&fre); // 取得CPU頻率
	QueryPerformanceCounter(&startTime); // 取得開機到現在經過幾個CPU Cycle
	/* Program to Timing */
	DIR *dp;
	struct dirent *ep;
	string list_path = "./List/";
	
	fstream list_file, index_file, log_file;
	string file_name, index_path, log_path = "./List_to_Invert_index_Log.txt";

	char buf[1024];
	string keyword, max_keyword, max_file_ID;
	int max_length = 0, keyword_number;

	log_file.open(log_path, ios::out);
	if (!log_file)
		cerr << "Error: can not create log file!" << endl;

	dp = opendir(list_path.c_str()); // for each file f, create a list f_bar of unique keyword
	if (dp != NULL)
	{

		readdir(dp); // .
		readdir(dp); // ..
		while (ep = readdir(dp)) // read the list file, the list file need to be UNIX format
		{
			//printf("%s\n", ep->d_name);
			file_name.assign(ep->d_name);

			list_path = "./List/";
			list_path.append(file_name);
			list_file.open(list_path, ios::in);
			if (!list_file)
				cerr << "Error: open: " << list_path << " failed..." << endl;
			else
			{
				memset(buf, 0, sizeof(buf));
				keyword_number = 0;
				file_name.erase(file_name.end() - 5, file_name.end());
				while (list_file.getline(buf, sizeof(buf)))
				{
					keyword_number++;
					keyword.assign(buf);
					//cout << keyword << endl;
					if (keyword.size() > max_length)
					{
						max_length = keyword.size();
						max_keyword = keyword;
						max_file_ID = file_name;
					}

					index_path = "./Index/" + keyword;
					//cout << index_path << endl;
					index_file.open(index_path, ios::app);
					if (!index_file)
					{
						cerr << "Error: open " << index_path << " failed..." << endl << endl;
						log_file << "Error: open " << index_path << " failed..." << endl << endl;
					}
					else
					{
						index_file << file_name << '\n';
						index_file.close();
					}
					memset(buf, 0, sizeof(buf));
				}
				//cout << "          File ID: " << file_name << endl; 
				//cout << "Number of keyword: " << keyword_number << endl << endl;
				log_file << "          File ID: " << file_name << endl;
				log_file << "Number of keyword: " << keyword_number << endl << endl;
				
				list_file.close();
			}
		}
	}
	
	cout << "The maximum length of keyword: " << max_length << " in file ID: " << max_file_ID << endl;
	cout << "The maximum keyowrd: " << max_keyword << endl;
	log_file << "The maximum length of keyword: " << max_length << " in file ID: " << max_file_ID << endl;
	log_file << "The maximum keyword: " << max_keyword << endl;
	/* Program to Timing */
	QueryPerformanceCounter(&endTime); // 取得開機到程式執行完成經過幾個CPU Cycle
	times = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;

	cout << fixed << setprecision(3) << "Processing time: " << times << 's' << endl << endl;
	log_file << fixed << setprecision(3) << "Processing time: " << times << 's' << endl << endl;
	log_file.close();
	
	system("PAUSE");
	return 0;
}