#!/bin/bash
valgrind --leak-check=full ./mytests 1000000 4 32
