#!/usr/bin/bash

download() {
    prefix=$1
    pages=$(($2+1))
    suffix=$3
    name=$4

    dir="/home/corona/Downloads"
    cd ${dir}
    mkdir "${name}"
    cd "${name}"

    trial_time=4
    for ((t=0;t<${trial_time};t++)); do
        for ((i=1;i<${pages};i++)); do
            [[ -f $i.$suffix ]] || {
                url=$prefix$i.$suffix
                aria2c ${url}
            }
        done
    done

    for ((i=1;i<${pages};i++)); do
        [[ -f $i.$suffix ]] || {
            echo "Failed to download: $prefix$i.$suffix" >> ./download_log
        }
    done

}

[[ $# -ne 1 ]] && {
    echo "Please set a file."
    exit 1
}

cat ./$1 | while read line
do
    params=(${line//,/ })
    download ${params[0]} ${params[1]} ${params[2]} ${params[3]}
done
