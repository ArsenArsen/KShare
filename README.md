# The entire project has been moved to GitLab. https://gitlab.com/ArsenArsen/KShare
# The current state of the project can be considered on hold for the time being. When I am back on working there will be a pretty big new release and a new wiki.

# KShare
A [ShareX](https://getsharex.com/) inspired cross platform utility written with Qt.

|Linux|Windows|OS X|
|:---:|:-----:|:--:|
|[![Build Status](https://nativeci.arsenarsen.com/job/KShare/badge/icon)](https://nativeci.arsenarsen.com/job/KShare)| [![Build Status](https://nativeci.arsenarsen.com/job/KShare%20Windows%20x86_64/badge/icon)](https://nativeci.arsenarsen.com/job/KShare%20Windows%20x86_64/)| Soon |
## Screenshot
Made with KShare itself, of course :)

![](http://i.imgur.com/ffWvCun.png)

## Usage
See the [wiki](https://github.com/ArsenArsen/KShare/wiki).

## Dependencies
* Qt 5 Widgets
* Qt 5 GUI
* Qt 5 Network
* Qt 5 X11Extras | Winextras
* [QHotkey](https://github.com/Skycoder42/QHotkey)
* libavformat
* libavcodec
* libavutil
* libswscale

Despite the name implying so, this project does not depend on the KDE API at all.

## Goals
See the [projects](https://github.com/ArsenArsen/KShare/projects)

## Install

### **NOTE:** These links are temporarily down and will be fixed with the next release. Current (potentially unstable) development builds are available at [this location](https://nativeci.arsenarsen.com/job/KShare%20(dev))

|Distro|Link|
|:----:|:--:|
|Arch Linux (development)|[kshare-git](https://aur.archlinux.org/packages/kshare-git/)|
|Ubuntu (development)|[Ubuntu .deb](https://nativeci.arsenarsen.com/job/KShare/lastSuccessfulBuild/artifact/packages/simpleName.deb)|
|Arch Linux |[kshare](https://aur.archlinux.org/packages/kshare/)|
|Ubuntu |[Ubuntu .deb](https://nativeci.arsenarsen.com/job/KShare%20Stable/lastSuccessfulBuild/artifact/packages/simpleName.deb)|

I do plan to make a Debian packages.

For other UNIX-like platforms, and MSYS2 (for Windows):

You have to obtain the dependencies though.
```bash
git clone https://github.com/ArsenArsen/KShare.git
cd KShare
qmake // Might be qmake-qt5 on your system
make
```

On systems with FFMpeg pre-3.1 you need to apply `OlderSystemFix.patch` to `recording/encoders/encoder.cpp`.
On systems with Qt pre-5.7 you need to install the Qt version from their website.

## Support (and contact)
There's a few ways to get support, you can:
* join `#kshare` on [Freenode](https://webchat.freenode.net/?channels=kshare)
* open an issue

If someone can answer, they probably will.

###### Started on 19th of April 2017 to bring some attention and improvement to Linux screenshotting.
