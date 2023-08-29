#include <stdio.h>
#include <stdlib.h>
#include <gssapi/gssapi.h>
#include <gssapi/gssapi_krb5.h>
#include <krb5.h>
#include <string.h>

void print_error(const char* message, OM_uint32 major_status, OM_uint32 minor_status) {
	fprintf(stderr, "%s: %i %i\n", message, major_status, minor_status);
}


int main() {
    OM_uint32 major_status, minor_status; //major_status -успешно/неуспешно  minor_status - дополнительная информация
    gss_name_t initiator_name;
    gss_cred_id_t initiator_cred_handle;
    gss_ctx_id_t context_handle;
    gss_buffer_desc output_token;
	gss_buffer_desc login_buffer = GSS_C_EMPTY_BUFFER;
    gss_buffer_desc password_buffer = GSS_C_EMPTY_BUFFER;
	gss_name_t target_name = GSS_C_NO_NAME;

	// Установка логина и пароля
    const char* login = "";
    const char* password = "";
    login_buffer.value = (void*)login;
    login_buffer.length = strlen(login);
    password_buffer.value = (void*)password;
    password_buffer.length = strlen(password);
	
	fprintf(stderr, "========================PRINTF_1===================\n");

    // Установка имени сервера Samba в target_name
    const char* server_name = "";
    //major_status = gss_import_name(&minor_status, (void*)server_name, GSS_C_NT_HOSTBASED_SERVICE, &target_name);
	major_status = gss_import_name(&minor_status, (gss_buffer_t)server_name, GSS_C_NT_HOSTBASED_SERVICE, &target_name);

	fprintf(stderr, "========================PRINTF_2===================\n");
    if (major_status != GSS_S_COMPLETE) {
        print_error("Ошибка при импорте имени", major_status, minor_status);
        return 1;
    }
    // Инициализация GSS-API
    //major_status = gss_init_sec_context(&minor_status, GSS_C_NO_CREDENTIAL, &context_handle, GSS_C_NO_NAME, GSS_C_NO_OID, GSS_C_MUTUAL_FLAG, 0, GSS_C_NO_CHANNEL_BINDINGS, GSS_C_NO_BUFFER, NULL, &output_token, NULL, NULL);
	major_status = gss_init_sec_context(&minor_status, GSS_C_NO_CREDENTIAL, &context_handle, target_name, GSS_C_NO_OID, GSS_C_MUTUAL_FLAG, 0, GSS_C_NO_CHANNEL_BINDINGS, GSS_C_NO_BUFFER, NULL, &output_token, NULL, NULL);

    if (major_status != GSS_S_COMPLETE) {
        //fprintf(stderr, "Ошибка при инициализации GSS-API: %i %i\n", major_status, minor_status);
		print_error("Ошибка при инициализации GSS-API", major_status, minor_status);
        return 1;
    }

    // Получение дескриптора учетных данных инициатора
    major_status = gss_acquire_cred_with_password(&minor_status, initiator_name, &password_buffer, GSS_C_INDEFINITE, GSS_C_NO_OID_SET, GSS_C_INITIATE, &initiator_cred_handle, NULL, NULL);
    if (major_status != GSS_S_COMPLETE) {
        print_error("Ошибка при получении дескриптора учетных данных", major_status, minor_status);
        return 1;
    }



    // Освобождение ресурсов
    gss_release_name(&minor_status, &initiator_name);
    gss_release_cred(&minor_status, &initiator_cred_handle);
    gss_release_buffer(&minor_status, &output_token);

    return 0;
}



