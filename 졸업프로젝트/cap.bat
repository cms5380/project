@echo off
set /p A= ip :

cmd /c adb root
cmd /c adb remount

cmd /c adb connect %A%


set A=0


:Test

cmd /c adb shell "su -c 'tcpdump -c 1000 -vv -w /sdcard/p.pcap'"


cmd /c adb pull /sdcard/p.pcap

start pcap2csv.exe

timeout /t 1 /nobreak > NUL


copy C:\Users\cms53\Desktop\�û���\��ġ\p.pcap C:\Users\cms53\Desktop\�û���\��ġ\pcap_file\p%A%.pcap
copy C:\Users\cms53\Desktop\�û���\��ġ\p.csv C:\Users\cms53\Desktop\�û���\��ġ\csv_file\p%A%.csv
set /a A = A + 1

Del p.pcap
Del p.csv

goto Test


