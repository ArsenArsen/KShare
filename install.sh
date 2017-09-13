#!/usr/bin/env bash
uname=$(uname)
function installIfNeeded {
    brew ls --versions $1 > /dev/null
    if [[ ! $? = 0 ]]; then brew install $1
    else brew upgrade $1; fi
}

resultfile=""

if [[ "$uname" = "Darwin" ]]; then
export PATH="/usr/local/opt/qt/bin:$PATH"
command -v brew >/dev/null || { echo "Homebrew is required!"; exit }
installIfNeeded qt
installIfNeeded ffmpeg
installIfNeeded pkg-config
installIfNeeded git
brew link --force qt
resultfile="$(pwd)/KShare/build/KShare.app/"
elif [[ "$uname" = "Linux" ]]; then
echo "Please install Qt5 GUI, Widgets, Networking, and X11 Extras, qmake, ffmpeg development files, git, and pkgconfig"
sh
resultfile="$(pwd)/KShare/build/src/KShare"
else echo "Unsupported OS!" && exit 1; fi

git clone --recursive https://github.com/ArsenArsen/KShare.git || exit 2
cd KShare
mkdir build || exit 3
cd build
qmake-qt5 .. || qmake .. || exit 4
make || exit 5
echo "------------------------------------------------------"
echo "Resulting file is $resultfile"
if [[ "$uname" = "Linux" ]]; then echo "To link the file into path, run sudo ln -s $resultfile /usr/bin/kshare"; fi
cd ..
echo "To update, go to $(pwd), git pull, cd build, and \`make\`"
echo "------------------------------------------------------"
