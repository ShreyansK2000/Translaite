#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../Internet/Parse.h"
#include "Test.h"

int test_parse_register(void)
{
    // User exists test
    int user_exists = parse_register_buffer("\"USER_EXISTS\"");
    if (user_exists != UserExists) {
        printf("test_parse_register (user exists): Expected output: %d,\n Got output: %d\n\n", UserExists, user_exists);
        return Failed;
    } 

    // Ok remove test (that number is a fake user mongo object id)
    int remove_ok = parse_register_buffer("\"123123141231231\"");
    if (remove_ok != RemoveOK) {
        printf("test_parse_register (remove ok): Expected output: %d,\n Got output: %d\n\n", RemoveOK, remove_ok);
        return Failed;
    }

    // Server timeout test
    int timeout = parse_register_buffer("");
    if (timeout != NULLRegisterFailure) {
        printf("test_parse_register (server timeout): Expected output: %d,\n Got output: %d\n\n", NULLRegisterFailure, timeout);
        return Failed;
    }

    printf("PASSED parse_register test\n");
    return Passed;
}
 
int test_parse_login(void)
{
    // User does not exist test
    int user_dne = parse_login_buffer("\"USER_DNE\"");
    if (user_dne != UserDNE) {
        printf("test_parse_login (user dne): Expected output: %d,\n Got output: %d\n\n", UserDNE, user_dne);
        return Failed;
    } 

    // Incorrect password test
    int incorrect_password = parse_login_buffer("\"INCORRECT_PASSWORD\"");
    if (incorrect_password != IncorrectPassword) {
        printf("test_parse_login (incorrect password): Expected output: %d,\n Got output: %d\n\n", IncorrectPassword, incorrect_password);
        return Failed;
    }

    // Server timeout test
    int server_timeout = parse_login_buffer("");
    if (server_timeout != NULLLoginFailure) {
        printf("test_parse_login (server timeout): Expected output: %d,\n Got output: %d\n\n", NULLLoginFailure, server_timeout);
        return Failed;
    }
    
    // Ok login test (that number is a fake user mongo object id)
    int ok_login = parse_login_buffer("\"12341231231412\"");
    if (ok_login != LoginSuccess) {
        printf("test_parse_login (ok login): Expected output: %d,\n Got output: %d\n\n", LoginSuccess, ok_login);
        return Failed;
    }

    printf("PASSED parse_login test\n");
    return Passed;
}

int test_parse_add_history(void)
{
    // User does not exist test
    int no_such_user = parse_add_history_buffer("\"NO_SUCH_USER\"");
    if (no_such_user != NoSuchUser) {
        printf("test_parse_add_history (no such user): Expected output: %d,\n Got output: %d\n\n", NoSuchUser, no_such_user);
        return Failed;
    }

    // Server timeout test
    int server_timeout = parse_add_history_buffer("");
    if (server_timeout != NULLAddFailure) {
        printf("test_parse_add_history (server timeout): Expected output: %d,\n Got output: %d\n\n", NULLAddFailure, server_timeout);
        return Failed;
    }

    // Add history ok
    int add_ok = parse_add_history_buffer("\"ADD_OK\"");
    if (add_ok != AddOK) {
        printf("test_parse_add_history (add ok): Expected output: %d,\n Got output: %d\n\n", AddOK, add_ok);
        return Failed;
    }

    printf("PASSED parse_add_history test\n");
    return Passed;
}

int test_parse_remove_history(void)
{
    // Translation does not exist test
    int no_trans = parse_remove_history_buffer("\"NO_SUCH_TRANSLATION\"");
    if (no_trans != NoSuchTranslation) {
        printf("test_parse_remove_history (no such translation): Expected output: %d,\n Got output: %d\n\n", NoSuchTranslation, no_trans);
        return Failed;
    }

    // Server timeout test
    int server_timeout = parse_remove_history_buffer("");
    if (server_timeout != NULLRemoveFailure) {
        printf("test_parse_remove_history (server timeout): Expected output: %d,\n Got output: %d\n\n", NULLRemoveFailure, server_timeout);
        return Failed;
    }

    // Remove history ok
    int remove_ok = parse_remove_history_buffer("\"REMOVE_OK\"");
    if (remove_ok != RemoveOK) {
        printf("test_parse_remove_history (remove ok): Expected output: %d,\n Got output: %d\n\n", RemoveOK, remove_ok);
        return Failed;
    }

    printf("PASSED parse_remove_history test\n");
    return Passed;
}

