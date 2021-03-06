#include "SyncSlate.h"
#include "ui_SyncSlate.h"
#include "Settings.h"
#include <fstream>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QFileInfo>
#include <vector>
#include <qsettings.h>
#include <qdesktopservices.h>

extern int x_pos;
extern int y_pos;

using namespace std;

QString video = "";
QString timestamp = "";
QString save = "";
QString save_path = "";

bool stampsselected = false;
bool vidselected = false;

double getSpeedMultiplier() {

	QSettings settings;
	double prozent = settings.value("settings/zeitrafferspeed", 200).toDouble();
	return 1 / (prozent / 100);

}

SyncSlate::SyncSlate(QWidget *parent) :
	QMainWindow(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint), ui(new Ui::SyncSlateClass)
{
	ui->setupUi(this);

	QCoreApplication::setOrganizationName("JugendHackt");
	QCoreApplication::setApplicationName("SyncSlate");

	QSettings settings;

	int x_pos = settings.value("x_pos", NULL).toInt();
	int y_pos = settings.value("y_pos", NULL).toInt();

	if (x_pos != NULL && y_pos != NULL)
		move(x_pos, y_pos);

}

SyncSlate::~SyncSlate() {

}

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

void FfmCut(vector<vector<string>> markers, string path, string out_path, bool intro, bool outtro, string intro_path, string outtro_path) {
	string cmd = "ffmpeg -y";
	int count_vids = 0;
	cmd += " -nostats -loglevel 0 -i \"" + path + "\" ";

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
	vector<string> keystone_two_h_m_s;
	int count = 0;
	int count_vids_used = 1;

	QSettings settings;

	string Splitter1 = settings.value("settings/schnittmarkerindentificationname", "Timestamp 1").toString().toStdString();
	string Splitter2 = settings.value("settings/zeitraffermarkerindentificationname", "Timestamp 2").toString().toStdString();


	for (int i = 0; i < markers.size(); i++) { // 1. Marker Rausfiltern
		if (markers.at(i).size() != 0) {
			if (strstr(markers.at(i).at(1).c_str(),Splitter1.c_str()) != NULL) { keystone_one_h_m_s.push_back(markers.at(i).at(0)); }
		}
	}

	for (int i = 0; i < markers.size(); i++) { // 1. Marker Rausfiltern
		if (markers.at(i).size() != 0) {
			if (strstr(markers.at(i).at(1).c_str(), Splitter2.c_str()) != NULL) { keystone_two_h_m_s.push_back(markers.at(i).at(0)); }
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

	vector<string> keystone_two;

	for (int i = 0; i < keystone_two_h_m_s.size(); i++) {
		vector<string> hms = explode(keystone_two_h_m_s.at(i), ':');

		int h = stoi(hms.at(0));
		int m = stoi(hms.at(1));
		int s = stoi(hms.at(2));

		int s_ges = s + m * 60 + h * 3600;

		keystone_two.push_back(to_string(s_ges));

	}

	if (keystone_one.size() % 2 == 0) {

		for (int x = 0; x < keystone_one.size(); x += 2) {

			if (count > 0) { cmd += ";"; }
			count++;

			cmd += "[0:v]trim=start=" + keystone_one.at(x) + ":end=" + keystone_one.at(x + 1) + ",setpts=PTS-STARTPTS[v" + to_string(count) + "]";
			cmd += ";[0:a]atrim=start=" + keystone_one.at(x) + ":end=" + keystone_one.at(x + 1) + ",asetpts=PTS-STARTPTS[a" + to_string(count) + "]";

			if (count > 1) {

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

		cmd += "\" -map [v" + to_string(count) + "] -map [a" + to_string(count) + "] \"" + out_path + "\"";

		system(("echo Das Video wird geschnitten ... &" + cmd).c_str());

		QMessageBox msgBox;
		msgBox.setWindowTitle("Schnitt abgeschlossen!");
		msgBox.setText("Fertig!\nMöchten sie die Anwendung schließen?");
		msgBox.setStandardButtons(QMessageBox::Yes);
		msgBox.addButton(QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		if (msgBox.exec() == QMessageBox::Yes) {
			QApplication::quit();
		}
		else {
			msgBox.close();
		}
	}

}

void backend(string pathtomarkers, string pathtovideo, string pathtoout, bool intro, bool outtro, string intro_path, string outtro_path)
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


	FfmCut(markers, pathtovideo, pathtoout, intro, outtro, intro_path, outtro_path);
}

void SyncSlate::onTimestamp()
{
	qInfo("C Style Info Message");

	const QString TimestampFolder = QStandardPaths::writableLocation(QStandardPaths::TempLocation);

	QString timestamppath = QFileDialog::getOpenFileName(
		this,
		tr("Timestamp Datei öffnen..."),
		TimestampFolder,
		"Textdateien (*.txt);;Alle Dateien (*.*);;"
	);

	QFileInfo timestampinfo(timestamppath);

	QString filename = timestampinfo.fileName();

	timestamp = timestamppath;

	if (timestamppath != "") {

		ui->videoButton->setStyleSheet("QPushButton:hover { background-color: rgb(45, 120, 210)} QPushButton {background-color: rgb(23, 131, 225); color:white; padding : 18; border:none; border-radius: 7px}");
		stampsselected = true;
		ui->videoFormat->setStyleSheet("color:rgb(106, 111, 122);");
		ui->video_logo->setStyleSheet("background-image: url(\":/SyncSlate/Resources/video.png\"); background-position : center; background-repeat:no-repeat; ");
		ui->line_left->setStyleSheet("background-color:rgb(126, 132, 145);");
	}
}
void SyncSlate::on_videoButton_clicked()
{
	if (stampsselected) {
		qInfo("Video auswählen!");
		const QString videoFolder = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

		QString videopath = QFileDialog::getOpenFileName(
			this,
			tr("Video öffnen..."),
			videoFolder,
			"Alle Videodateien (*.*);;"
		);

		QFileInfo videoinfo(videopath);

		QString filename = videoinfo.fileName();

		video = videopath;

		if (videopath != "") {
			ui->runButton->setStyleSheet("QPushButton:hover { background-color: rgb(45, 120, 210)} QPushButton {background-color: rgb(23, 131, 225); color:white; padding : 18; border:none; border-radius: 7px}");
			vidselected = true;
			ui->export_logo->setStyleSheet("background-image: url(\":/SyncSlate/Resources/download.png\"); background-position : center; background-repeat:no-repeat;");
			ui->line_right->setStyleSheet("background-color:rgb(126, 132, 145);");
		}
	}
}
void SyncSlate::on_runButton_clicked()
{
	if (vidselected) {
		ifstream ifile(video.toStdString());
		ifstream file2(timestamp.toStdString());

		if ((bool)ifile && (bool)file2) {

			const QString videoFolder = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

			QString local_save_path = QFileDialog::getSaveFileName(
				this,
				tr("Speicherort öffnen..."),
				videoFolder,
				"Movie Datei (*.mov);;"
			);

			save_path = local_save_path;

			backend(timestamp.toStdString(), video.toStdString(), local_save_path.toStdString(), false, false, "", "");
		}
		else {
			QMessageBox::information(this, tr("Error!"), "Timestamps oder Video können nicht gelesen werden!");
		}
	}
}
void SyncSlate::exit() {

	QApplication::quit();

}
void SyncSlate::minimize() {

	SyncSlate::showMinimized();
}
void SyncSlate::reloadUi() {

	ui->line_right->setStyleSheet("background-color :rgb(97, 101, 111);");
	ui->line_left->setStyleSheet("background-color :rgb(97, 101, 111);");
	ui->video_logo->setStyleSheet("background-image: url(\":/SyncSlate/Resources/videoGrey.png\"); background-position : center; background-repeat:no-repeat; ");
	ui->export_logo->setStyleSheet("background-image: url(\":/SyncSlate/Resources/downloadGrey.png\"); background-position : center; background-repeat:no-repeat; ");
	ui->videoFormat->setStyleSheet("color:transparent");
	ui->videoButton->setStyleSheet("QPushButton {background-color:rgb(65, 69, 76);color:rgb(83, 87, 96); padding:18;border:none;border-radius: 7px}");
	ui->runButton->setStyleSheet("QPushButton {background-color:rgb(65, 69, 76);color:rgb(83, 87, 96); padding:18;border:none;border-radius: 7px}");

}
void SyncSlate::OpenSettings() {

	Settings *settings = new Settings();
	settings->show();

	this->close();

}