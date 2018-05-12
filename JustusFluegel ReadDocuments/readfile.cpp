#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const vector<string> explode(const string& s, const char& c)
{
	string buff{ "" };
	vector<string> v;

	for (auto n : s)
	{
		if (n != c) buff += n; else
			if (n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if (buff != "") v.push_back(buff);

	return v;
}

int main()
{

	ifstream file;
	string filename = "D:\\VideoDateien(Roh,Blutig,Arbeitsverzeichnis)\\marks.txt";
	char zeile[1024];

	vector<vector<string>> markers;
	vector<int> containsstart;


	file.open(filename, ios::in);
	if (file.good()) {

		file.seekg(0L, ios::beg);

		for (int counter = 0; !file.eof(); counter++)
		{
			file.getline(zeile, 1024);
			if (strstr(zeile, "START") != NULL) {

				containsstart.push_back(counter);

			}

		}
		//system("ffmpeg -v");
	}
	file.close();
	int laststart = containsstart.at(containsstart.size() - 1);

	file.open(filename, ios::in);
	if (file.good()) {

		file.seekg(0L, ios::beg);

		for (int i = 0; i < laststart + 1; i++) {
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		while (!file.eof())
		{
			file.getline(zeile, 1024);
			if (strstr(zeile, "STOP") == NULL) {
				vector<string> line = explode(zeile, '-');

				markers.push_back(line);
			}

		}

		//system("ffmpeg -v");
	}
	else {

		//cout << "Datei nicht gefunden." << endl;

	}

	for (int i = 0; i < markers.size(); i++) {
		for (int x = 0; x < markers.at(i).size(); x++) {

			if (x + 1 != markers.at(i).size()) {
				//cout << markers.at(i).at(x) + " | ";
			}
			else {
				//cout << markers.at(i).at(x);
			}

		}
		//cout << "\n";
	}

	system("pause >nul");

	return 0;
}