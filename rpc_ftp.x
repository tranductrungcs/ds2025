/*
 * DISTRIBUTED SYSTEMS - Group 3 - FTP Proxy via RPC 
 * RPC Interface Definition for FTP Proxy
 */
program FTPPROXY_PROG {
    version FTPPROXY_VERS {
        int upload(string file_name, opaque file_data<>);
        opaque download(string file_name) = 2;
        string list_files() = 3;
    } = 1;
} = 0x20000001;
