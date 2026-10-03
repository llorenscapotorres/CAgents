#include <stdio.h>
#include <stdlib.h>

#include <generate/invoke.h>

int main(void) {
    const char *json =
        "{\"model\":\"qwen3:4b\","
        "\"messages\":[{\"role\":\"user\",\"content\":\"Hello, how are you?\"}],"
        "\"stream\":false}";
    
    char *text = invoke(json);

    printf("LLM Response: %s\n", text);
    free(text);

    return 0;
}