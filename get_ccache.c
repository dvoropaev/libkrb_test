#include <stdio.h>
#include <stdlib.h>
#include <krb5.h>



int main() {
    krb5_context context;
    krb5_error_code retval = krb5_init_context(&context);

    if (retval != 0) {
        fprintf(stderr, "Ошибка при инициализации контекста: %s\n", krb5_get_error_message(NULL, retval));
        return 1;
    }

    char *username;
    retval = krb5_get_default_realm(context, &username);
    if (retval != 0) {
        krb5_free_context(context);
        fprintf(stderr, "Ошибка при получении имени пользователя: %s\n", krb5_get_error_message(NULL, retval));
        return 1;
    }
	printf("username: %s\n", username);

    krb5_principal client;
    retval = krb5_parse_name(context, username, &client);
    if (retval != 0) {
        krb5_free_context(context);
        krb5_free_default_realm(context, username);
        fprintf(stderr, "Ошибка при разборе имени пользователя: %s\n", krb5_get_error_message(NULL, retval));
        return 1;
    }
	username="voropaevdmtr@SMB.BASEALT.RU";
	printf("username: %s\n", username);
    krb5_ccache ccache;
    retval = krb5_cc_cache_match(context, client, &ccache);
	fprintf(stderr, "========================PRINTF_1===================\n");
    if (retval != 0) {
        krb5_free_context(context);
		fprintf(stderr, "========================PRINTF_2===================\n");
        //krb5_free_default_realm(context, username);
		fprintf(stderr, "========================PRINTF_3===================\n");
       // krb5_free_principal(context, client);
        fprintf(stderr, "Ошибка при получении кэша учетных данных: %s\n", krb5_get_error_message(NULL, retval));
        return 1;
    }
    printf("Кэш учетных данных успешно получен из keyring.\n");

    krb5_free_context(context);
    krb5_free_default_realm(context, username);
    krb5_free_principal(context, client);

    return 0;
}
