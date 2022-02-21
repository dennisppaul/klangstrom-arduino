#! /bin/zsh

ARDUINO_CLI=/usr/local/bin/arduino-cli
TIMEOUT=/usr/local/bin/timeout
TEST_DURATION=15
FAILURE_COUNTER=0

###############################################################################

OPT_COMPILE="COMPILE"
OPT_UPLOAD="UPLOAD"
BASE_COLOR=23

if [[ "$TERM" != "dumb" ]]; then
    COLOR_NORM=$(tput sgr0)
    COLOR_OK=$(tput setaf 2)
    COLOR_FAIL=$(tput setaf 1)
fi

###############################################################################

print_section() {
    echo
    echo "++++++++++++++++++++++++++++++++++++++++++"
    echo "+++ "$1 | tr a-z A-Z
    echo "++++++++++++++++++++++++++++++++++++++++++"
    echo
}

run_test() {
	if [[ "$3" == $OPT_COMPILE ]]; then
	   echo -n $OPT_COMPILE
	else
	   echo -n $OPT_UPLOAD
	   FAILURE_COUNTER=$((FAILURE_COUNTER+1));
	fi
	
	if [ -z "$4" ]; then
		if [[ "$3" == $OPT_COMPILE ]]; then
			arduino-cli compile -b $1 -p . $2 > /dev/null 2>&1
		else
			arduino-cli compile -b $1 -u -p . $2 > /dev/null 2>&1
		fi
		RESULT_VALUE=$?
		OK_VALUE=0
	else
		if [[ "$3" == $OPT_COMPILE ]]; then
			arduino-cli compile -b $1 -p . $2 > /dev/null 2>&1
			RESULT_VALUE=$?
			OK_VALUE=0
		else
			timeout --preserve-status $4 arduino-cli compile -b $1 -u -p . $2 > /dev/null 2>&1
			RESULT_VALUE=$?
			OK_VALUE=143
		fi
	fi
	if [ $RESULT_VALUE -eq $(($OK_VALUE)) ]; then
	   echo -n $COLOR_OK":OK"
	else
	   echo -n $COLOR_FAIL":FAIL"
	   FAILURE_COUNTER=$((FAILURE_COUNTER+1));
	fi
	echo -n $COLOR_NORM
}

###############################################################################

print_section "check for Arduino CLI"

$ARDUINO_CLI version

###############################################################################

print_section "compile and upload examples"

for FILE in **/*.ino
do
	echo    "+++ TEST '"$FILE"'"

	echo -n "    "
	echo -n "DESKTOP (SDL)    > "
	run_test klangstrom:desktop:sdl $FILE $OPT_COMPILE $TEST_DURATION
	echo -n " + "
	run_test klangstrom:desktop:sdl $FILE $OPT_UPLOAD  $TEST_DURATION
	echo

	echo -n "    "
	echo -n "MCU (KLST_SHEEP) > "
	run_test klangstrom:stm32:Klangstrom:pnum=KLST_SHEEP $FILE $OPT_COMPILE
	echo -n " + "
	run_test klangstrom:stm32:Klangstrom:pnum=KLST_SHEEP $FILE $OPT_UPLOAD
	echo

# 	echo -n "+++     MCU (KLST_CORE)  "
# 	run_test klangstrom:stm32:Klangstrom:pnum=KLST_CORE $FILE $OPT_COMPILE
# 	echo -n " + "
# 	run_test klangstrom:stm32:Klangstrom:pnum=KLST_CORE $FILE $OPT_UPLOAD
# 	echo
done

echo "+++ NUMBER OF EXAMPLES FAILED: " $FAILURE_COUNTER

# timeout --preserve-status $TEST_DURATION arduino-cli compile -b klangstrom:desktop:sdl -u -p . klang/ExampleADCtoDAC ; echo $?