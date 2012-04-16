@echo off
md c:\prj\temp
cd c:\prj\temp
copy c:\prj\src\obj\pac_*.o
copy c:\prj\src\obj\fat.o
copy c:\prj\src\obj\dsp.o
cd c:\prj\src\vdspfact
gcc -Ic:/prj/src/include vdspfact.c c:\prj\temp\*.o -o vdspfact
deltree c:\prj\temp

