#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <wchar.h>

#include "Parse.h"

char * get_Q_to_Q(char *str, char*buf){
	int i;
	for(i = 0; i < strlen(str); i++) {
		if (str[i] == '\"') {
			break;
		}
	}

	if (i >= strlen(str)) {
		return NULL;
	}

	// Move str pointer to just after the first quotation
	str = str + (i + 1);

    i = 0;
    while(*str != '\"') {
        buf[i++] = *(str++);
    }
    str++;
    buf[i] = '\0';

    return str;
}

char * get_CB_to_CB(char *str, char * buf, int * bytes_written){
    while(*str != '{'){
        str++;
    }
    str++;

    int i = 0;
    while(*str != '}') {
        buf[i++] = *(str++);
    }
    str++;
    buf[i] = '\0';
    *bytes_written = i;

    return str;
}

int check_empty_array(char * str){
    while(*str != '['){
        str++;
    }
    str++;
    return *str == ']';
}

char * spaces_replaced(char * original)
{
	int i;
	int j = 0;
	char * replaced = malloc(sizeof(char) * 1024);
	for(i = 0; i < strlen(original); i++) {
		if (original[i] != ' ') {
			replaced[j++] = original[i];
		} else {
			replaced[j++] = '%';
			replaced[j++] = '2';
			replaced[j++] = '0';
		}
	}

	replaced[j] = '\0';
	return replaced;
}

int parse_register_buffer(char *str)
{
	char register_response[1024] = "";

    // If there are no '"' characters in the response, timeout or error
	if (get_Q_to_Q(str, register_response) == NULL) {
        return NULLRegisterFailure;
    }

	return strcmp("USER_EXISTS", register_response) == 0 ? UserExists : RegisterSuccess;
}

int parse_login_buffer(char *str)
{
	char login_response[1024] = "";
    
    // If there are no '"' characters in the response, timeout or error
	if (get_Q_to_Q(str, login_response) == NULL) {
		return NULLLoginFailure;
	}

	return strcmp("USER_DNE", login_response) == 0 ? UserDNE :
			(strcmp("INCORRECT_PASSWORD", login_response) == 0 ? IncorrectPassword : LoginSuccess);
}

int parse_add_history_buffer(char * str)
{
	char add_history_response[1024] = "";

    // If there are no '"' characters in the response, timeout or error
	if (get_Q_to_Q(str, add_history_response) == NULL) {
        return NULLAddFailure;
    }

	return strcmp("NO_SUCH_USER", add_history_response) == 0 ? NoSuchUser : AddOK;
}

int parse_remove_history_buffer(char * str)
{
	char remove_history_response[1024] = "";

    // If there are no '"' characters in the response, timeout or error
	if (get_Q_to_Q(str, remove_history_response) == NULL) {
        return NULLRemoveFailure;
    }

	return strcmp("NO_SUCH_TRANSLATION", remove_history_response) == 0 ? NoSuchTranslation : RemoveOK;
}

int parse_open_sock_buffer(char * str)
{
	char open_sock_response[1024] = "";

    // If there are no '"' characters in the response, timeout or error
	if (get_Q_to_Q(str, open_sock_response) == NULL) {
        return NULLOpenSockFailure;
    }

	return strcmp("OK", open_sock_response) == 0 ? OpenSockOK : OpenSockERR;
}

int parse_location_buffer(char *str)
{
	char location_reponse[1024] = "";

    // If there are no '"' characters in the response, timeout or error
	if (get_Q_to_Q(str, location_reponse) == NULL) {
		return NULLLocationFailure;
	}

    /*
     * Currently only check for 5 european countries,
     * can be improved in the future for more flexibility 
     */
     
    if (strcmp("france", location_reponse) == 0){
        return GetLocationFrance;
    } else if (strcmp("spain", location_reponse) == 0) {
        return GetLocationSpain;
    } else if (strcmp("germany", location_reponse) == 0) {
        return GetLocationGermany;
    } else if (strcmp("italy", location_reponse) == 0) {
        return GetLocationItaly;
    } else {
        return GetLocationOther;
    }
}

