
import os
from os.path import join
from os import listdir
from os import path
import shutil


def get_folder_list():
    if not path.exists('tasmota'):
        print("Build environment broken!!")
        assert False
    items = listdir('tasmota')
    ino_folders = []
    for item in items:
        if os.path.isdir(join("tasmota",item)) and "ino_x" in item:
            ino_folders.append(join("tasmota",item))
    print("Adding Tasmota drivers from: ",ino_folders)
    return ino_folders

def concatenate_inos(folders):
    file_list = []
    for folder in folders:
        files = sorted(listdir(folder))
        for file in files:
            if ".ino" in file:
                file_list.append(join(folder,file))
                file_list.append(join("tasmota","split_file")) # to be sure we always have line breaks!
            elif ".h" in file:
                # This is a workaround until these files are refactored to become .ino too!!
                # We could easily delete them after the build or check, if they already are in place
                print("Copy non INO file: "+file+" to /tasmota")
                shutil.copy(join(folder,file), join("tasmota",file))
    # print(file_list)

    print("Concatenating " + str(len(file_list)) + " driver files to xxx_tasmota_all_drivers.ino")
    with open(join("tasmota",'xxx_tasmota_all_drivers.ino'),'wb') as stew:
        for f in file_list:
            with open(f,'rb') as fd:
                shutil.copyfileobj(fd, stew)
                # same as above, we could check/delete this file before/after use

ino_folders = get_folder_list()
concatenate_inos(ino_folders)
# assert False 
