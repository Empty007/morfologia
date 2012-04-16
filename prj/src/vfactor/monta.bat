@echo off
md c:\prj\temp
cd c:\prj\temp
copy c:\prj\src\obj\pac_*.o
copy c:\prj\src\obj\fat.o
cd c:\prj\src\vfactor
gcc -Ic:/prj/src/include vfactor.c c:\prj\temp\*.o -o vfactor
deltree c:\prj\temp

