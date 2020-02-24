#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//clang -m64 -o backtrace -O0 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-stack-protector -g backtrace

struct frame {
    struct frame    *f_frame;
    long 			f_retaddr;
    long 			f_arg0;
};

#define get_current_fp()        __builtin_frame_address(0)
#define get_next_fp(fp)         (((struct frame *)(fp))->f_frame)
#define get_return_ptr(fp)      (((struct frame *)(fp))->f_retaddr)

int backtrace (size_t maxip)
{
    int i;
    struct frame *fp;
    for (i = 0, fp = get_current_fp();
   		((size_t)i < maxip) &&
   		get_next_fp(fp) &&
        get_return_ptr(fp);
   		i++, fp = (struct frame *)get_next_fp(fp)) {
  		long ptr = get_return_ptr(fp);
		printf("#%d %lx\n", i, ptr);
    }
    return i;
}

int func5(int arg)
{
	backtrace(4);
	return arg*100;
}

int func4(int arg)
{
	return func5(arg+10);
}

int func3(int arg)
{
	int res = 0;
	if (arg) {
		res = res + (10*arg);
		res = func4(res);
		return res;
	}
	return res + arg;
}

int func2(int arg)
{
	int res = 0;
	if (!arg) {
		res = res + (10*arg);
		return res;
	}
	res = func3(arg+res);
	return res + 4;
}

int func1(int arg)
{
	int res = 0;
	if (arg) {
		res = res + (10*arg);
		res = func2(res);
		return res;
	}
	return res + 5;
}

int main(int argc, char *argv[])
{
	int x = 0, y = 0;
	if (argc > 1) {
		y = atoi(argv[1]);
	}
	int res = func1(x+y);
	printf("Result: %d\n", res);
	return 0;
}
