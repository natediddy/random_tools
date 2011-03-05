Simple Shell Toolkit
---------

To build and install, from this directory run:
    sudo ./build install
To uninstall, from this directory run:
    sudo ./build uninstall

eq  -> equal <br />
ne  -> not equal <br />
gt  -> greater than <br />
ge  -> greater than or equal to <br />
lt  -> less than <br />
le  -> less than or equal to <br />
fe  -> file exists <br />
fne -> file does not exist <br />
fsz -> view size of file <br />

These are meant to be used from the command line or in scripts with control statements such as *if* and *while*.

The 2 programs *eq* and *ne* can be used with numerical values as well as string values, while the rest are only for numerical values.

Examples:
    VAR1=hello
    VAR2=world
    COUNT=0
    NUM=14
    FILE=~/somefile.zip

    if eq hello $VAR1; then
      echo "hello == $VAR1"
    fi
    if ne $VAR1 $VAR2; then
      echo "$VAR1 != $VAR2"
    fi
    while lt $COUNT $NUM; do
      echo "$COUNT"
      (( COUNT++ ))
    done
    if fe $FILE; then
      echo "$FILE exists!"
    fi

Upon passing the test, all of these return 0, and 1 otherwise. Except for *fsz*, which prints the size of a file
