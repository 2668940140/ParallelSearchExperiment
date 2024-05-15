all:
	rm -f main && \
	rm -rf build && \
	mkdir -p build\
	&& cd build \
	&& cmake .. \
	&& make && \
	cp main ../main

clean:
	rm -f main && \
	rm -rf build && \
	rm -rf vtune_result