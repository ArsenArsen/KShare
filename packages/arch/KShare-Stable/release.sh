sed "s/;VER;/$1/" PKGBUILD.sample > PKGBUILD
makepkg --printsrcinfo > .SRCINFO
git stage .SRCINFO PKGBUILD
git commit -m "Release $1"
git push
