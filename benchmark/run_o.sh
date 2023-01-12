run () {
	echo "Benchmarking \"$1\""
	./bin/ltn -o --src benchmark/$1.ltn
	echo
}


# run () {
# 	echo "Benchmarking \"$1\""
# 	ltnFile="benchmark/$1.ltn"
# 	asmFile="benchmark/$1.asm.ltn"
# 	binFile="benchmark/$1.bin.ltn"
# 	./bin/ltnc -o --exe $binFile --src $ltnFile 
# 	./bin/ltnvm --exe $binFile
# 	echo 
# }

run "array"
run "string"
run "memory"
run "call"
run "member"
run "range"