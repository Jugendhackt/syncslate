# Wiki

## Our guidelines
### Our api's
* We use Qt to make our applications compatible to Windows, MacOs and Linux.
* We use FFMpeg to cut the videos, make timelapses, add transitions and render audio.

### Our design
* We want to have a modern, beautiful design so we :
    * Reimplemented all Buttons to have a framless window
    * Have a dark background
    * Have Flat Buttons with no border :D
    * Have many symbols
* We want our appliaction can be understand by everyone after a short time

## How to install

### Using Visual Studio 2017 (VS2017)
#### Qt
##### Download Qt

First to support Ui you need to download and install Qt if you haven't already done that. 
You can download Qt [here](https://www.qt.io/download-thank-you?hsLang=en)

##### Install Qt

Open the Installer and folow the guide till you can select versions. Select version 5.10.1.

(images comes soon...)

Make sure you have only selected VS2017 tools, you don't need the other versions for VS2015 and VS2013. So you don't have to download about 60gib, just about 20gib.

#### Visual Studio
##### Download VS2017

After installing Qt (Or before, it's up to you), you have to download Visual Studio 2017 from the Offical Microsoft Site: [here](https://visualstudio.microsoft.com/de/thank-you-downloading-visual-studio/?sku=Community&rel=15) or for Mac [here](https://visualstudio.microsoft.com/de/thank-you-downloading-visual-studio-mac/?sku=communitymac&rel=15). 

If you run linux, I'm sorry but you have to get another device that has a supported OS, or you can run a free Windows 10 VM, isos are enough in the internet. Wine is not working, but Linux installers will come soon, and also Windows and mac installers. If they're out pleade follow the section "Install SyncSlate using Installers".

##### Install VS2017

Run the Installer and folow the instructions on the screen,

#### Connect Qt and VS2017
##### Install Qt Plugin in Visual Studio

Open the Extension Manager, select "Online" and search for Qt. Install it and folow the instructions on the screen.

(Images Come Soon)

##### Tell the Extension where to find Qt.

At the main Menu you can see a new section "Qt VS Tools"