int test_parse_open_sock(void)
{
    // Open socket ok
    int open_ok = parse_open_sock_buffer("\"OK\"");
    if (open_ok != OpenSockOK) {
        printf("test_parse_open_sock (open ok): Expected output: %d,\n Got output: %d\n\n", OpenSockOK, open_ok);
        return Failed;
    }

    // Open socket error test (anything other than OK is considered an error)
    int open_err = parse_open_sock_buffer("\"garbagebagiajdiwjadiw\"");
    if (open_err != OpenSockERR) {
        printf("test_parse_open_sock (open err): Expected output: %d,\n Got output: %d\n\n", OpenSockERR, open_err);
        return Failed;
    }

    // Server timeout error
    int server_timeout = parse_open_sock_buffer("");
    if (server_timeout != NULLOpenSockFailure) {
        printf("test_parse_open_sock (open err): Expected output: %d,\n Got output: %d\n\n", NULLOpenSockFailure, server_timeout);
        return Failed;
    }

    printf("PASSED parse_open_socket test\n");
    return Passed;
}

int test_parse_location(void)
{
    char * countries[6] = {"\"france\"", "\"germany\"", "\"italy\"", "\"spain\"",  "\"canada\"",  "\"ethiopia\""};
    int i;

    for (i = 0; i < 6; i++) {
        if (parse_location_buffer(countries[i]) == NULLLocationFailure){
            printf("VALID TEST\nExpected a non-null code, got NULLLocationFailure");
            return Failed;
        }
    }

    printf("PASSED parse_location test\n");
    return Passed;
}

int test_parse_Q_to_Q(void)
{
    char valid[1024] = "\"This is\" a valid Q to Q string";
    char valid_expected_buffer[1024] = "This is";
    char valid_expected_return[1024] = " a valid Q to Q string";
    
    char * input = &valid[0];
    char buffer[1024] = "";
    input = get_Q_to_Q(input, buffer);
    
    if (strcmp(valid_expected_buffer, buffer) != 0) {
        printf("VALID TEST\nExpected Q_to_Q output:\t %s\nGot output:\t %s\n\n", valid_expected_buffer, buffer);
        return Failed;
    }

    if (strcmp(valid_expected_return, input) != 0) {
        printf("VALID TEST\nExpected Q_to_Q remaining: %s,\n Got output: %s\n\n", valid_expected_return, input);
        return Failed;
    }

    char invalid[1024] = "This is an invalid Q to Q string";
    char invalid_expected_buffer[1024] = "";
    char invalid_expected_return[1024] = "This is an invalid Q to Q string";
    
    char * input_invalid = &invalid[0];
    char invalid_buffer[1024] = "";
    input = get_Q_to_Q(input, invalid_buffer);
    
    if (strcmp(invalid_expected_buffer, invalid_buffer) != 0) {
        printf("INVALID TEST\nExpected Q_to_Q output: %s,\n Got output: %s\n\n", invalid_expected_buffer, invalid_buffer);
        return Failed;
    }

    if (strcmp(invalid_expected_return, input_invalid) != 0) {
        printf("INVALID TEST\nExpected Q_to_Q remaining: %s,\n Got output: %s\n\n", invalid_expected_return, input_invalid);
        return Failed;
    }
    
    printf("PASSED Q_to_Q test\n");
   return Passed;

}

int test_parse_CB_to_CB(void)
{
    int * bytes_written;
    *bytes_written = 0;

    char valid[1024] = "{Response} value";
    char valid_expected_buffer[1024] = "Response";
    char valid_expected_return[1024] = " value";
    
    char * input = &valid[0];
    char buffer[1024] = "";
    input = get_CB_to_CB(input, buffer, bytes_written);
    
    if (strcmp(valid_expected_buffer, buffer) != 0) {
        printf("VALID TEST\nExpected CB_to_CB output: %s,\n Got output: %s\n\n", valid_expected_buffer, buffer);
        return Failed;
    }

    if (!strcmp(valid_expected_return, input) != 0) {
        printf("VALID TEST\nExpected CB_to_CB remaining: %s,\n Got output: %s\n\n", valid_expected_return, input);
        return Failed;
    }

    char invalid[1024] = "Response value";
    char invalid_expected_buffer[1024] = "";
    char invalid_expected_return[1024] = "Response value";
    
    char * input_invalid = &invalid[0];
    char invalid_buffer[1024] = "";
    input = get_CB_to_CB(input, invalid_buffer, bytes_written);
    
    if (strcmp(invalid_expected_buffer, invalid_buffer) != 0) {
        printf("INVALID TEST\nExpected CB_to_CB output: %s,\n Got output: %s\n\n", invalid_expected_buffer, invalid_buffer);
        return Failed;
    }

    if (strcmp(invalid_expected_return, input_invalid) != 0) {
        printf("INVALID TEST\nExpected CB_to_CB remaining: %s,\n Got output: %s\n\n", invalid_expected_return, input_invalid);
        return Failed;
    }
    
    printf("PASSED CB_to_CB test\n");
    return Passed;
}

