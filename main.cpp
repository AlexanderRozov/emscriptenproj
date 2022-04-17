



int result = 0;
void* thread_main(void* arg)
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.userData = (void*)0x12345678;
    attr.attributes = EMSCRIPTEN_FETCH_REPLACE | EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;

    attr.onsuccess = [](emscripten_fetch_t *fetch) {
        assert(fetch);
        printf("Finished downloading %llu bytes\n", fetch->numBytes);
        assert(fetch->url);
        assert(!strcmp(fetch->url, "gears.png"));
        assert(fetch->id != 0);
        assert((uintptr_t)fetch->userData == 0x12345678);
        assert(fetch->totalBytes == 6407);
        assert(fetch->numBytes == fetch->totalBytes);
        assert(fetch->data != 0);
        // Compute rudimentary checksum of data
        uint8_t checksum = 0;
        for(int i = 0; i < fetch->numBytes; ++i)
            checksum ^= fetch->data[i];
        printf("Data checksum: %02X\n", checksum);
        assert(checksum == 0x08);
        emscripten_fetch_close(fetch);
        result = 42;
#ifdef DO_PTHREAD_EXIT
        pthread_exit(&result);
#endif
    };

    attr.onprogress = [](emscripten_fetch_t *fetch) {
        assert(fetch);
        if (fetch->status != 200) return;
        printf("onprogress: dataOffset: %llu, numBytes: %llu, totalBytes: %llu\n", fetch->dataOffset, fetch->numBytes, fetch->totalBytes);
        if (fetch->totalBytes > 0) {
            printf("Downloading.. %.2f%% complete.\n", (fetch->dataOffset + fetch->numBytes) * 100.0 / fetch->totalBytes);
        } else {
            printf("Downloading.. %lld bytes complete.\n", fetch->dataOffset + fetch->numBytes);
        }
        // We must receive a call to the onprogress handler with 100% completion.
        if (fetch->dataOffset + fetch->numBytes == fetch->totalBytes) ++result;
        assert(fetch->dataOffset + fetch->numBytes <= fetch->totalBytes);
        assert(fetch->url);
        assert(!strcmp(fetch->url, "gears.png"));
        assert(fetch->id != 0);
        assert((uintptr_t)fetch->userData == 0x12345678);
    };

    attr.onerror = [](emscripten_fetch_t *fetch) {
        assert(false && "onerror handler called, but the transfer should have succeeded!");
        result = 43;
#ifdef DO_PTHREAD_EXIT
        pthread_exit(&result);
#endif
    };

    emscripten_fetch_t *fetch = emscripten_fetch(&attr, "gears.png");
    assert(fetch);
    static int my_result = 99;
    return (void*)&my_result;
}
void Threding() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_main, NULL);
    int* res;
    printf("joining fetch thread\n");
    pthread_join(thread, (void**)&res);
    printf("join done\n");
    printf("fetch thread complete: result=%d\n", *res);
#if DO_PTHREAD_EXIT
    // if we don't use pthread_exit then exit status should be &result
      assert(res == &result);
#else
// if we don't use pthread_exit then we expect the exit status to
// be what was returned from thread_main.
    assert(*res == 99);
#endif


}



int main() {
#define TEST_SIZE 1024000


    Threding();



    /*
        // 1. Populate an IndexedDB file entry with custom data bytes in memory.
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "EM_IDB_STORE");
    attr.attributes = EMSCRIPTEN_FETCH_REPLACE | EMSCRIPTEN_FETCH_SYNCHRONOUS | EMSCRIPTEN_FETCH_PERSIST_FILE;
    uint8_t *data = (uint8_t*)malloc(TEST_SIZE);
    srand(time(NULL));
    for(int i = 0; i < TEST_SIZE; ++i)
      data[i] = (uint8_t)rand() | 0x40;
    attr.requestData = (char *)data;
    attr.requestDataSize = TEST_SIZE;
    emscripten_fetch_t *fetch = emscripten_fetch(&attr, "myfile.dat");
  
     assert(fetch->status == 200 && "Initial IndexedDB store of myfile.dat should have succeeded");
  
    emscripten_fetch_close(fetch);
  
    // 2. Make sure it is there.
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_NO_DOWNLOAD | EMSCRIPTEN_FETCH_SYNCHRONOUS | EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    fetch = emscripten_fetch(&attr, "myfile.dat");
  
    emscripten_fetch_close(fetch);
  
    // 3. Delete the file from IndexedDB by invoking an Emscripten-specific request "EM_IDB_DELETE".
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "EM_IDB_DELETE");
    attr.attributes = EMSCRIPTEN_FETCH_SYNCHRONOUS;
    fetch = emscripten_fetch(&attr, "myfile.dat");
  //  assert(fetch->status == 200 && "Deleting the file from IndexedDB should have succeeded");
  //  assert(fetch->numBytes == 0);
  //  assert(fetch->data == 0);
    emscripten_fetch_close(fetch);
  
    // 4. Now try to get the file again from IndexedDB, and ensure it is no longer available.
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_NO_DOWNLOAD | EMSCRIPTEN_FETCH_SYNCHRONOUS | EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    fetch = emscripten_fetch(&attr, "myfile.dat");
  //  assert(fetch->status == 404 && "Attempting to GET the file from IndexedDB again should have failed after the file has been deleted");
  //  assert(fetch->numBytes == 0);
  //  assert(fetch->data == 0);
    emscripten_fetch_close(fetch);
  
    printf("Test succeeded!\n");
  
  #ifdef REPORT_RESULT
    REPORT_RESULT(0);
  #endif
  */
}