/*
 * DISTRIBUTED SYSTEMS - Group 3 - FTP Proxy via RPC 
 * RPC Server Implementation for FTP Proxy
 * Provides file upload, download, directory listing functionality
 */

#include <rpc/rpc.h>
#include "rpc_ftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define FILE_DIRECTORY "./files/"

/*
 * Saves a file with the given name on the server
 * Currently writes the file name itself as content, not the actual data
 */
int *upload_1_svc(char **file_name, struct svc_req *req) {
    static int result = 0;
    FILE *file = fopen(*file_name, "wb");
    if (!file) {
        perror("Error opening file for writing");
        result = -1;
        return &result;
    }
    fwrite(*file_name, sizeof(char), strlen(*file_name), file);
    fclose(file);
    result = 0;
    return &result;
}

/*
 * Reads and returns file content from the server
 * Limited to 4096 bytes per call. No file name validation
 */
opaque *download_1_svc(char **file_name, struct svc_req *req) {
    static char file_data[4096];
    memset(file_data, 0, sizeof(file_data));
    FILE *file = fopen(*file_name, "rb");
    if (!file) {
        perror("File not found");
        return NULL;
    }
    fread(file_data, 1, sizeof(file_data), file);
    fclose(file);
    return file_data;
}

/*
 * Lists all regular files in the specified directory
 * Buffer size is limited; may truncate long lists
 */
char **list_files_1_svc(void *argp, struct svc_req *req) {
    static char result[4096];
    memset(result, 0, sizeof(result));
    DIR *dir = opendir(FILE_DIRECTORY);
    if (!dir) {
        strcpy(result, "Error opening directory.");
        return &result;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            strcat(result, entry->d_name);
            strcat(result, "\n");
        }
    }
    closedir(dir);
    return &result;
}
