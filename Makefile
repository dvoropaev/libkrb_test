.PHONY: all clean

test_libkrb5:	test_libkrb5.c
	gcc test_libkrb5.c -o test_libkrb5 -lgssapi_krb5 -lkrb5

get_ccach:
	gcc -o get_ccache get_ccache.c -lkrb5

clean:
	rm -rf test_libkrb5 get_ccache *.o *.so *~

