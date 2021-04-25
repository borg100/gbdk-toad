C:\gbdk\bin\png2mtspr ..\tiles\toadbody.png -sw 16 -sh 24 -spr8x8 -c ..\tiles\toadbody.c
REM C:\gbdk\bin\png2mtspr ..\tiles\toadsmoke.png -sw 8 -sh 16 -spr8x8 -c ..\tiles\toadsmoke.c
C:\gbdk\bin\lcc.exe -o main.gb ..\main.c ..\tiles\toadbody.c ..\tiles\toadsmoke.c