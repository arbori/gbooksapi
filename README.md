# gbooksapi
This project is the native code will be integrated in Bookstore project to retrive data from Google Books.

To accomplish it this project has the header file generated from java class with native code in the project [Bookstore](https://github.com/arbori/bookstore).

To create the shared library with the native code that will use in java project is necessary execute the following commands, where the last one is necessary have the superuser rights.

1. g++ -c -fPIC gbooks.cpp -o gbooks.o
2. g++ -c -fPIC -I/usr/lib/jvm/java-11-openjdk-amd64/include/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/linux/ com_bookstore_service_BookStoreIntegration.cpp -o com_bookstore_service_BookStoreIntegration.o
3. sudo g++ -shared -fPIC -o /lib/libgbooksapi.so gbooks.o com_bookstore_service_BookStoreIntegration.o /usr/lib/x86_64-linux-gnu/libcurl.so -lc

Afeter that the natie code will be available to be used in java project Bookstore.
