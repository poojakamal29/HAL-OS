#ifndef _FILESYS_H
#define _FILESYS_H

#define FNAME_MAX_LEN 32
#define MAX_DENTRY 63
#define FOURKB 4096
#define FILENAME_MAXLEN 32

#include "types.h"
#include "structures.h"

/*Points to the very beginning of the filesystem*/
boot_block_t * filesys_begin;
/*Points to the beginning of the inodes in the filesystem*/
inode_t * inodes_begin;
/*Points to the beginning of the data blocks in the filesystem*/
data_block_t * data_block_begin;
/*The number of inodes within the file system*/
uint32_t num_inodes;

//initializes the file system
void init_filesys(boot_block_t * bb);

//fills in a dentry when given an index by name
int32_t read_dentry_by_name(const uint8_t * fname, dentry_t * dentry);

//reads data from a file
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length);

//finds and records a dentry when given a numerical index
int32_t read_dentry_by_index(uint32_t index, dentry_t * dentry);

//compares to strings to check if they are equal
int32_t stringcompare(const uint8_t * a, const uint8_t * b, int cmplen);

//get the length of a string
int32_t stringlength(const uint8_t * string);

//open a file
int32_t file_open(const uint8_t * filename);

//writes to a file
int32_t file_write(int32_t fd, const void * buf, int32_t n_bytes);

//reads data from a file into a buffer
int32_t file_read(uint32_t inode_index, uint32_t offset, uint8_t * buf, uint32_t nbytes);

//close a file descriptor
int32_t file_close(int32_t fd);

//open a directory
int32_t dir_open(const uint8_t * filename);

//read a directory
int32_t dir_read(uint32_t inode_index, uint32_t offset, uint8_t * buf, uint32_t nbytes);

//write to a directroy
int32_t dir_write(int32_t fd, const void * buf, int32_t n_bytes);

//close a directory file descriptor
int32_t dir_close(int32_t fd);

#endif
