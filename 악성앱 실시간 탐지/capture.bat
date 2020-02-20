@echo off

set A=0


:Test

cmd /c adb shell tcpdump -c 1000 -vv -w /sdcard/p.pcap


cmd /c adb pull /sdcard/p.pcap

start pcap2csv.exe

timeout /t 1 /nobreak > NUL


copy C:\Users\cms53\Desktop\올빼미\어치\p.pcap C:\Users\cms53\Desktop\올빼미\어치\pcap_file\p%A%.pcap
copy C:\Users\cms53\Desktop\올빼미\어치\p.csv C:\Users\cms53\Desktop\올빼미\어치\csv_file\p%A%.csv
set /a A = A + 1

Del p.pcap
Del p.csv

goto Test