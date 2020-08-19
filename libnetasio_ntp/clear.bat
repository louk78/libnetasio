cd Debug
del *.ilk
del *.pdb
del *.exp
del *.exe
del *.lib
del libnetasio.dll
cd ..
rd /s /Q .vs
rd /s /Q client\Debug
rd /s /Q libnetasio\Debug
rd /s /Q server\Debug
rd /s /Q sqlclient\Debug
rd /s /Q sqlserver\Debug