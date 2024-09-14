#!/bin/sh

flex++ -o CoolLexer.cpp ./CoolLexer.flex

g++ -Wall ./driver.cpp ./CoolLexser.cpp -o driver
