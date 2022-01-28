#include "run.h"
#include "shell.h"

void run(int argv, char** argc) {
	if(argv == 1)
		launchShell();
}
