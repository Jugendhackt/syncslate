// ConsoleApplication1.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

/*#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector> */

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

void FfmCut(vector<vector<string>> markers, string path , string out_path, int mode, bool intro , bool outtro, string intro_path, string outtro_path){
	string cmd = "ffmpeg -y";
	int count_vids = 0;
	if (mode == 0 | mode == 3) {

		cmd += " -nostats -loglevel 0";

	}
	cmd += " -i \"" + path + "\" ";

	if (intro) {
		cmd += " -i \"" + intro_path + "\" ";
		count_vids++;
	}
	if (outtro) {
		cmd += " -i \"" + outtro_path + "\" ";
		count_vids++;
	}

	cmd += " -filter_complex \"";

	vector<string> keystone_one_h_m_s;
	int count = 0;
	int count_vids_used = 1;

	for (int i = 0; i < markers.size(); i++) { // 1. Marker Rausfiltern
		if (markers.at(i).size() != 0) {
			if (strstr(markers.at(i).at(1).c_str(), "1") != NULL) { keystone_one_h_m_s.push_back(markers.at(i).at(0)); }
		}
	}

	vector<string> keystone_one;

	for (int i = 0; i < keystone_one_h_m_s.size(); i++) {
		vector<string> hms = explode(keystone_one_h_m_s.at(i), ':');

		int h = stoi(hms.at(0));
		int m = stoi(hms.at(1));
		int s = stoi(hms.at(2));

		int s_ges = s + m * 60 + h * 3600;

		keystone_one.push_back(to_string(s_ges));

	}

	if (keystone_one.size() % 2 == 0) {

		for (int x = 0; x < keystone_one.size(); x += 2) {
			
			if (count > 0) {cmd += ";";}
			count++;

			cmd += "[0:v]trim=start=" + keystone_one.at(x) + ":end=" + keystone_one.at(x + 1) + ",setpts=PTS-STARTPTS[v" + to_string(count) + "]";
			cmd += ";[0:a]atrim=start=" + keystone_one.at(x) + ":end=" + keystone_one.at(x + 1) + ",asetpts=PTS-STARTPTS[a" + to_string(count) + "]";

			if (count >= 2) {

				cmd += ";[v" + to_string(count - 1) + "][v" + to_string(count) + "]concat[v" + to_string(count + 1) + "]";
				cmd += ";[a" + to_string(count - 1) + "][a" + to_string(count) + "]concat=v=0:a=1[a" + to_string(count + 1) + "]";
				count++;
			}

		}

		if (intro) {
			cmd += ";[" + to_string(count_vids_used) + ":v][v" + to_string(count) + "]concat[v" + to_string(count + 1) + "]";
			cmd += ";[" + to_string(count_vids_used) + ":a][a" + to_string(count) + "]concat=v=0:a=1[a" + to_string(count + 1) + "]";
			count++;
			count_vids_used++;
		}
		if (outtro) {
			cmd += ";[" + to_string(count_vids_used) + ":v][v" + to_string(count) + "]concat[v" + to_string(count + 1) + "]";
			cmd += ";[" + to_string(count_vids_used) + ":a][a" + to_string(count) + "]concat=v=0:a=1[a" + to_string(count + 1) + "]";
			count++;
		}

		cmd += "\" -map [v"+ to_string(count)+"] -map [a"+to_string(count)+"] \""+out_path+"\"";

		system(cmd.c_str());

	}

	return cmd;
	
}

void backend(string pathtomarkers, string pathtovideo, string pathtoout, bool intro, bool outtro, string intro_path, bool outtro_path)
{
	
	ifstream file;
	string filename = pathtomarkers;
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

		
	}
	
	
	FfmCut(markers, pathtovideo, pathtoout, mode, intro, outtro, intro_path, outtro_path);

	return 0;
}