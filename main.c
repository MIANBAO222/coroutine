#include "coroutine.h"
#include <stdio.h>

struct args {
	int n;
};

static void
foo(struct schedule * S, void *ud) {
	struct args * arg = ud;
	int start = arg->n;
	int i;
	for (i=0;i<5;i++) {
		printf("coroutine %d : %d\n",coroutine_running(S) , start + i);//打印调度器此时正在running的协程id（即自己的协程id）
		coroutine_yield(S);//协程主动放弃
	}
}

static void
test(struct schedule *S) {
	struct args arg1 = { 0 };
	struct args arg2 = { 100 };

	int co1 = coroutine_new(S, foo, &arg1);//创建协程1，此时未运行
	int co2 = coroutine_new(S, foo, &arg2);//创建协程2，此时未运行
	printf("main start\n");
	while (coroutine_status(S,co1) && coroutine_status(S,co2)) {
		coroutine_resume(S,co1);
		coroutine_resume(S,co2);
	} 
	printf("main end\n");
}

int 
main() {
	struct schedule * S = coroutine_open();
	test(S);
	coroutine_close(S);
	
	return 0;
}

