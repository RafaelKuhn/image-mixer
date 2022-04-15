compile: src/main.cpp
	g++ -o main.exe src/main.cpp src/png-wrapper.cpp -I./src/lib src/lib/bmplib.lib src/lib/png-zlib.lib

run:
	@echo "[make] running"
	@./main.exe

clean:
#rm -rf $(OBJDIR)
	@rm -f *.exe