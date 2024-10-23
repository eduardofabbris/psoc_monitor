SOURCES=monitor_guide.md

HTML=guide.html

.PHONY: $(HTML) clean

$(HTML): $(SOURCES)
	pandoc --standalone --toc \
	$(SOURCES) -o $(HTML) \
	--css=pandoc.css \
	--mathjax="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML" \
	--metadata title="PSoc 6 Monitor Guide"

clean:
	rm -f *.html
