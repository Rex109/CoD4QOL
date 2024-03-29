del mod.ff
del xcommon_cod4qol.iwd

xcopy ui ..\..\raw\ui /SY
xcopy ui_mp ..\..\raw\ui_mp /SY
xcopy images ..\..\raw\images /SY
xcopy materials ..\..\raw\materials /SY
xcopy material_proprieties ..\..\raw\material_proprieties /SY
copy /Y mod.csv ..\..\zone_source

cd ..\..\bin
linker_pc.exe -language english -compress -cleanup mod
cd ..\mods\cod4qol
copy ..\..\zone\english\mod.ff
del ..\..\zone\english\qol.ff
ren ..\..\zone\english\mod.ff qol.ff

7za a -r -tzip xcommon_cod4qol.iwd images