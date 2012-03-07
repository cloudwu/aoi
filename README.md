## Area of Interest

```C

/* 
 * update an aoi object.
 * "w" : watcher
 * "m" : marker
 * "wm" : watcher and marker
 * "d" : drop an object
 */
void aoi_update(struct aoi_space * space , uint32_t id, const char * mode , float pos[3]);

/*
 * define a callback function first, and call aoi_message in your timer or main loop.
 */
void aoi_message(struct aoi_space *space, aoi_Callback cb, void *ud);

/*
 * You can define your own allocator for each independent space.
 * typedef void * (*aoi_Alloc)(void *ud, void * ptr, size_t sz);
 */
struct aoi_space * aoi_create(aoi_Alloc alloc, void *ud);
void aoi_release(struct aoi_space *);

/*
 * You can use the default allocator
 */
struct aoi_space * aoi_new();

```
See test.c for example.

See http://blog.codingnow.com/2012/03/dev_note_13.html for algorithm (In chinese) .

