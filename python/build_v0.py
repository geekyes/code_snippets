#! -*- coding: utf-8 -*-


#================================================================
#                source for rtos_learn
#   
#   filename   : build.py
#   author     : chenjiang
#   date       : 2018-06-06
#   description: ʳ�� python build.py ����
#                TODO ��Ϊʵ�ֵĹ��� makefile ֱ�ӿ���ʵ�֣����������´ο���
#
#================================================================

import os
import sys

def get_src_files_header_paths(curr_path):
    folder_list = []
    header_file_dir_list = []
    src_file_list = []

    #  ��ȡ curr_path ��Ŀ¼
    for elem in os.listdir(curr_path):
        if (os.path.isdir(os.path.join(curr_path, elem))):
            folder_list.append(os.path.join(curr_path, elem))

    for sub_folder in folder_list:
        tmp_folder = []
        folder_exist_file_flag = False
        
        #  һ��Ŀ¼���������ֶ�����һ����Ŀ¼�������ļ�
        #  TODO Ӧ����Ҫ���Ŀ¼������ȵĿ���
        for elem in os.listdir(sub_folder):
            #  ˵�����Ŀ¼�»�����Ŀ¼����ӵ���ʱ�����б���ȥ
            if (os.path.isdir(os.path.join(sub_folder, elem))):
                tmp_folder.append(os.path.join(sub_folder, elem))
            elif (os.path.isfile(os.path.join(sub_folder, elem))):
                if ('c' == elem.split('.')[-1]): #  ˵����Դ�ļ�
                    src_file_list.append(os.path.join(sub_folder, elem))
                elif ('h' == elem.split('.')[-1]): #  ˵�����Ŀ¼��ͷ�ļ�Ŀ¼
                    folder_exist_file_flag = True
        
        #  ���Ŀ¼��ͷ�ļ�Ŀ¼���Լ���֪����ô��ȡ��ǰ�ļ���·����ֻ�ܳ����²�
        if(True == folder_exist_file_flag):
            folder_exist_file_flag = False
            header_file_dir_list.append(sub_folder)
        
        #  ����ǰĿ¼����Ŀ¼��ӵ������б���
        folder_list += tmp_folder

    #  ���Ժ����Ƿ�ִ����ȷ��join�ǰ� list �ַ���
    #  print('src file list: \n' + '\n'.join(src_file_list))
    #  print('header file dir list: \n' + '\n'.join(header_file_dir_list))

    return src_file_list,header_file_dir_list

def execute_cmd(cmd):
    if(os.system(cmd)):
        sys.exit('os.system(' + cmd + ') exec failed!!!')

if ('__main__' == __name__):

    #  ���빤�߱�������
    prefix = 'arm-none-eabi-'
    suffix = ' '
    gcc = prefix + 'gcc' + suffix
    cflags = '-MM' + suffix
    cflags += '-E' + suffix
    cflags += '-DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD' + suffix

    #  ���̵�·����������
    script_path = os.path.abspath('.')
    project_path = os.path.abspath('../')
    test_path = os.path.join(os.path.join(project_path, 'src'), 'test')

    #  ��ȡ���̵�Դ�ļ���ͷ�ļ�Ŀ¼
    src_files, header_paths = get_src_files_header_paths('../')

    #  ��ͷ�ļ�Ŀ¼ת���� gcc �Ĳ���
    for elem in header_paths:
        cflags += '-I"' + elem + '"' + suffix

    #  ɾ�� obj_depend �ļ�
    if (os.path.isfile('obj_depend.d')):
        os.remove('obj_depend.d')
    #  �������� obj ������
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

