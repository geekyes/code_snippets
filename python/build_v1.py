#! -*- coding: utf-8 -*-


#================================================================
#                source for rtos_learn
#   
#   filename   : build.py
#   author     : chenjiang
#   date       : 2018-06-06
#   description: 食用 python build.py 构建
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
        if (os.path.isdir(curr_path + elem)):
            folder_list.append(curr_path + elem)

    for sub_folder in folder_list:
        tmp_folder = []
        folder_exist_file_flag = False
        
        #  一个目录里面有两种东西，一是子目录，二是文件
        #  TODO 应该需要添加目录遍临深度的控制
        for elem in os.listdir(sub_folder):
            #  说明这个目录下还有子目录，添加到临时遍临列表中去
            if (os.path.isdir(sub_folder + '/' + elem)):
                tmp_folder.append(sub_folder + '/' + elem)
            elif (os.path.isfile(sub_folder + '/' + elem)):
                if ('c' == elem.split('.')[-1]): #  说明是源文件
                    src_file_list.append(sub_folder + '/' + elem)
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
    target = 'stm32f103c8'
    prefix = 'arm-none-eabi-'
    suffix = ' '
    gcc = prefix + 'gcc' + suffix
    ld = prefix + 'ld' + suffix
    objcopy = prefix + 'objcopy' + suffix
    objdump = prefix + 'objdump' + suffix

    #  工程的路径变量定义
    script_path = '.'
    project_path = '../'

    #  获取工程的源文件和头文件目录
    src_files, header_paths = get_src_files_header_paths(project_path)

    #  将头文件目录转换成 gcc 的参数
    head_file_args = ''
    for elem in header_paths:
        head_file_args += '-I"' + elem + '"' + suffix
    cflags = head_file_args

    #  创建目录结构
    target_path = script_path + '/' + target
    objs_path = target_path + '/' + 'objs'
    if (target != ''):
        if (not os.path.exists(target_path)):
            os.makedirs(objs_path)
    else:
        print('please set "target"')

    #  生成 makefile.common
    mk_common_path = script_path + '/' + 'makefile.common'
    if (os.path.isdir(mk_common_path)):
        os.remove(mk_common_path)
    mk_common_file = open(mk_common_path, 'w')
    mk_common_file.write('\n')
    mk_common_file.write('TARGET := ' + target + '\n')
    mk_common_file.write('CC := ' + gcc + '\n')
    mk_common_file.write('LD := ' + ld + '\n')
    mk_common_file.write('OBJCOPY := ' + objcopy + '\n')
    mk_common_file.write('OBJDUMP := ' + objdump + '\n')
    mk_common_file.write('\n')
    mk_common_file.write('OBJ_PATH := ' + objs_path + '\n')
    mk_common_file.write('BIN_PATH := ' + target_path + '\n')
    mk_common_file.write('HEAD_FILE_PATHS := ' + head_file_args + '\n')
    mk_common_file.write('\n')
    dep = ''
    cflags += '-MM' + suffix
    cflags += '-E' + suffix
    cflags += '-DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD' + suffix
    for elem in src_files:
        base_name = os.path.basename(elem)
        base_name = base_name.split('.')[:-1]
        dep += '$(OBJ_PATH)/' + ''.join(base_name) + '.d '
        mk_common_file.write('$(OBJ_PATH)/' + ''.join(base_name) + '.d : ' +
                ''.join(elem) + '\n')
        mk_common_file.write('\t$(CC) ' + cflags +' $< > $@\n')
    mk_common_file.write('.PHONY : gen_c_dep\ngen_c_dep : ' + dep + '\n\n')
    mk_common_file.write('\n')
    mk_common_file.close()