topLevelParsed * parse_translation_buffer(char *str)
{
    // Allocate memory for required structs
    outObjs * objects = malloc(sizeof(outObjs));
    outObjs * targetLanguage = malloc(sizeof(outObjs));
    outObjs * nativeLanguage = malloc(sizeof(outObjs));

    objects->head = NULL;

    targetLanguage->head = NULL;
    targetLanguage->objectCount = 0;

    nativeLanguage->head = NULL;
    nativeLanguage->objectCount = 0;

    /*
     * Call getQtoQ and getParam once for each outer level key in the JSON object
     * If the first quote '"' cannot be found, null server reponse, else parse normally
     */
    char param1[1024] = "";
    if ((str = get_Q_to_Q(str, param1)) == NULL) return NULL;
    get_param(&str, nativeLanguage, targetLanguage, objects, param1);

    char param2[1024] = "";
    str = get_Q_to_Q(str, param2);
    get_param(&str, nativeLanguage, targetLanguage, objects, param2);

    char param3[1024] = "";
    str = get_Q_to_Q(str, param3);
    get_param(&str, nativeLanguage, targetLanguage, objects, param3);

    // -------------------------- DEBUG calls and prints ----------------------//
//     printf("native language name: %s\n", nativeLanguage->name);
//     printf("native language value: %s\n", nativeLanguage->val);
//     printf("target language name: %s\n", targetLanguage->name);
//     printf("target language value: %s\n", targetLanguage->val);
//     printf("objects name: %s\n", objects->name);
//     printf("objects value: %s\n", objects->val);
//
//     objTransNode * node = objects->head;
//     while (node != NULL) {
//         printf("node native: %s\n", node->native);
//         printf("node translated: %s\n", node->translated);
//         node = node->next;
//     }
    // -------------------------------------------------------------//

    topLevelParsed * output = malloc(sizeof(topLevelParsed));
    output->nativeLanguage = nativeLanguage;
    output->targetLanguage = targetLanguage;
    output->objects = objects;

    return output;
}

int get_param(char ** str, outObjs * nativeLanguage, outObjs * targetLanguage, outObjs * objects, char * param)
{

    char nativelang[] = "nativeLanguage";
    char targetlang[] = "targetLanguage";
    char obj[] = "objects";
    char valbuf[4096] = "";

    // Store native language value for key
    if (strcmp(nativelang, param) == 0) {
        strcpy(nativeLanguage->name, nativelang);
        *str = get_Q_to_Q(*str, valbuf);
        strcpy(nativeLanguage->val, valbuf);

        return 0;
    } 

    // Store target language value for key
    else if (strcmp(targetlang, param) == 0) {
        strcpy(targetLanguage->name, targetlang);
        *str = get_Q_to_Q(*str, valbuf);
        strcpy(targetLanguage->val, valbuf);

        return 1;
    } 
    
    // Store object value as linkedlist of possible for key
    else if (strcmp(obj, param) == 0) {
        strcpy(objects->name, obj);

        /*
         * Check to see if no objects were provided in response, if 
         * so, don't parse
         */
        if (check_empty_array(*str)) {
            strcpy(objects->val, "NULL");
            return 2;
        }

        int bytes_written = 0;
        int numObjs = 1;
        
        // Get contents between first pair of curly braces {...}
        *str = get_CB_to_CB(*str, valbuf, &bytes_written);
        char* newPointer = valbuf;
        
        /* 
         * Remaining curly brace pairs will only occur if commas
         * present in the array 
         * 
         * Store all curly brace object into a single string (valbuf), removes
         * '{', '}' and ','
         */
        while (**str == ',')
        {   
            numObjs++;
            newPointer = (newPointer + bytes_written); 
            bytes_written = 0; 
            *str = get_CB_to_CB(*str, newPointer, &bytes_written);
        }
        strcpy(objects->val, valbuf); 
        char * objString = valbuf;
        objTransNode * prev = NULL;

        /*
         * Construct a linked list for all objects found in the "objects":[...] value
         */
        objects->objectCount = numObjs;
        while (numObjs > 0) {
            objTransNode * newNode = malloc(sizeof(objTransNode));
            newNode->next = NULL;

            char buf1[1024] = "";
            objString = get_Q_to_Q(objString, buf1);
            get_obj_params(&objString, buf1, newNode);

            char buf2[1024] = "";
            objString = get_Q_to_Q(objString, buf2);
            get_obj_params(&objString, buf2, newNode);

            if (objects->head == NULL) {
                objects->head = newNode;
            }

            numObjs--;

            if (prev != NULL) {
                prev->next = newNode;
            }      
            prev = newNode;
        }
        prev = NULL;

        return 2;
    }

    return -1;
}


int get_obj_params(char ** str, char * buf, objTransNode * node)
{
    char labelnative[] = "native";
    char labeltarget[] = "translated";
    char valbuf[1024] = "";

    if (strcmp(labelnative, buf) == 0) {
        *str = get_Q_to_Q(*str, valbuf);
        strcpy(node->native, valbuf);

        return 0;
    } else if (strcmp(labeltarget, buf) == 0) {
        *str = get_Q_to_Q(*str, valbuf);
        strcpy(node->translated, valbuf);

        return 1;
    }

    return -1;
}

void free_struct(topLevelParsed * toFree)
{
	/*
	 * Free all allocated memory to avoid issues.
	 */
	free_nodes(toFree->objects);
	free(toFree->objects);
	free(toFree->nativeLanguage);
	free(toFree->targetLanguage);
	free(toFree);
    toFree = NULL;
}

