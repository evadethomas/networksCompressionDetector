#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "cJSON.h"

FILE *config_file;

int main(int argc, char* argv[]) {

    //Getting config file name
        char* config_filename = argv[1];
        
        //opening file with open check
        FILE *fp = fopen(config_filename, "r");
        if (fp == NULL) {
            perror("Error opening file");
            return 1;
        }
        
        // Reading the contents of the file into a buffer using fseek
        
        //Get file size to make buffer
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        //Allocate memory for buffer
        char *json_buffer = (char *)malloc(file_size + 1);
        
        if (json_buffer == NULL) {
            perror("Buffer malloc failed.");
            fclose(fp);
            return 1;
        }
        
        //Reading file using byte size check
        size_t bytes_read = fread(json_buffer, 1, file_size, fp);
        
        //Byte size check
    if (bytes_read != file_size) {
            perror("Error reading file");
            fclose(fp);
            free(json_buffer);
            return 1;
    }
    
    //Closing file now that we have buffer
    fclose(fp);
    
    //Add a null terminator char to buffer
        json_buffer[file_size] = '\0'; 
        
        //get the json object using parse
        cJSON *config_json = cJSON_Parse(json_buffer);
        
        
        if (config_json == NULL) {
            perror("Error parsing JSON: %s\n");
            free(json_buffer);
            return 1;
        }
    
        free(json_buffer);
        
        const char *ipaddr = "server_ip_address";
        cJSON *ipaddrcheck = cJSON_GetObjectItem(config_json, ipaddr);
        if (ipaddrcheck != NULL) {
            printf("Name: %s\n", ipaddrcheck->valuestring);
        } else {
            printf("Item '%s' not found\n", ipaddr);
        }
    
    int fdsocket = socket(PF_INET, SOCK_STREAM, PF_UNSPEC);
    if (fdsocket == -1) {
        printf("%s\n", "Socket creation failed");
        return 0;
    }
    
    cJSON_Delete(config_json);
    
    


}