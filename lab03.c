#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "passwords.h"
#include "sha256.h"

#define DIG_BIN_LEN 32
#define DIG_STR_LEN ((DIG_BIN_LEN * 2) + 1)
#define DICTIONARY_SIZE 10000
#define PASSWORD_SIZE 64


// using typedef struct for exisitng data type
 typedef struct Dictionary
{
    char normal[PASSWORD_SIZE], leet[PASSWORD_SIZE],plusOne[PASSWORD_SIZE];
    char normalHash[DIG_STR_LEN],leetHash[DIG_STR_LEN], plusOneHash[DIG_STR_LEN];
}Passwords;

// Function prototypes
void sha256_hash(char *dest, char *src);
void transform_password(char *password);
void add_one(char *password);
void write_dictionary();

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        // Check if the dictionary file exists
        FILE *fp = fopen("dict.txt", "r");
        if (fp == NULL)
        {
            // If the dictionary file doesn't exist, create it
            write_dictionary();
            printf("dictionary built\n");
        }
        else
        {
            printf("dictionary exists\n");
            fclose(fp);
        }
    }
    else if (argc == 2)
    {
   		    char *given_hash = (char *)argv[1];
            char *hash = malloc(DIG_STR_LEN);
            char *passkey = malloc(256);
    
            // Open the dictionary file
            int fd = open("dict.txt", O_RDONLY);   // O_RDONLY to open file in only read mode
            if (fd == -1)
            {
                perror("Error opening file");
                exit(EXIT_FAILURE);
            }

            // malloc pointer to get memory allocation 
            Passwords *pss = malloc(sizeof(Passwords));

            // while loop as *pss runs through the passwords
            while (read(fd, pss, sizeof(Passwords)) > 0)
            {
                
                // Check if the given hash matches any of the hashes in the dictionary
                // arrow operator is used with pointer to store the value of var where it's pointing
                if (strcmp(pss->normalHash, given_hash) == 0)
                {
                    strcpy(passkey, pss->normal);
                    break;
                }
                else if (strcmp(pss->leetHash, given_hash) == 0)
                {
                                strcpy(passkey, pss->leet);
                                break;
                            }
                            else if (strcmp(pss->plusOneHash, given_hash) == 0)
                            {
                                strcpy(passkey, pss->plusOne);
                                break;
                            }
                        }
                
                        if (passkey[0] == '\0')
                        {
                            printf("Not found\n");
							return -1;
                           
                        }
                        else
                        {
                            printf("%s\n", passkey);
                            return 0;
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Usage: %s <password>\n", argv[0]);
                       		

                       		return -1;
                    }
                }

// Hashing function (given in project01.c)
 void sha256_hash(char *dest, char *src)
{
 	struct sha256_ctx ctx;
 	memset(&ctx, 0, sizeof(ctx));
               
	unsigned char dig_bin[DIG_BIN_LEN];
	memset(dig_bin, 0, DIG_BIN_LEN);
               
  	memset(dest, 0, DIG_STR_LEN);
               
 	__sha256_init_ctx(&ctx);
 	__sha256_process_bytes(src, strlen(src), &ctx);
 	__sha256_finish_ctx(&ctx, dig_bin);

 	for (int i = 0; i < DIG_BIN_LEN; i++)
 	{
		snprintf(dest, 3, "%02x", dig_bin[i]);
		dest += 2;
  	}
}

 // Transformation function (leet function from project01.c)
 void transform_password(char *password)
 {
     for (int i = 0; i < strlen(password); i++)
     {
         switch (password[i])
         {
         case 'o':
             password[i] = '0';
             break;
         case 'e':
             password[i] = '3';
             break;
         case 'i':
             password[i] = '!';
             break;
         case 'a':
             password[i] = '@';
             break;
         case 'h':
             password[i] = '#';
             break;
         case 's':
             password[i] = '$';
             break;
         case 't':
             password[i] = '+';
             break;
         }
     }
 }

 // Attaches '1' to the end of the string password
 void add_one(char *password)
 {
     strcat(password, "1");	
 }

 // Write the dictionary to a file
 void write_dictionary()
 {
 	/* O_CREATE to createa file only if it doesn't exist 
 	 * O_WRONLY to open file to write in
 	 * S_IRUSR is to read permission for owner of the file
 	 * S_IWUSR is to write permission for owner fo the file	
 	 * used here to create/ open and write in the file */
 	 
     int fd = open("dict.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR); 
     if (fd == -1)
     {
         perror("Error opening file");
         exit(-1);
     }
 
     for (int i = 0; i < DICTIONARY_SIZE; i++)
     {
         Passwords *pss = malloc(sizeof(Passwords));
 
         strcpy(pss->normal, passwords[i]);
         strcpy(pss->leet, passwords[i]);
         strcpy(pss->plusOne, passwords[i]);
 
         sha256_hash(pss->normalHash, pss->normal);
         transform_password(pss->leet);
         sha256_hash(pss->leetHash, pss->leet);
         add_one(pss->plusOne);
         sha256_hash(pss->plusOneHash, pss->plusOne);
 
         write(fd, pss, sizeof(Passwords));
         free(pss); // deallocate the pointer 
     }
     close(fd);
 }
