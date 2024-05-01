qmake.exe LemonxNote.pro -spec win32-g++ "CONFIG+=qtquickcompiler"
copy "C:\Users\linch\Documents\build-LemonxNote-Desktop_Qt_5_14_2_MinGW_32_bit-Release\release\LemonxNote.exe" "C:\Users\linch\Documents\LemonxNote\build\LemonxNote.exe" /y

timeout /t 3