int test_check_empty_array(void) 
{
    int retval;
    char valid_empty_array[1024] = "This is the array: []";
    
    if (!(retval = check_empty_array(&valid_empty_array[0]))) {
        printf("VALID TEST\ncheck_empty_array should have been empty, returned %d\n\n", retval);
        return Failed;
    }

    char invalid_empty_array[1024] = "This is the array: [Not Empty!]";
    
    if ((retval = check_empty_array(&invalid_empty_array[0]))) {
        printf("INVALID TEST\ncheck_empty_array should NOT have been empty, returned %d\n\n", retval);
        return Failed;
    }
    
    printf("PASSED check_empty_array test\n");

    return Passed;
}

int test_parse_translation(void)
{
    FILE * valid_file;

    valid_file = fopen("Test_Files/translation_valid_input.txt", "rb");

    fseek(valid_file, 0, SEEK_END);
    long fsize = ftell(valid_file);
    fseek(valid_file, 0, SEEK_SET);

    char * valid_string = malloc(fsize + 1);
    fread(valid_string, 1, fsize, valid_file);
    fclose(valid_file);
    valid_string[fsize] = 0;

    topLevelParsed * valid_response = parse_translation_buffer(valid_string);

    if (valid_response->nativeLanguage == NULL) {
        printf("Native Language field must not be NULL!\n");
        free_struct(valid_response);
        return Failed;
    }

    if (valid_response->nativeLanguage->objectCount != 0) {
        printf("Native Language object count must be 0!\n");
        free_struct(valid_response);
        return Failed;
    }

    if (valid_response->targetLanguage == NULL) {
        printf("Target Language field must not be NULL!\n");
        free_struct(valid_response);
        return Failed;
    }

    if (valid_response->targetLanguage->objectCount != 0) {
        printf("Target Language object count must be 0!\n");
        free_struct(valid_response);
        return Failed;
    }

    if (valid_response->objects == NULL) {
        printf("Objects field must not be NULL!\n");
        free_struct(valid_response);
        return Failed;
    }

    if (valid_response->objects->objectCount != 2) {
        printf("For test input, object count must be 2!\n");
        free_struct(valid_response);
        return Failed;
    }

    if (valid_response->objects->head == NULL) {
        printf("For test input, object head must not be NULL!!\n");
        free_struct(valid_response);
        return Failed;
    }

    free_struct(valid_response);
    free(valid_string);
    valid_string = NULL;

    FILE * invalid_file;

    invalid_file = fopen("Test_Files/translation_invalid_input.txt", "r");

    fseek(invalid_file, 0, SEEK_END);
    fsize = ftell(invalid_file);
    fseek(invalid_file, 0, SEEK_SET);

    char *invalid_string = malloc(fsize + 1);
    fread(invalid_string, 1, fsize, invalid_file);
    fclose(invalid_file);
    invalid_string[fsize] = 0;

    if (parse_translation_buffer(invalid_string) != NULL) {
        printf("Translation parse must return NULL!\n");
        return Failed;
    }

    free(invalid_string);
    invalid_string = NULL;

    printf("PASSED parse_translation test\n");
	return Passed;
}

int test_parse_history(void)
{
    FILE * valid_file;

    valid_file = fopen("Test_Files/history_valid_input.txt", "r");

    fseek(valid_file, 0, SEEK_END);
    long fsize = ftell(valid_file);
    rewind(valid_file);

    char *valid_string = malloc(fsize + 1);
    fread(valid_string, 1, fsize, valid_file);
    fclose(valid_file);
    valid_string[fsize] = 0;

    historyObj * valid_response = parse_history_buffer(valid_string);

    if (valid_response->translationCount != 2) {
        printf("Translation count must not be 0\n");
        free_translation_history(valid_response);
        return Failed;
    }

    if (valid_response->head == NULL) {
        printf("Head field must not be NULL!\n");
        free_translation_history(valid_response);
        return Failed;
    }

    free_translation_history(valid_response);
    free(valid_string);
    valid_string = NULL;


    FILE * invalid_file;

    invalid_file = fopen("Test_Files/history_invalid_input.txt", "r");

    fseek(invalid_file, 0, SEEK_END);
    fsize = ftell(invalid_file);
    fseek(invalid_file, 0, SEEK_SET);

    char *invalid_string = malloc(fsize + 1);
    fread(invalid_string, 1, fsize, invalid_file);
    fclose(invalid_file);
    invalid_string[fsize] = 0;

    if(parse_history_buffer(invalid_string) != NULL) {
        printf("History parse must return NULL!\n");
        return Failed;
    }

    free(invalid_string);
    invalid_string = NULL;

    printf("PASSED parse_history test\n");
	return Passed;
}
