gcov *.gcno
grep -v -e '-:' *.h.gcov | grep -e '\#' -v > todo.txt

