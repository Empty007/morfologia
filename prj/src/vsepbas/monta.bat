@echo off
md c:\prj\temp
cd c:\prj\temp
copy c:\prj\src\obj\pac_*.o
copy c:\prj\src\obj\fat.o
copy c:\prj\src\obj\dsp.o
cd c:\prj\src\vsepbas
gcc -Ic:/prj/src/include vsepbas.c c:\prj\temp\*.o -o vsepbas
deltree c:\prj\temp

