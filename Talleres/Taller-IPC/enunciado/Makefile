.PHONY: all clean cleanall

all: enunciado

enunciado: enunciado.tex
	pdflatex enunciado.tex #> /dev/null
	pdflatex enunciado.tex #> /dev/null

	rm -f *.aux *.log *.nav *.out *.snm *.toc *.vrb

clean:
	rm -f *.aux *.log *.nav *.out *.snm *.toc *.vrb

cleanall:
	rm -f enunciado.pdf
	rm -f *.aux *.log *.nav *.out *.snm *.toc *.vrb
