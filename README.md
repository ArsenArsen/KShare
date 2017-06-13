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
Currently, the only package I provide is `kshare-git` on the AUR.
I do plan to make a Debian and Ubuntu packages, as well as `kshare` stable for Arch.

## Wayland Support
If it's requested enough, I will make a pull request towards QHotkey to support Wayland. Or open an issue, depending on my mood. But Qt _should_ still work on Wayland.

###### Started on 19th of April 2017 to bring some attention and improvement to Linux screenshotting.
