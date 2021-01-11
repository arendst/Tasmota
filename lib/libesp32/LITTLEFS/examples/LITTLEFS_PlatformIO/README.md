# How to run on PlatformIO IDE

- Download and extract to this project root a **mklittlefs** executable for your OS [from a zipped binary here](https://github.com/earlephilhower/mklittlefs/releases) 
- Open **LITTLEFS_PlatformIO** folder
- Run PlatformIO project task: **Upload Filesystem Image** 
- Run PlatformIO project task: **Upload and Monitor**
- You will see a Serial output like:
```
--- Miniterm on COM5  115200,8,N,1 ---
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
ets Jun  8 2016 00:22:57

rst:0x1 (POWERON_RESET),boot:0x13 (Snfigsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0018,len:4
load:0x3fff001c,len:1044
load:0x40078000,len:10044
load:0x40080400,len:5872
entry 0x400806ac
Listing directory: /
  FILE: /file1.txt  SIZE: 3  LAST WRITE: 2020-10-06 15:10:33
  DIR : /testfolder  LAST WRITE: 2020-10-06 15:10:33
Creating Dir: /mydir
Dir created
Writing file: /mydir/hello2.txt
- file written
Listing directory: /
  FILE: /file1.txt  SIZE: 3  LAST WRITE: 2020-10-06 15:10:33
  DIR : /mydir  LAST WRITE: 1970-01-01 00:00:00
Listing directory: /mydir
  FILE: /mydir/hello2.txt  SIZE: 6  LAST WRITE: 1970-01-01 00:00:00
  DIR : /testfolder  LAST WRITE: 2020-10-06 15:10:33
Listing directory: /testfolder
  FILE: /testfolder/test2.txt  SIZE: 3  LAST WRITE: 2020-10-06 15:10:33
Deleting file: /mydir/hello2.txt
- file deleted
Removing Dir: /mydir
Dir removed
Listing directory: /
  FILE: /file1.txt  SIZE: 3  LAST WRITE: 2020-10-06 15:10:33
  DIR : /testfolder  LAST WRITE: 2020-10-06 15:10:33
Listing directory: /testfolder
  FILE: /testfolder/test2.txt  SIZE: 3  LAST WRITE: 2020-10-06 15:10:33
Writing file: /hello.txt
- file written
Appending to file: /hello.txt
- message appended
Reading file: /hello.txt
- read from file:
Hello World!
Renaming file /hello.txt to /foo.txt
- file renamed
Reading file: /foo.txt
- read from file:
Hello World!
Deleting file: /foo.txt
- file deleted
Testing file I/O with /test.txt
- writing................................................................
 - 1048576 bytes written in 12006 ms
- reading................................................................
- 1048576 bytes read in 547 ms
Deleting file: /test.txt
- file deleted
Test complete
```
- If you have a module with more than 4MB flash, you can uncomment **partitions_custom.csv** in **platformio.ini** and modify the csv file accordingly