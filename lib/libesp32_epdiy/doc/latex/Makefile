LATEX_CMD=pdflatex

all: refman.pdf

pdf: refman.pdf

refman.pdf: clean refman.tex
	$(LATEX_CMD) refman
	makeindex refman.idx
	$(LATEX_CMD) refman
	latex_count=8 ; \
	while egrep -s 'Rerun (LaTeX|to get cross-references right)' refman.log && [ $$latex_count -gt 0 ] ;\
	    do \
	      echo "Rerunning latex...." ;\
	      $(LATEX_CMD) refman ;\
	      latex_count=`expr $$latex_count - 1` ;\
	    done
	makeindex refman.idx
	$(LATEX_CMD) refman


clean:
	rm -f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.out *.brf *.blg *.bbl refman.pdf
