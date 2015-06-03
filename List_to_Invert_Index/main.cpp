#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <windows.h>
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
	string list_path = "./List_Multi_File/";
	
	fstream list_file, index_file, log_file;
	string file_name, index_path, log_path = "./List_to_Invert_index_Log.txt";

	char buf[1024];
	string keyword, max_keyword, max_file_ID, max_number_keyword;
	int max_length = 0, temp_length, keyword_number, temp_number, max_file_number = 0;

	map < string, vector<int> > invert_index_map;
	map < string, vector<int> >::iterator it;
	int file_ID;

	log_file.open(log_path, ios::out);
	if (!log_file)
		cerr << "Error: can not create log file!" << endl;

	dp = opendir(list_path.c_str()); // for each file f, create a list f_bar of unique keyword
	if (dp != NULL)
	{
		index_path = "./Index/Invert_Index.idx";
		//cout << index_path << endl;
		index_file.open(index_path, ios::out | ios::binary);
		if (!index_file)
		{
			cerr << "Error: open " << index_path << " failed..." << endl << endl;
			log_file << "Error: open " << index_path << " failed..." << endl << endl;
		}
		else
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
					file_ID = atoi(file_name.c_str());
					//cout << file_ID << endl;
					while (list_file.getline(buf, sizeof(buf)))
					{
						keyword_number++;
						keyword.assign(buf);
						//cout << keyword << endl;
						temp_length = keyword.size();
						if (temp_length > max_length)
						{
							max_length = keyword.size();
							max_keyword = keyword;
							max_file_ID = file_name;
						}
						memset(buf, 0, sizeof(buf));
						invert_index_map[keyword].push_back(file_ID);

					}
					//cout << "          File ID: " << file_name << endl; 
					//cout << "Number of keyword: " << keyword_number << endl << endl;
					log_file << "          File ID: " << file_name << ", ";
					log_file << "Number of keyword: " << keyword_number << endl << endl;

					list_file.close();
				}
			}

			for (it = invert_index_map.begin(); it != invert_index_map.end(); it++)
			{
				//cout << it->first << ":";
				index_file << it->first << ":";
				keyword = it->first;
				temp_number = invert_index_map[keyword].size();
				if (temp_number > max_file_number)
				{
					max_file_number = temp_number;
					max_number_keyword.assign(keyword);
				}
				for (int i = 0; i < temp_number; i++)
				{
					//cout << invert_index_map[keyword][i] << " ";
					index_file << invert_index_map[keyword][i] << " ";
				}
				//cout <<endl;
				index_file <<endl;
			}

			index_file.close();
		}
	}
	
	cout << "The maximum length of keyword: " << max_length << " in file ID: " << max_file_ID << endl;
	cout << "The maximum keyowrd: " << max_keyword << endl;
	log_file << "The maximum length of keyword: " << max_length << " in file ID: " << max_file_ID << endl;
	log_file << "The maximum keyword: " << max_keyword << endl;
	/* Program to Timing */
	QueryPerformanceCounter(&endTime); // 取得開機到程式執行完成經過幾個CPU Cycle
	times = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;

	cout << "The maximum number of file including a keyword: " << max_file_number << ", the keyword is: " << max_number_keyword << endl;
	log_file << "The maximum number of file including a keyword: " << max_file_number << ", the keyword is: " << max_number_keyword << endl;
	cout << fixed << setprecision(3) << "Processing time: " << times << 's' << endl << endl;
	log_file << fixed << setprecision(3) << "Processing time: " << times << 's' << endl << endl;
	log_file.close();
	
	system("PAUSE");
	return 0;
}