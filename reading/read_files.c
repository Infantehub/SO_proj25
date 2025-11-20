#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>  // for S_IRUSR, S_IWUSR
#include "../include/board.h"

#define STRIDE 512

void open_read_file(char f_name[]) {
     /**
     * the attributes are:
     * - O_RDONLY: open the file for reading
     */
    int fd = open(f_name, O_RDONLY);

    if (fd < 0) { //Opeening error
        perror("open error");
        return EXIT_FAILURE;
    }

    char* buffer = malloc(STRIDE + 1);

    /* read the contents of the file */
    int done = 0;
    while (1) {
        int bytes_read = read(fd, buffer + done, STRIDE);

        if (bytes_read < 0) { //Reading error
            perror("read error");
            free(buffer);
            close(fd);
            return EXIT_FAILURE;
        }

        // if we read 0 bytes, we're done, EOF
        if (bytes_read == 0) {
            break;
        }
        //it might not have managed to read all data.

        done += bytes_read;

        char* new_buffer = realloc(buffer, 2*bytes_read + 1 + done);
        
        buffer = new_buffer; 
    }

    int state = 1;
    board_t *board = malloc(sizeof(board_t)); //FIXME not sure about this

    for (int i = 0; i < done; i++) {
        if (buffer[i] == '#') state = 0;
        else if (state == 1 && buffer[i] == '\n') state = 1;

        else if (state == 1) {
            if (f_name[-1] == 'l') analyse_char_level(buffer, i, board);
            else analyse_char_caracter(buffer, i);
        }
    }

    free(buffer);
    /* close the file */
    close(fd);
    return EXIT_SUCCESS;
}

void analyse_char_level(char buf[], int i, board_t *board) {
    // Placeholder function to analyze a character
    // Implement your character analysis logic here

    switch (buf[i]){
        case 'D':{
            board ->width = buf[i + 4];
            board ->height = buf[i + 6];}
            break;
        case 'T':{
            int num = 0;
            int c = 6;
            while (isdigit(buf[i + c])){
                num = num * 10 + (buf[i + c] - '0');
                c++;
            }
            board ->tempo = num;}
            break;
        case 'P':{
            char pac_file[256];
            int c = 4;
            int j = 0;
            while(buf[i + c] != '\n'){
                pac_file[j] = buf[i + c];
                j++;
                c++;
            }
            pac_file[j] = '\0';
            strcpy(board->pacman_file, pac_file);}
            break;
            
        case 'M': {
            char mo_files[25][MAX_FILENAME];
            int file_count = 0;
            int c = i + 4;  // começa a ler depois do 'M'
            int j = 0;

            while (buf[c] != '\n' && buf[c] != '\0' && file_count < 25) {
                if(buf[c] == ' ') {
                    file_count++;
                    j = 0;
                    c++;
                }
                else{
                    mo_files[file_count][j] = buf[c];
                    c++;
                    j++;
                }
                
            }

            for(int i = 0; i < file_count; i++){
                strcpy(board->ghosts_files[i], mo_files[i]);
            }
        }
        break;
        case 'X':
        case 'o':
        case '@':
            }
    
}

void analyse_char_caracter(char buf[], int i) {
    // Placeholder function to analyze a character
    // Implement your character analysis logic here

    switch (buf[i]){
        case 'T':
            if (buf[i + 1] == 'A'){}
            if (buf[i + 1] == 'O'){}
            if (buf[i + 1] == ' '){}
        case 'C':
        case 'A':
        case 'D':
        case 'W':
        case 'S':
        
    }
}

int main(int argc, char* argv[]) {
    return /*open_read(argc, argv)*/1;
}