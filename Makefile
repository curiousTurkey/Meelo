meelo: meelo.c
		$(CC) meelo.c -o meelo -Wall -Wextra -pedantic -std=c99
# -Wall = Warning all, -Wextra = Warning extra that is not enabled by Wall, -pedantic =  Issues warnings for non-standard C constructs , std=c99 : specifies the c standard to compile.