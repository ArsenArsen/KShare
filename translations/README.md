## So, I hear ya want to translate
here is how to do it:
Firstly, run `lupdate ../KShare.pro -no-obsolete -ts <your locale (eg. en_US or sr_RS)>.ts`

If you see something that looks like it was just changed a bit, take the old translation and update it, as `-no-obsolete` removed it.
Then open and edit the file in Qt Linguist.
When you're done editing the file, save it and run `lrelease <your .ts file>` and add an entry into `../src/translations.qrc` for the newly produced `.qm` file (if there isn't one).

That's all, thank you!
