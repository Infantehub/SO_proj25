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

    board_t *board = malloc(sizeof(board_t));
    int monster_file_count = 0;
    int pacman_file_count = 0;

    if (f_name[-1] == 'l') analyse_level(buffer, board);

    else if(f_name[-1] == 'm'){
        analyse_monster(buffer, board, monster_file_count);
        monster_file_count++;
    } 
    else if(f_name[-1] == 'p') {
        analyse_char_pacman(buffer, board, pacman_file_count);
        pacman_file_count++;
    }

    free(buffer);
    /* close the file */
    close(fd);
    return EXIT_SUCCESS;
}

void analyse_level(char buf[], board_t *board) {
    // Placeholder function to analyze a character
    // Implement your character analysis logic here

    int pos_index = 0;

    for(int i = 0; buf[i] != '\0'; i++){
        if (buf[i] == '#') {
            while (buf[i] != '\n' || buf[i] != '\0') {
                i++;
            }
            continue;
        };
    
        if(buf[i] == 'D'){
            board ->width = buf[i + 4];
            board ->height = buf[i + 6];
            board->board = malloc(board->width * board->height * sizeof(board_pos_t));
            continue;
        }

        if(buf[i] == 'T'){
            int num = 0;
            int c = 6;
            while (isdigit(buf[i + c])){
                num = num * 10 + (buf[i + c] - '0');
                c++;
            }
            board ->tempo = num;
            continue;
        }

        if(buf[i] == 'P'){
            char pac_file[MAX_FILENAME];
            int c = 4;
            int j = 0;
            while(buf[i + c] != '\n'){
                pac_file[j] = buf[i + c];
                j++;
                c++;
            }
            board->n_pacmans = 1; //apenas 1 pacman
            board->pacmans = malloc(board->n_pacmans * sizeof(pacman_t));
            pac_file[j] = '\0';
            /* use bounded copy to avoid potential issues and ensure null-termination */
            strncpy(board->pacman_file, pac_file, MAX_FILENAME - 1);
            board->pacman_file[MAX_FILENAME - 1] = '\0';

            continue;
        }
            
        if(buf[i] == 'M') {
            char mo_files[MAX_GHOSTS][MAX_FILENAME];
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
            board->n_ghosts = file_count + 1; // número de ficheiros é o número de fantasmas
            board->ghosts = malloc(board->n_ghosts * sizeof(ghost_t));

            for(int i = 0; i < file_count; i++){
                strncpy(board->ghosts_files[i], mo_files[i], MAX_FILENAME - 1);
                board->ghosts_files[i][MAX_FILENAME - 1] = '\0';
            }

            continue;
        }
        //Posições do tabuleiro - têm que ser preenchidos todos os atributos de board_pos_t

        if(buf[i] == 'X'){
            board->board[pos_index].content = buf[i];
            board->board[pos_index].has_dot = 0;
            board->board[pos_index].has_portal = 0;
            pos_index++;
            continue;
        }
        if(buf[i] == 'o'){
            board->board[pos_index].content = buf[i];
            board->board[pos_index].has_dot = 1;
            board->board[pos_index].has_portal = 0;
            pos_index++;
            continue;
        }
        if(buf[i] == '@'){
            board->board[pos_index].content = buf[i];
            board->board[pos_index].has_portal = 1;
            board->board[pos_index].has_dot = 0;
            pos_index++;
            continue;
        }
    }
}

void analyse_monster(char buf[], board_t *board, int m_count) {
    // Placeholder function to analyze a character
    // Implement your character analysis logic here
    int pos_index = 0;

    for(int i = 0; buf[i] != '\0'; i++){
        if (buf[i] == '#') {
            while (buf[i] != '\n' || buf[i] != '\0') {
                i++;
            }
            continue;
        }

        if ( buf[i] == 'T' &&buf[i + 1] == 'A'){
            board -> ghosts[m_count].passo = buf[i + 6];
            continue;
        }
        if ( buf[i] == 'T' && buf[i + 1] == 'O'){
            board -> ghosts[m_count].pos_x = buf[i + 4];
            board -> ghosts[m_count].pos_y = buf[i + 6];
            continue;
        }
        if ( buf[i] == 'T' && buf[i + 1] == ' '){
                //FIXME TODO
                //Jogada nula???
            }
        if ( buf[i] == 'C'){/*whats this??*/}

        //FIXME no can be because monsters move automatically

        if ( buf[i] == 'A'){
            write(1, "A", 1);
            continue;
        }
        if ( buf[i] == 'D'){
            write(1, "D", 1);
            continue;
        }
        if ( buf[i] == 'W'){
            write(1, "W", 1);
            continue;   
        }
        if ( buf[i] == 'S'){
            write(1, "S", 1);
            continue;
        }
        
    }
}

void analyse_pacman(char buf[], board_t *board, int p_count) {
    // Placeholder function to analyze a character
    // Implement your character analysis logic here

    int pos_index = 0;

    for(int i = 0; buf[i] != '\0'; i++){
        if (buf[i] == '#') {
            while (buf[i] != '\n' || buf[i] != '\0') {
                i++;
            }
            continue;
        }

        if ( buf[i] == 'T' && buf[i + 1] == 'A'){
            board -> ghosts[p_count].passo = buf[i + 6];
            continue;
        }
        if ( buf[i] == 'T' && buf[i + 1] == 'O'){
            board -> ghosts[p_count].pos_x = buf[i + 4];
            board -> ghosts[p_count].pos_y = buf[i + 6];
            continue;
        }
        if ( buf[i] == 'T' && buf[i + 1] == ' '){
                //FIXME TODO
                //Jogada nula???
            }
        if ( buf[i] == 'C'){/*whats this??*/}

        //FIXME is this the supposed?

        if ( buf[i] == 'A'){
            write(1, "A", 1);
            continue;
        }
        if ( buf[i] == 'D'){
            write(1, "D", 1);
            continue;
        }
        if ( buf[i] == 'W'){
            write(1, "W", 1);
            continue;   
        }
        if ( buf[i] == 'S'){
            write(1, "S", 1);
            continue;
        }
        
    }
}

int main(int argc, char* argv[]) {
    return /*open_read(argc, argv)*/1;
}