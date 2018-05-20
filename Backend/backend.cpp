// ConsoleApplication1.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <experimental/filesystem> //To get the temp Folder

//Create namespace for filesystem
namespace fs = std::experimental::filesystem::v1;

using namespace std;

//Explode function (Works same the PHP explode(); function with one letter)
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

//The FFmCut function to generate the FFmpeg command
string FFmCut(vector<vector<string>> markers, string path, string out_path, int mode, bool intro, bool outtro, string intro_path, string outtro_path) {
	
	//Set tmp path to string temp
	string temp = fs::temp_directory_path().string();

	//Create Code for Intro/Outtro
	#pragma region Intro/Outtro

	string cmd = "ffmpeg -y";
	int count_vids = 0;
	#pragma warning(push)
	#pragma warning(disable : 4554)
	if (mode == 0 || mode == 3) {
		cmd += " -nostats -loglevel 0";
	}
	#pragma warning(pop)

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
	#pragma endregion

	//Create keystone variables in h:mm:ss format
	#pragma region Create variables
	vector<string> keystone_one_h_m_s;
	vector<string> keystone_two_h_m_s;
	#pragma endregion

	//Create counter variables
	#pragma region Create variables
	int count = 0;
	int count_vids_used = 1;
	#pragma endregion

	//Filter keystones
	#pragma region Filter keystones
	#pragma warning(push)
	#pragma warning(disable : 4018)
	for (int i = 0; i < markers.size(); i++) { 
		if (markers.at(i).size() != 0) {
			//Filter keystones with String "1"
			if (strstr(markers.at(i).at(1).c_str(), "1") != NULL) { keystone_one_h_m_s.push_back(markers.at(i).at(0)); }
			//Filter keystones with String "2"
			if (strstr(markers.at(i).at(1).c_str(), "2") != NULL) { keystone_two_h_m_s.push_back(markers.at(i).at(0)); }
		}
	}
	#pragma warning(pop)
	#pragma endregion

	//Create keystone variables in seconds
	#pragma region Create variables
	vector<string> keystone_one;
	vector<string> keystone_two;
	#pragma endregion
	

	//Convert keystones in h:mm:ss format to seconds
	#pragma region Convert keystones
	#pragma warning(push)
	#pragma warning(disable : 4018)
	for (int i = 0; i < keystone_one_h_m_s.size(); i++) {
		
		//Convert Keystones with the "1" String in h:mm:ss Format to seconds
		if (!keystone_one_h_m_s.size() == 0) {
			vector<string> hms1 = explode(keystone_one_h_m_s.at(i), ':');
			int s_ges1 = stoi(hms1.at(2)) + stoi(hms1.at(1)) * 60 + stoi(hms1.at(0)) * 3600;

			keystone_one.push_back(to_string(s_ges1));
		}

		//Convert Keystones with the "2" String in h:mm:ss Format to seconds
		if (!keystone_two_h_m_s.size() == 0) {
			vector<string> hms2 = explode(keystone_two_h_m_s.at(i), ':');
			int s_ges2 = stoi(hms2.at(2)) + stoi(hms2.at(1)) * 60 + stoi(hms2.at(0)) * 3600;

			keystone_two.push_back(to_string(s_ges2));
		}
	}
	#pragma warning(pop)
	#pragma endregion

	//Generate command for FFmpeg
	if (keystone_one.size() % 2 == 0 && keystone_one.size() != 0) { //Check if we have a correct count of keystones

		//go trough every second keystone
		#pragma region Go trough keystones
		#pragma warning(push)
		#pragma warning(disable : 4018)
		for (int x = 0; x < keystone_one.size(); x += 2) { 

			if (count > 0) { cmd += ";"; } //append every command a semicolon
			
			count++; //Count counter

			//Generate trim commands for FFmpeg
			#pragma region Trim
			cmd += "[0:v]trim=start=" + keystone_one.at(x) + ":end=" + keystone_one.at(x + 1) + ",setpts=PTS-STARTPTS[v" + to_string(count) + "]";
			cmd += ";[0:a]atrim=start=" + keystone_one.at(x) + ":end=" + keystone_one.at(x + 1) + ",asetpts=PTS-STARTPTS[a" + to_string(count) + "]";
			#pragma endregion

			//Concatenate some videos
			if (count >= 2) {

				cmd += ";[v" + to_string(count - 1) + "][v" + to_string(count) + "]concat[v" + to_string(count + 1) + "]";
				cmd += ";[a" + to_string(count - 1) + "][a" + to_string(count) + "]concat=v=0:a=1[a" + to_string(count + 1) + "]";
				count++;
			}

		}
		#pragma warning(pop)
		#pragma endregion

		//Add intro files
		if (intro) {
			cmd += ";[" + to_string(count_vids_used) + ":v][v" + to_string(count) + "]concat[v" + to_string(count + 1) + "]";
			cmd += ";[" + to_string(count_vids_used) + ":a][a" + to_string(count) + "]concat=v=0:a=1[a" + to_string(count + 1) + "]";
			count++;
			count_vids_used++;
		}
		
		//Add outtro files
		if (outtro) {
			cmd += ";[" + to_string(count_vids_used) + ":v][v" + to_string(count) + "]concat[v" + to_string(count + 1) + "]";
			cmd += ";[" + to_string(count_vids_used) + ":a][a" + to_string(count) + "]concat=v=0:a=1[a" + to_string(count + 1) + "]";
			count++;
		}

		//Complete FFmpeg command
		cmd += "\" -map [v" + to_string(count) + "] -map [a" + to_string(count) + "] \"" + out_path + "\"";

		//Run FFmpeg command
		system(cmd.c_str());

	}

	//Return the command for future uses
	return cmd;

}

