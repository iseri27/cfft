#!/bin/sh

template_dir=$HOME/Templates

if ! [[ -d $template_dir ]]; then
	echo "没有模板文件夹!"
	notify-send -t 1500 "❌没有模板文件夹"
	exit 1
fi

file_list=`ls $template_dir`
file_list=(${file_list// /})
file_num=${#file_list[@]}

if [ $file_num == 0 ]; then
	echo "没有模板文件!"
	notify-send -t 1500 "❌没有模板文件!"
	exit 1
fi

for((i=0; i < $file_num; i++)); do
	if [ $file_num -ge 10 ] && [ $i -lt 9 ]; then
		idx=" $((i+1))"
	else
		idx="$((i+1))"
	fi
	echo "$idx. ${file_list[$i]} "
done

echo -e "选择要创建的模板 (输入 0 以退出程序):"
read chosen

if [[ "$chosen" = "0" ]]; then
	exit 0
fi

if ! [[ "$chosen" =~ ^[0-9]+$ ]] || ! [ "$chosen" -ge 1 ] || ! [ "$chosen" -le $file_num ]; then
	echo "无效选择: \"$chosen\" !"
	exit 1
fi

chosen=$((chosen-1))

if [[ -f "./${file_list[$chosen]}" ]]; then
	echo "当前目录已有同名文件!"
	exit 1
fi

cp -r "$template_dir/${file_list[$chosen]}" ./${file_list[$chosen]}
