#!/usr/bin/env bash
function addFile {
    cp $1 .
    echo "Source: \"$(basename $1)\"; DestDir: \"{app}\"; Flags: ignoreversion" >> installer.iss
}

function addAllFiles {
    (find $1 -type f -iname "$2" || exit 1) | while read -r filename; do
        cp $filename .
        echo $filename
        echo "Source: \"$(basename $filename)\"; DestDir: \"{app}\"; Flags: ignoreversion" >> installer.iss
    done
}

ver=$(cat main.cpp | grep setApplicationVersion | sed "s/\\s*a.setApplicationVersion(\"//g" | sed "s/\");//g")

cd packages/windows
cp ../../KShare.exe . || exit 1
# BOOOOOOIII Feeels good, it's not powershell.

sed "s/;VER;/$ver/" installer.iss.pattern.top > installer.iss

#addFile ../../build/ffmpeg-3.3.2-win64-shared/bin/avcodec-57.dll
##addFile ../../build/ffmpeg-3.3.2-win64-shared/bin/avdevice-57.dll
##addFile ../../build/ffmpeg-3.3.2-win64-shared/bin/avfilter-6.dll
#addFile ../../build/ffmpeg-3.3.2-win64-shared/bin/avformat-57.dll
#addFile ../../build/ffmpeg-3.3.2-win64-shared/bin/avutil-55.dll
##addFile ../../build/ffmpeg-3.3.2-win64-shared/bin/postproc-54.dll
##addFile ../../build/ffmpeg-3.3.2-win64-shared/bin/swresample-2.dll
#addFile ../../build/ffmpeg-3.3.2-win64-shared/bin/swscale-4.dll
#addFile /c/Qt/5.9/mingw53_32/bin/Qt5Core.dll
#addFile /c/Qt/5.9/mingw53_32/bin/Qt5Network.dll
#addFile /c/Qt/5.9/mingw53_32/bin/Qt5Gui.dll
#addFile /c/Qt/5.9/mingw53_32/bin/Qt5Widgets.dll
#addFile /c/Qt/5.9/mingw53_32/bin/Qt5WinExtras.dll
#addFile /c/Qt/5.9/mingw53_32/bin/Qt5Network.dll
#addFile /c/Qt/5.9/mingw53_32/bin/LIBSTDC++-6.DLL
#addFile /c/Qt/5.9/mingw53_32/bin/LIBWINPTHREAD-1.DLL
#addFile /c/Qt/5.9/mingw53_32/bin/LIBGCC_S_DW2-1.DLL

addAllFiles /c/Qt/5.9/mingw53_32/bin/ '*.dll'
addAllFiles ../../build/QtAV-depends-windows-x86+x64/bin/ '*.dll'

cat installer.iss.pattern.bottom >> installer.iss
"C:\Program Files (x86)\Inno Setup 5\ISCC.exe" installer.iss
cp Output/setup.exe ../../installer.exe || exit 1