//The main function (unnecessary comment, I know :D)
int main()
{
	//Select Language
	#pragma region Select language
	cout << "Select language: 0 = english, 1 = german \nLanguage:  ";
	int language = 0;
	cin >> language;
	#pragma endregion
	
	//Select run mode
	#pragma region Select mode
	cout << ((language == 1) ? "Modus auswählen: 0 = Standard; 1 = Debug; 3 = Command ausgeben (FFMPEG) \nModus:  " : "Select mode: 0 = standard; 1 = debug; 3 = show command (FFmpeg) \nMode:  ");
	int mode;
	cin >> mode;
	#pragma endregion

	//Create variables for paths and if a intro/outtro avaiable
	#pragma region Create variables
	string pathtomarkers = "";
	string pathtovideo = "";
	string pathtoout = "";

	bool intro = false;
	bool outtro = false;

	string intro_path = "";
	string outtro_path = "";
	#pragma endregion

	//Set previously created variables through the ui
	#pragma region Set variables
	cout << ((language == 1) ? "Pfad zu der Markerdatei eingeben:  ": "Enter path to keystone file:  ");
	cin >> pathtomarkers;

	cout << ((language == 1) ? "Jetzt bitte Pfad zu der Videodatei eingeben:  " : "Enter path to video file:  ") ;
	cin >> pathtovideo;
	cout << ((language == 1) ? "Jetzt bitte Ausgabepfad angeben (mit Dateinamen):  " : "Enter output path (and filename.mov):  ");
	cin >> pathtoout;
	#pragma endregion

	//Get input / check if input is valid for intro
	#pragma region Get intro
	bool valid = true;
	while (valid) {

		//Get input trough user
		#pragma region Create variables and get if we have an intro
		string input;
		cout << ((language == 1) ? "Hast du ein Intro? \n(ja/nein):  " : "Do you have an Intro? \n(yes/no):  ");
		cin >> input;
		#pragma endregion
		
		//Check if we have an yes or an no
		#pragma region Check user input
		if (input == "ja" || input == "j" || input == "yes" || input == "y") {
			valid = false;
			cout << ((language == 1) ? "Pfad zum Intro: \n" : "Path to intro:\n");
			cin >> intro_path;
			intro = true;
		}
		else if (input == "nein" || input == "n" || input == "no") {
			valid = false;
			intro = false;
		}
		#pragma endregion
	}
	valid = true;
	#pragma endregion

	//Get input / check if input is valid for outtro
	#pragma region Get outtro
	while (valid) {

		//Get input trough user
		#pragma region Create variables and get if we have an intro
		string input;
		cout << ((language == 1) ? "Hast du ein Outtro? \n(ja/nein):  " : "Do you have an outtro\n(yes/no):  ");
		(cin >> input).get();
		#pragma endregion

		//Check if we have an yes or an no
		#pragma region Check user input
		if (input == "ja" || input == "j" || input == "yes" || input == "y") {
			valid = false;
			cout << ((language == 1) ? "Pfad zum Outtro: \n": "Path to auttro:\n");
			cin >> outtro_path;
			outtro = true;
		}
		else if (input == "nein" || input == "n" || input == "no") {
			valid = false;
			outtro = false;
		}
		#pragma endregion
	}
	valid = true;
	#pragma endregion

	//Check mode, if we are in one of the standard modes, clear commandline
	if (mode == 0 || mode == 3) { system("cls"); }

	//Give out information for the user
	cout << ((language == 1) ? "Ihr Video wird zugeschnitten ......." : "Your video is cutting .......");

	//Create variables for file reading
	#pragma region Create variables
	ifstream file;
	string filename = pathtomarkers;
	char zeile[1024];

	vector<vector<string>> markers;
	vector<int> containsstart;
	#pragma endregion

	//Read fle and find out where are "START"'s in the file (lines)
	#pragma region Read file
	file.open(filename, ios::in);
	if (file.good()) {
		
		//Go to first character
		file.seekg(0L, ios::beg);

		//Read every line and start a counter
		#pragma region Read lines
		for (int counter = 0; !file.eof(); counter++)
		{
			//Get the line
			file.getline(zeile, 1024);

			//Check if line contains "START"
			if (strstr(zeile, "START") != NULL) {

				//If line contains "START" write line number into an vector
				containsstart.push_back(counter);

			}

		}	
		#pragma endregion

	}
	file.close();
	#pragma endregion
	
	//Get last line that contains "START"
	int laststart = containsstart.at(containsstart.size() - 1);

	//Open file again
	#pragma region Read file
	file.open(filename, ios::in);
	if (file.good()) {

		//Jump to first letter
		file.seekg(0L, ios::beg);

		//Jump over the lines before last "START"
		for (int i = 0; i < laststart + 1; i++) {
			file.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		//Read lines
		while (!file.eof())
		{
			//Write line in "zeile"
			file.getline(zeile, 1024);

			//Make sure the line does not contains "STOP"
			if (strstr(zeile, "STOP") == NULL) {
				
				//Write timestamp and identifier into an vector
				vector<string> line = explode(zeile, '-');

				//Push the vector into another vector with all timestamps/identifiers
				markers.push_back(line);
			}

		}

	}
	#pragma endregion

	//Run the FFmCut method to cut the video
	string cmd = FFmCut(markers, pathtovideo, pathtoout, mode, intro, outtro, intro_path, outtro_path);

	//Check mode and give outputs
	if (mode == 0 || mode == 3) { system("cls"); }

	//Close the programm
	cout << ((language == 1) ? "Enter zum Beenden \n": "Enter to close\n");
	
	//If "return FFmpeg command" is active, give out FFmpeg command
	if (mode == 3) {
		cout << "\n"<< ((language == 1) ? "FFmpeg Kommando:" : "FFmpeg command:" )<< "  \n\n\n\n" << cmd;
	}

	//Pause the program until user input and close it after that
	system("pause > nul");
	
	return 0;
}