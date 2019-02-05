#!/bin/sh

doxygen docs/doxygen.cfg

if [[ $? -eq 0 ]]
then
    echo "\nOpen docs/html/index.html to see the results."
fi
