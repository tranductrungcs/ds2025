/*
 * DISTRIBUTED SYSTEMS - Group 3 - FTP Proxy via RPC
 * RPC Client Implementation for FTP Proxy
 * Provides functionality to upload, download, list files on a remote server
 */

#include <rpc/rpc.h>
#include "rpc_ftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * upload_file Uploads a local file to the remote server
 * @client Pointer to the RPC client handle
 * @file_name Name of the local file to upload
 * Reads the entire file into memory before sending, which may be inefficient for large files
 */
void upload_file(FTPPROXY_PROG_client *client, const char *file_name) {
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_data = malloc(file_size);  // Allocate memory for file data
    fread(file_data, 1, file_size, file);  // Read file into memory
    fclose(file);

    int *result = upload_1(file_name, file_data, client);  // RPC call to upload
    if (result && *result == 0) {
        printf("File uploaded successfully!\n");
    } else {
        printf("Failed to upload file.\n");
    }
    free(file_data);
}

/*
 * download_file Downloads a file from the remote server to the local machine
 * @client Pointer to the RPC client handle
 * @file_name Name of the file to download
 * Assumes file content fits into a single RPC response
 */
void download_file(FTPPROXY_PROG_client *client, const char *file_name) {
    opaque *file_data = download_1(file_name, client);  // RPC call to download
    if (file_data == NULL) {
        printf("Failed to download file.\n");
        return;
    }

    FILE *file = fopen(file_name, "wb");
    if (!file) {
        perror("Failed to create file locally");
        return;
    }

    fwrite(file_data, 1, strlen(file_data), file);  // Write data to local file
    fclose(file);
    printf("File downloaded successfully!\n");
}

/*
 * list_files Lists all files available on the remote server
 * @client Pointer to the RPC client handle
 */
void list_files(FTPPROXY_PROG_client *client) {
    char **file_list = list_files_1(NULL, client);  // RPC call to list files
    if (file_list) {
        printf("Files on server:\n%s", *file_list);
    } else {
        printf("Failed to list files.\n");
    }
}

/*
 * main Entry point of the FTP Proxy RPC client.
 * @argc Argument count.
 * @argv Argument vector. Expects server host as an argument.
 * Example Usage ./client <server_host>
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <server_host>\n", argv[0]);
        exit(1);
    }

    // Create RPC client handle for the specified server
    FTPPROXY_PROG_client *client = clnt_create(argv[1], FTPPROXY_PROG, FTPPROXY_VERS, "tcp");
    if (!client) {
        clnt_pcreateerror(argv[1]);  // Print error if connection fails
        exit(1);
    }

    // Example calls to upload, download, list files
    upload_file(client, "example.txt");
    download_file(client, "example.txt");
    list_files(client);

    clnt_destroy(client);  // Cleanup and destroy the client handle
    return 0;
}
