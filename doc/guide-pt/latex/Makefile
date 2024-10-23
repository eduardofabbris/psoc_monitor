# Variables
TEXFILE = main.tex
PDF = $(TEXFILE:.tex=.pdf)

# Phony targets
.PHONY: all clean

# Default target
all: project

# Rule to build the PDF
project: $(TEXFILE)
	pdflatex $(TEXFILE)
	pdflatex $(TEXFILE)
	pdflatex $(TEXFILE)

# Clean up auxiliary files
clean:
	rm -f *.aux *.bbl *.blg *.log *.out *.lot *.lof *.toc main.pdf

