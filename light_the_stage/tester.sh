#!/bin/bash
echo "---------------------------------------------------------------------"


# Pass n as command line argument to only compile without runnning tests
if echo "$1" | grep -iq "^n"; then
    echo "---------------------------------------------------------------------"
    exit
fi


for objfile in ./*.o ; do
	for testfile in ./tests/*.in ; do
		echo "---------------------------------------------------------------------"
        actual_output="$($objfile < $testfile)"
        actual_output_trimmed=$(echo -e "${actual_output}" | tr -d '[[:space:]]')
        testname="${testfile%.in}"
        outputfile="$testname.out"
        expected_output="$(cat $outputfile)"
        expected_output_trimmed=$(echo -e "${expected_output}" | tr -d '[[:space:]]')
        diffed_output=$(echo "${actual_output}" | diff -y "${outputfile}" -)
        if [ "$actual_output_trimmed" == "$expected_output_trimmed" ]; then
        	echo -e "${testfile} \e[32mpassed!\e[0m"
        else
        	echo -e "${testfile} \e[31mfailed!\e[0m"
            echo ""
            echo "Diff"
            echo ""
            echo "$diffed_output"
        fi
    done
done
echo "---------------------------------------------------------------------"
