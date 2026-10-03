#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#include "invoke.h"

#define KEY_DICT_MESSAGE "message"
#define KEY_DICT_CONTENT "content"

struct Memory {
    char *data;
    size_t size;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->data, mem->size + total +1);
    if (ptr == NULL) {
        return 0;
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, total);
    mem->size += total;
    mem->data[mem->size] = '\0';

    return total;
}

char *invoke(const char *json) {
    CURL *curl;
    CURLcode res;
    long status = 0;

    struct Memory chunk = {0};

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:11434/api/chat");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

        if (res != CURLE_OK) {
            fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_slist_free_all(headers);
    curl_global_cleanup();

    printf("HTTP status: %ld\n", status);

    cJSON *root = cJSON_Parse(chunk.data);
    cJSON *msg = cJSON_GetObjectItem(root, KEY_DICT_MESSAGE);
    cJSON *text = cJSON_GetObjectItem(msg, KEY_DICT_CONTENT);

    size_t len = strlen(text->valuestring);
    char *result = malloc(len + 1);
    memcpy(result, text->valuestring, len + 1);
    cJSON_Delete(root);
    free(chunk.data);

    return result;
}