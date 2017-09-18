---
title: Encoder Settings
categories: [wiki]
layout: wikipage
---
# Encoder settings
The encoders used can be customized to a high degree. Every encoder besides GIF has some settings.
![](http://i.imgur.com/O8nfeos.png)

## Image encoder settings
The only option here is quality, by default it uses the default for the format you use. You can change it by unticking format default. [More info](http://doc.qt.io/qt-5/qpixmap.html#save)

## Video encoder settings
The common settings are bitrate and GOP size. 

GOP Size is the size of a group of pictures.  
Bitrate is the target bitrate.

### h264/h265
The H.26[45] MP4 codec has a few presets to choose. The recommended is to choose the slowest one you can cope with.  
I recommend medium.

CRF is the constant rate factor of this video stream, and again I recommend leaving the default value, `23`.

### VP9
VP9 is the WebP codec. Only option it has is lossless encoding, and you want it off, trust me.

### GIF
GIF has no options. It's terrible whatever you do to it.
