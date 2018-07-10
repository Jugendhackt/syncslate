#include "Settings.h"
#include "ui_Settings.h"
#include "SyncSlate.h"
#include <qsettings.h>

using namespace std;

bool open = true;
int prozent;
float komma;
int fps; 

int valuemode = 0; // 0 = prozent; 1 = komma; 2 = fps;

Settings::Settings(QWidget *parent) :
	QMainWindow(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint), ui(new Ui::SettingsClass)
{
	ui->setupUi(this);

	QSettings settings;
	ui->BezeichnerTimestampSchitt->setText(settings.value("settings/schnittmarkerindentificationname", "Timestamp 1").toString());
	ui->BezeichnerTimestampZeitraffer->setText(settings.value("settings/zeitraffermarkerindentificationname", "Timestamp 2").toString());
	ui->ZeitrafferSpeed->setInputMask("9999");
	ui->fps_out->setInputMask("9999");
	ui->ZeitrafferSpeed->setText(settings.value("settings/zeitrafferspeed", 200).toString());

	close_fps_popup();

	int x_pos = settings.value("x_pos", NULL).toInt();
	int y_pos = settings.value("y_pos", NULL).toInt();

	if(x_pos != NULL && y_pos != NULL)
		move(x_pos,y_pos);

	prozent = stoi(ui->fps_out->text().toStdString());
	komma = prozent / 100;
	fps = 30 * komma;

}

Settings::~Settings() {

}
void Settings::exit() {

	QApplication::quit();

}
void Settings::minimize() {

	Settings::showMinimized();
}
void Settings::closeForm() {

	QSettings settings; 

	settings.setValue("settings/schnittmarkerindentificationname", ui->BezeichnerTimestampSchitt->text());
	settings.setValue("settings/zeitraffermarkerindentificationname", ui->BezeichnerTimestampZeitraffer->text());
	settings.setValue("settings/zeitrafferspeed", stoi(ui->ZeitrafferSpeed->text().toStdString()));

	SyncSlate *syncslate = new SyncSlate();
	syncslate->show();

	this->close();

}
void Settings::open_fps_popup() {

	ui->fps_dropdown_fps->setVisible(true);
	ui->fps_dropdown_komma->setVisible(true);
	ui->fps_dropdown_prozent->setVisible(true);
	open = true;

}
void Settings::close_fps_popup() {

	ui->fps_dropdown_fps->setVisible(false);
	ui->fps_dropdown_komma->setVisible(false);
	ui->fps_dropdown_prozent->setVisible(false);
	open = false;

}
void Settings::change_fps_popup() {

	if (open) {
		close_fps_popup();
	}
	else {
		open_fps_popup();
	}

}
void Settings::fps_popup_komma() {
	close_fps_popup();
	ui->SelectMode_fps->setText("0,x");
	ui->fps_out->setText(QString::fromStdString(to_string(komma)));
	valuemode = 1;
}
void Settings::fps_popup_prozent() {
	close_fps_popup();
	ui->SelectMode_fps->setText("%");
	ui->fps_out->setText(QString::fromStdString(to_string(prozent)));
	valuemode = 0;
}
void Settings::fps_popup_fps() {
	close_fps_popup();
	ui->SelectMode_fps->setText("fps");
	ui->fps_out->setText(QString::fromStdString(to_string(fps)));
	valuemode = 2;
}
void Settings::fps_out_updated() {
	if (valuemode == 0) {
		prozent = stoi(ui->fps_out->text().toStdString());
		komma = (float)prozent / 100;
		fps = 30 * komma;
	}
	else if (valuemode == 1) {
		komma = stof(ui->fps_out->text().toStdString());
		prozent = komma * 100;
		fps = 30 * komma;
		ui->fps_out->setText(QString::fromStdString(to_string(komma)));
	}
	else if(valuemode == 2){
		fps = stoi(ui->fps_out->text().toStdString());
		komma = (float)fps / 30;
		prozent = komma * 100;
	}

	//system(("echo \"Komma:" + to_string(komma) + "\"& echo \"Prozent:" + to_string(prozent) + "\"& echo \"Framerate:" + to_string(fps) + "\" & pause").c_str());

}