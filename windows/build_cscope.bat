@echo off

find . -name "*.c" -o -name "*.h" -o -name "*.cc" > cscope.files\
&& cscope -bkq -i cscope.files
