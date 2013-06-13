#!/bin/bash

rm *.aux *.log *~
rm *.dvi

latex *.tex
bibtex *.aux
latex *.tex
pdflatex *.tex

rm *.dvi
