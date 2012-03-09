#include "aoi.c"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX 1000

static struct aoi_space * SPACE = NULL;
static int INDEX[MAX];

static void
shuffle(int number) {
	int i;
	for (i=0;i<number * 2;i++) {
		int idx1 = rand() % number;
		int idx2 = rand() % number;
		int temp = INDEX[idx1];
		INDEX[idx1] = INDEX[idx2];
		INDEX[idx2] = temp;
	}
}

static void
init(int number, int start) {
	int i;
	for (i=0;i<number;i++) {
		INDEX[i] = i+start;
	}
}

static void
mnew(struct map *m, uint32_t id) {
	struct object * obj = map_query(SPACE , m , id);
//	printf("new %u\n",id);
	assert(obj->id == id);
	assert(obj->ref == 1);
}

static void
mdelete(struct map *m, uint32_t id) {
	struct object * obj = map_drop(m , id);
//	printf("delete %u\n",id);
	assert(obj != NULL);
	assert(obj->id == id);
	assert(obj->ref == 1);
	drop_object(SPACE, obj);
}

static void
test(struct map *m , int n, int start) {
	init(n,start);
	shuffle(n);
	int i;
	for (i=0;i<n;i++) {
		mnew(m,INDEX[i]);
	}
	shuffle(n);
	n =  rand() % (n/2);
	for (i=0;i<n;i++) {
		mdelete(m,INDEX[i]);
	}
}

static void
check(void *ud, struct object * obj) {
	struct map * m = ud;
	printf("%u ",obj->id);
	mdelete(m, obj->id);
}

struct alloc_cookie {
	int count;
	int max;
	int current;
};

static void *
my_alloc(void * ud, void *ptr, size_t sz) {
	struct alloc_cookie * cookie = ud;
	if (ptr == NULL) {
		void *p = malloc(sz);
		++ cookie->count;
		cookie->current += sz;
		if (cookie->max < cookie->current) {
			cookie->max = cookie->current;
		}
//		printf("%p + %u\n",p, sz);
		return p;
	}
	-- cookie->count;
	cookie->current -= sz;
//	printf("%p - %u \n",ptr, sz);
	free(ptr);
	return NULL;
}

int
main()
{
	struct alloc_cookie cookie = { 0,0,0 };
	SPACE = aoi_create(my_alloc , &cookie);

	struct map * m = map_new(SPACE);

	int i;
	for (i=0;i<10;i++) {
		test(m,10,i*10);
	}
	test(m,100,100);
	test(m,200,200);
	test(m,500,500);

	map_foreach(m,check,m);

	map_delete(SPACE, m);
	aoi_release(SPACE);

	printf("max memory = %d, current memory = %d\n", cookie.max , cookie.current);
	return 0;
}