void free_nodes(outObjs * objects)
{
    objTransNode * node = objects->head;
    objTransNode * nextNode = NULL;
    while (node != NULL) {
        nextNode = node->next;
        free(node);
        node = nextNode;
    }
    nextNode = NULL;
}

historyObj * parse_history_buffer(char * str)
{
    historyObj * history = malloc(sizeof(historyObj));
    history->head = NULL;
    history->translationCount = 0;
    
    char valbuf[4096] = "";
    /*
     * Call getQtoQ to see if "history" key is present in JSON return
     * If null, error or timeout in server response
     */
    char param[1024] = "";
    if ((str = get_Q_to_Q(str, param)) == NULL) return NULL;
    char * afterHistoryPointer = str;

    if (strcmp("history", param) != 0) {
        return history;
    } else {
        if (!check_empty_array(str)) {
            str = afterHistoryPointer;
           
            int bytes_written = 0;
            int numTranslations = 1;
            
            /*
             * Get the first curly brace information
             */
            str = get_CB_to_CB(str, valbuf, &bytes_written);
            char * newPointer = valbuf;
            
            /* 
            * Remaining curly brace pairs will only occur if commas
            * present in the array 
            * 
            * Store all curly brace object into a single string (valbuf), removes
            * '{', '}' and ','
            */
            while (*str == ',')
            {   
                numTranslations++;
                newPointer = (newPointer + bytes_written); 
                bytes_written = 0; 
                str = get_CB_to_CB(str, newPointer, &bytes_written);
            }

            history->translationCount = numTranslations;
            char * translationString = valbuf;
            historyObjectNode * prev = NULL;

            while (numTranslations > 0) {
                historyObjectNode * newNode = malloc(sizeof(historyObjectNode));
                newNode->next = NULL;

                char buf1[1024] = "";
                translationString = get_Q_to_Q(translationString, buf1);
                get_history_params(&translationString, buf1, newNode);

                char buf2[1024] = "";
                translationString = get_Q_to_Q(translationString, buf2);
                get_history_params(&translationString, buf2, newNode);

                char buf3[1024] = "";
                translationString = get_Q_to_Q(translationString, buf3);
                get_history_params(&translationString, buf3, newNode);

                char buf4[1024] = "";
                translationString = get_Q_to_Q(translationString, buf4);
                get_history_params(&translationString, buf4, newNode);

                numTranslations--;

                if (history->head == NULL) {
                    history->head = newNode;
                }

                if (prev != NULL) {
                    prev->next = newNode;
                }      

                prev = newNode;
            }

            prev = NULL;

            // -------------------------- DEBUG calls and prints ----------------------//
//            printf("Head Native Language: %s\n", history->head->native_language);
//            printf("Head Target Language: %s\n", history->head->target_language);
//            printf("Head Native Word: %s\n", history->head->native_word);
//            printf("Head Target Word: %s\n", history->head->target_word);
            // ------------------------------------------------------------------------//
            return history;
        }
    }

    return history;        
}

int get_history_params(char ** str, char * buf, historyObjectNode * node)
{

    const char nativelang[] = "native_language";
    const char targetlang[] = "target_language";
    const char nativeword[] = "native";
    const char targetword[] = "translated";
    char valbuf[4096] = "";

    if (strcmp(nativelang, buf) == 0) {
        *str = get_Q_to_Q(*str, valbuf);
        strcpy(node->native_language, valbuf);

        return 0;
    } else if (strcmp(targetlang, buf) == 0) {
        *str = get_Q_to_Q(*str, valbuf);
        strcpy(node->target_language, valbuf);

        return 1;
    } else if (strcmp(nativeword, buf) == 0) {
        *str = get_Q_to_Q(*str, valbuf);
        strcpy(node->native_word, valbuf);

        return 2;
    } else if (strcmp(targetword, buf) == 0) {
        *str = get_Q_to_Q(*str, valbuf);
        strcpy(node->target_word, valbuf);

        return 3;
    }

    return -1;
}

void remove_history_node(historyObj * history, int index)
{
    int i = 0;
    historyObjectNode * curr = history->head;
    historyObjectNode * next = curr->next;

    if (index == 0) {
        history->head = next;
        free(curr);
        curr = NULL;
        history->translationCount--;
    } else {
        while (i != index - 1) {
            i++;
            curr = next;
            if (curr->next == NULL) {
                printf("NULL Pointer error, indexing incorrectly\n");
                break;
            }
            next = curr->next;
        }
        
        historyObjectNode * tmp = next;
        curr->next = tmp->next;
        free(tmp);
        tmp = NULL;
        history->translationCount--;
    }
}

void free_translation_history(historyObj * history)
{
    free_history_nodes(history);
    free(history);
    history = NULL;
}

void free_history_nodes(historyObj * history)
{
    historyObjectNode * node = history->head;
    historyObjectNode * nextNode = NULL;
    while (node != NULL) {
        nextNode = node->next;
        free(node);
        node = nextNode;
    }
    nextNode = NULL;
}
