# KShare
A [ShareX](https://github.com/ShareX/) clone written in Qt. Should be cross platform

**Note:** Windows users, stick to ShareX.

## Dependencies
* Qt 5 Widgets
* Qt 5 GUI
* Qt 5 Network
* [QHotkey](https://github.com/Skycoder42/QHotkey)

Despite the name implying so, this project does not depend on the KDE API at all.

## Goals
* Same support for Windows, Linux and Mac (if I ever get testers)
* Screenshotting: [done]
* 1. Fullscreen, [target: 1.0] [done]
* 2. Area; [target: 1.0] [done]
* Screen recording, same options as above: [target: 2.0]
* 1. WebM
* 2. GIF (nopls) 
* Custom uploader support [target: 1.0] [done]
* Default uploaders, including:
* 1. imgur [target: 1.0] [done]
* 2. Clipboard (not an uploader) [target: 1.0] [done]
* 3. (S)FTP [target: 2.0]
* Oh, and a good icon. [looks good to me, but to noone else]

## Wayland Support

If it's requested enough, I will make a pull request towards QHotkey to support Wayland. Or open an issue, depending on my mood. But Qt _should_ still work on Wayland.

###### Started on 19th of April 2017 to bring some attention and improvement to Linux screenshotting.
