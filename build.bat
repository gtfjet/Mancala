call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
cl mancala.c /nologo /O2
del *.obj
pause