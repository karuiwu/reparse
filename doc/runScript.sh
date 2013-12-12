#!/bin/bash

rm *.aux *.log *~
rm *.dvi


<<EOF


latex *.tex
bibtex *.aux
latex *.tex
EOF
pdflatex *.tex




rm *.dvi
