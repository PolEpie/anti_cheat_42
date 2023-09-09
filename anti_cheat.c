/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anti_cheat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 23:26:21 by pepie             #+#    #+#             */
/*   Updated: 2023/09/09 23:26:29 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>


int verify_printf(FILE *file)
{
    int is_commented;
    int commented_line;
    int c;
    char last_char;
    int is_in_print;
    int is_in_main;
    int line_number;

    is_commented = 0;
    is_in_print = 0;
    is_in_main = 0;
    commented_line = 0;
    line_number = 1;
    while ((c = fgetc(file)) != EOF)
    {
        if (last_char == "/" && c == '*')
            is_commented = 1;

        if (last_char == "*" && c == '/')
            is_commented = 0;

        if (last_char == '/' && c == '/')
            commented_line = 1;

        if (c == '\n')
        {
            commented_line = 0;
            line_number++;
        }    

        if (c == 'p' && commented_line == 0 && is_commented == 0 && (last_char == '\t' || last_char == ' '))
            is_in_print = 1;
        else if (c == 'r' && last_char == 'p' && is_in_print )
            is_in_print = 1;
        else if (c == 'i' && last_char == 'r' && is_in_print)
            is_in_print = 1;
        else if (c == 'n' && last_char == 'i' && is_in_print)
            is_in_print = 1;
        else if (c == 't' && last_char == 'n' && is_in_print)
            is_in_print = 1;
        else if (c == 'f' && last_char == 't' && is_in_print)
            is_in_print = 1;
        else if (c == '(' && last_char == 'f' && is_in_print)
            printf("[ ERROR ] printf detected at line: %d\n", line_number);
        else
            is_in_print = 0;

        if (c == 'm' && commented_line == 0 && is_commented == 0 && (last_char == '\t' || last_char == ' '))
            is_in_main = 1;
        else if (c == 'a' && last_char == 'm' && is_in_main)
            is_in_main = 1;
        else if (c == 'i' && last_char == 'a' && is_in_main)
            is_in_main = 1;
        else if (c == 'n' && last_char == 'i' && is_in_main)
            is_in_main = 1;
        else if (c == '(' && last_char == 'n' && is_in_main)
            printf("[ MAIN ] main detected at line: %d\n", line_number);
        else
            is_in_main = 0;

        last_char = c;
    }
}

void findCFiles(const char *dir) {
    DIR *dp;
    struct dirent *entry;

    dp = opendir(dir);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_DIR) {
            // Ignore "." and ".." directories
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".git") == 0) {
                continue;
            }
            
            // Recursively call the function for subdirectories
            char subDirPath[256];
            snprintf(subDirPath, sizeof(subDirPath), "%s/%s", dir, entry->d_name);
            findCFiles(subDirPath);
        } else if (entry->d_type == DT_REG) {
            // Check if the file has a .c extension
            char *extension = strrchr(entry->d_name, '.');
            if (extension && strcmp(extension, ".c") == 0) {
                printf("________________\n");
                printf("Found C file: %s/%s\n", dir, entry->d_name);
                FILE *file = fopen(entry->d_name, "r");

                verify_printf(file);

                if (file == NULL) {
                    perror("Error opening file");
                }
                fclose(file);
            }
            else if (strcmp(entry->d_name, "a.out") == 0)
            {
                printf("______________\n");
                printf("| build_file | Found a.out file: %s/%s\n", dir, entry->d_name);

                if (remove(entry->d_name) == 0) {
                    printf("File %s removed successfully.\n", entry->d_name);
                } else {
                    perror("Error removing file");
                }
            }
            else
            {
                if (extension && strcmp(extension, ".h") == 0)
                    continue;
                printf("______________\n");
                printf("| other file | Found file: %s/%s (i'll not delete it)\n", dir, entry->d_name);
            }
        }
    }

    closedir(dp);
}

int main(int argc, char **argv)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
       perror("getcwd() error");
       return 1;
    }
    
    const char *dir = cwd;
    printf("Current working dir: %s\n", dir);
    findCFiles(dir);

    return (0);
}

