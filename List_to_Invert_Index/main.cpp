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

	string list_path = "./List/Forward_index.list";
	
	fstream list_file, index_file, log_file;
	string index_path, log_path = "./List_to_Invert_index_Log.txt";

	//char buf[1024];
	string buf;
	string keyword, max_keyword, max_number_keyword, temp_ID;
	int max_length = 0, temp_length, keyword_number, temp_number, max_file_number = 0;

	map < string, vector<int> > invert_index_map;
	map < string, vector<int> >::iterator it;
	int file_ID, max_file_ID;

	int start, end;

	QueryPerformanceFrequency(&fre); // 取得CPU頻率
	QueryPerformanceCounter(&startTime); // 取得開機到現在經過幾個CPU Cycle
	/* Program to Timing */
	cout << "Create log file: " << log_path << endl;
	log_file.open(log_path, ios::out);
	if (!log_file)
		cerr << "Error: can not create log file!" << endl;
	cout << "Open source forward index file: " << list_path << endl;
	list_file.open(list_path, ios::in);
	if (!list_file)
	{
		cerr << "Error: open forward index: " << list_path << " failed..." << endl;
		log_file << "Error: open forward index: " << list_path << " failed..." << endl;
	}
	else
	{
		index_path = "./Index/Invert_Index.idx";
		cout << "Target index file: " << index_path << endl;
		index_file.open(index_path, ios::out | ios::binary);
		if (!index_file)
		{
			cerr << "Error: open " << index_path << " failed..." << endl << endl;
			log_file << "Error: open " << index_path << " failed..." << endl << endl;
		}
		else
		{
			while (getline(list_file, buf))
			{
				keyword_number = 0;
				/* Find the file ID */
				for (int i = 0; i < buf.size(); i++)
				{
					if (buf[i] == ':')
					{
						temp_ID.assign(buf.c_str(), i); // i: the length of the keyword
						file_ID = atoi(temp_ID.c_str());
						//cout << "File ID: " << file_ID << endl;
						start = i + 1; // set the begining index of the file ID
						break;
					}
				}
				/* Find the file ID */

				/* Find the keyword */
				for (int i = start; i < buf.size(); i++)
				{
					if (buf[i] == 32) // 32 is space in ASCII
					{
						end = i;
						keyword.assign(&buf[start], end - start);
						keyword_number++;
						temp_length = keyword.size();
						if (temp_length > max_length)
						{
							max_length = keyword.size();
							max_keyword = keyword;
							max_file_ID = file_ID;
						}
						//cout << keyword << endl;
						start = end + 1;
						invert_index_map[keyword].push_back(file_ID);
						invert_index_map[keyword].shrink_to_fit();
						//cout << keyword << " " << file_ID << endl; // show the pairs
					}
				}

				//cout << "          File ID: " << file_ID << endl; 
				//cout << "Number of keyword: " << keyword_number << endl << endl;
				log_file << "          File ID: " << file_ID << ", ";
				log_file << "Number of keyword: " << keyword_number << endl << endl;
				/* Find the keyword */
			}
			list_file.close();

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
				index_file << endl;
			}
			index_file.close();
		}
	}
	/* Program to Timing */
	QueryPerformanceCounter(&endTime); // 取得開機到程式執行完成經過幾個CPU Cycle
	times = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;

	cout << endl;
	cout << "The maximum length of keyword: " << max_length << " in file ID: " << max_file_ID << endl;
	cout << "The maximum keyowrd: " << max_keyword << endl;
	log_file << "The maximum length of keyword: " << max_length << " in file ID: " << max_file_ID << endl;
	log_file << "The maximum keyword: " << max_keyword << endl;

	cout << "The maximum number of file including a keyword: " << max_file_number << ", the keyword is: " << max_number_keyword << endl;
	log_file << "The maximum number of file including a keyword: " << max_file_number << ", the keyword is: " << max_number_keyword << endl;
	cout << fixed << setprecision(3) << "Processing time: " << times << 's' << endl << endl;
	log_file << fixed << setprecision(3) << "Processing time: " << times << 's' << endl << endl;
	log_file.close();
	
	system("PAUSE");
	return 0;
}