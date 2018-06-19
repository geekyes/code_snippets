
import os
import sys

def usage():
    sys.exit('\nUsage: %s curr_path [temp_file_name]\n' % str(sys.argv[0]))

def get_handle_file(curr_path):
    folder_list = [curr_path]
    handle_file_list = []

    #  递归当前所有目录, 获取目录的相对路径
    for sub_folder in folder_list:
        curr_folder = os.path.join(curr_path, sub_folder)
        tmp_folder = []
        for elem in os.listdir(curr_folder):
            if (os.path.isdir(os.path.join(curr_folder, elem))):
                tmp_folder.append(os.path.join(sub_folder, elem))
            elif (os.path.isfile(os.path.join(curr_folder, elem))):
                handle_file_list.append(os.path.join(sub_folder, elem))
        folder_list += tmp_folder


    print('folder_list: \n' + '\n'.join(folder_list) + '\n')
    print('handle_file_list: \n' + '\n'.join(handle_file_list) + '\n')

    return handle_file_list

if ('__main__' == __name__):
    if (len(sys.argv) != 2 and len(sys.argv) != 3):
        usage()

    handle_file_list = get_handle_file(sys.argv[1]) 
    tmp_file_name = ''
    if (3 == len(sys.argv)):
        tmp_file_name += sys.argv[2]

    for elem in handle_file_list:
        # 保存原来的名字
        if ('.pdf' == elem[-4:]):
            tmp_elem = elem[:-4]
        else:
            tmp_elem = elem
        print(tmp_elem)
        # 给当前的文件添加一个.c的后缀
        if(os.rename('' + elem + '', '' + elem + '.c')):
            sys.exit('os.rename(' + elem + ', ' + elem + '.c)' + ' exec failed!!!')
        #  if(os.system('busybox mv "' + elem + '" "' + elem + '.c"')):
            #  sys.exit('busybox mv "' + elem + '" "' + elem + '.c"' + ' exec failed!!!')
        elem += '.c'
        
        generate_file_name = ''
        script_path = os.path.abspath('.')
        
        cmd = []
        cmd = 'uedit32 '
        cmd += '"'
        cmd += elem
        cmd += tmp_file_name
        cmd += '"'
        print(cmd)
        
        #  执行命令不成功
        if (os.system(cmd)):
            sys.exit('os.system(cmd) exec failed!!!')
        
        if (3 == len(sys.argv)):
            generate_file_name = os.path.join(script_path, tmp_file_name)
        elif(2 == len(sys.argv)):
            generate_file_name = os.path.join(script_path, 'tmp')
        
        if (os.path.exists(generate_file_name)):
            os.remove(elem)
            #  os.system('busybox rm -f "' + elem + '"')
            if(os.system('busybox mv "' + generate_file_name + '" "' + tmp_elem + '"')):
                sys.exit('os.system(busybox mv file) exec failed!!!')

