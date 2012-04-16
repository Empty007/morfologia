rem gcc -c -Ic:/prj/src/include c:\prj\src\lib\pac_win.c 
rem gcc -c -Ic:/prj/src/include c:\prj\src\lib\pac_bas.c
rem gcc -c -Ic:/prj/src/include c:\prj\src\lib\pac_xpl.c
rem gcc -c -Ic:/prj/src/include c:\prj\src\lib\pac_tools.c
gcc -Ic:/prj/src/include vdspbas.c c:\prj\src\obj\pac_*.o -o vdspbas
