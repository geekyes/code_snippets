#! -*- coding: utf-8 -*-


#================================================================
#                source for rtos_learn
#   
#   filename   : build.py
#   author     : chenjiang
#   date       : 2018-06-06
#   description: 食用 python build.py 构建
#                TODO 因为实现的功能 makefile 直接可以实现，备份用于下次拷贝
#
#================================================================

import os
import sys

def get_src_files_header_paths(curr_path):
    folder_list = []
    header_file_dir_list = []
    src_file_list = []

    #  获取 curr_path 的目录
    for elem in os.listdir(curr_path):
        if (os.path.isdir(os.path.join(curr_path, elem))):
            folder_list.append(os.path.join(curr_path, elem))

    for sub_folder in folder_list:
        tmp_folder = []
        folder_exist_file_flag = False
        
        #  一个目录里面有两种东西，一是子目录，二是文件
        #  TODO 应该需要添加目录遍临深度的控制
        for elem in os.listdir(sub_folder):
            #  说明这个目录下还有子目录，添加到临时遍临列表中去
            if (os.path.isdir(os.path.join(sub_folder, elem))):
                tmp_folder.append(os.path.join(sub_folder, elem))
            elif (os.path.isfile(os.path.join(sub_folder, elem))):
                if ('c' == elem.split('.')[-1]): #  说明是源文件
                    src_file_list.append(os.path.join(sub_folder, elem))
                elif ('h' == elem.split('.')[-1]): #  说明这个目录是头文件目录
                    folder_exist_file_flag = True
        
        #  这个目录是头文件目录，自己不知道怎么获取当前文件的路径，只能出此下策
        if(True == folder_exist_file_flag):
            folder_exist_file_flag = False
            header_file_dir_list.append(sub_folder)
        
        #  将当前目录的子目录添加到遍临列表中
        folder_list += tmp_folder

    #  调试函数是否执行正确，join是把 list 字符化
    #  print('src file list: \n' + '\n'.join(src_file_list))
    #  print('header file dir list: \n' + '\n'.join(header_file_dir_list))

    return src_file_list,header_file_dir_list

def execute_cmd(cmd):
    if(os.system(cmd)):
        sys.exit('os.system(' + cmd + ') exec failed!!!')

if ('__main__' == __name__):

    #  编译工具变量定义
    prefix = 'arm-none-eabi-'
    suffix = ' '
    gcc = prefix + 'gcc' + suffix
    cflags = '-MM' + suffix
    cflags += '-E' + suffix
    cflags += '-DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD' + suffix

    #  工程的路径变量定义
    script_path = os.path.abspath('.')
    project_path = os.path.abspath('../')
    test_path = os.path.join(os.path.join(project_path, 'src'), 'test')

    #  获取工程的源文件和头文件目录
    src_files, header_paths = get_src_files_header_paths('../')

    #  将头文件目录转换成 gcc 的参数
    for elem in header_paths:
        cflags += '-I"' + elem + '"' + suffix

    #  删除 obj_depend 文件
    if (os.path.isfile('obj_depend.d')):
        os.remove('obj_depend.d')
    #  生成所有 obj 的依赖
    for src_file in src_files:
        cmd = gcc + cflags + '"' + src_file + '"' + ' >> ' + 'obj_depend.d'
        execute_cmd(cmd)
        print(os.path.basename(src_file) + ' --> done!!!')
    # save_as obj_depend
    if (os.path.isfile('obj_depend.d')):
        os.rename('obj_depend.d', 'obj_depend.c')
        execute_cmd('uedit32 obj_depend.c')
        os.remove('obj_depend.c')
        os.rename('tmp', 'obj_depend.d')

