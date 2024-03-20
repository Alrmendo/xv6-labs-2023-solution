#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"


/*
 * Function to remove trailing white spaces from a string
 * Parameters:
 *   - path: The string to be trimmed
 * Returns:
 *   - A pointer to the modified string
 */
char* 
rtrim(char* path)
{
    static char newStr[DIRSIZ+1];  // Static buffer to hold the modified string
    int whiteSpaceSize = 0;         // Variable to store the size of trailing white spaces
    int bufSize = 0;                // Variable to store the size of the resulting string
    
    // Loop through the string from the end to find the trailing white spaces
    for(char* p = path + strlen(path) - 1; p >= path && *p == ' '; --p) 
    {
        ++whiteSpaceSize;  // Counting the trailing white spaces
    }
    
    // Calculate the size of the resulting string after trimming white spaces
    bufSize = DIRSIZ - whiteSpaceSize;
    
    // Copy non-white space characters from the original string to the new string buffer
    memmove(newStr, path, bufSize);
    
    // Null-terminate the new string
    newStr[bufSize] = '\0';
    
    // Return the modified string
    return newStr;
}

/*
 * Recursive function to find a file in a directory and its subdirectories
 * Parameters:
 *   - path: The directory path to search
 *   - name: The name of the file to find
 */
void 
find(char* path, char* name)
{
    char buf[512], *p;              // Buffer to hold file paths, pointer for manipulation
    int fd;                         // File descriptor for directory
    struct dirent de;               // Directory entry structure
    struct stat st;                 // File status structure
    
    // Open the directory specified by the path
    if ((fd = open(path, 0)) < 0) 
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // Get file status of the directory
    if (fstat(fd, &st) == -1) 
    {
        fprintf(2, "find: cannot fstat %s\n", path);
        close(fd);
        return;
    }

    // Switch statement to handle different types of files/directories
    switch (st.type) 
    {

        case T_DEVICE:
        case T_FILE:
            fprintf(2, "find: %s not a path value.\n", path);
            close(fd);
            // printf("==='%s' is a File\n", path);
            break;
        case T_DIR:
            // Check if the path length exceeds the buffer size
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
            {
                printf("ls: path too long\n");
                break;
            }
            
            // Create the full path by copying the original path
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            
            // Read directory information for files and subdirectories
            while (read(fd, &de, sizeof(de)) == sizeof de) 
            {
                if (de.inum == 0)
                    continue;
                
                // Skip the current directory (.) and parent directory (..)
                if (strcmp(".", rtrim(de.name)) == 0 || strcmp("..", rtrim(de.name)) == 0)
                    continue;
                
                // Append the file/subdirectory name to the path
                memmove(p, de.name, DIRSIZ);
                
                // Null-terminate the path
                p[DIRSIZ] = '\0';
                
                // Get the file status of the current file/subdirectory
                if (stat(buf, &st) == -1) 
                {
                    fprintf(2, "find: cannot stat '%s'\n", buf);
                    continue;
                }
                
                // Print the full path if the file name matches the search name
                if (st.type == T_DEVICE || st.type == T_FILE) 
                {
                    if (strcmp(name, rtrim(de.name)) == 0) 
                    {
                        printf("%s\n", buf);
                        /*for (int i = 0; buf[i] != '\0'; ++i) 
                        {
                            printf("'%d'\n", buf[i]);
                        }*/

                    }
                }
                // Recursively search if the entry is a directory
                else if (st.type == T_DIR) 
                {
                    find(buf, name);
                }
            }
    }
}

int 
main(int argc, char* argv[])
{
    // Check if the number of command-line arguments is correct
    if (argc != 3) 
    {
        fprintf(2, "Usage: find path file.\n");
        exit(0);
    }
    
    char* path = argv[1];  // Extract the path from command-line arguments
    char* name = argv[2];  // Extract the file name to search from command-line arguments
    
    // Call the find function to search for the file
    find(path, name);
    
    exit(0);
}